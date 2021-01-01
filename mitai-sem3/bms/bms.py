#!/usr/bin/env python3.8

"""
VUT FIT MITAI
BMS 2020/2021
Project: Convolutional coder and Viterbi algorithm
Author: Vladimir Dusek
Login: xdusek27
"""

from argparse import ArgumentParser
from copy import deepcopy
from logging import basicConfig, getLogger
from random import choice as random_choice
from sys import argv, exit, stderr, stdin

MEMORY_SIZE = 5
MASTER_NODE_ID = 0
LOGGING_FORMAT = '%(asctime)s | %(levelname)s | %(message)s'

basicConfig(format=LOGGING_FORMAT, level='ERROR')
logger = getLogger(__name__)


def error(message: str):
    """
    Print error message to the stderr and exit with 2.
    """
    print('usage: bms.py [-h] [-e] [-d]\nbms.py: {}'.format(message), file=stderr)
    exit(2)


class NidCounter:

    def __init__(self):
        self.nid = MASTER_NODE_ID

    def get(self) -> int:
        """
        Get ID of the next node.

        Returns:
            int: ID of the next node
        """
        self.nid += 1
        return self.nid


nid_counter = NidCounter()


class ArgParser:

    def __init__(self):
        self.parser = ArgumentParser()

        self.parser.add_argument(
            '-e',
            '--encode',
            required=False,
            action='store_true',
            help='encoding process'
        )

        self.parser.add_argument(
            '-d',
            '--decode',
            required=False,
            action='store_true',
            help='decoding process'
        )

    def parse(self, argv: list) -> object:
        """
        Parse command line arguments.

        Args:
            argv (list): standard gnu argv

        Returns:
            object: arguments
        """

        args = self.parser.parse_args(argv[1:])

        if (args.encode and args.decode) or (not (args.encode or args.decode)):
            error('error during argument parsing has occurred')

        return args


class Reader:

    def read(self) -> str:
        """
        Read the input from stdin and returns it as a string. Remove all the whitespaces
        and non ASCII characters.

        Returns:
            str: stdin as string
        """
        logger.info('reading from stdin')

        buffer = ''
        for line in stdin:
            buffer += line

        logger.info('buffer = {}'.format(buffer))

        # remove non ASCII characters
        buffer_processed = ''
        for char in buffer:
            if char.isascii():
                buffer_processed += char

        # remove white spaces
        buffer_processed = ''.join(buffer_processed.split())

        logger.info('buffer_processed = {}'.format(buffer_processed))

        return buffer_processed


class State:

    def __init__(self, size: int):
        """
        Args:
            size (int): memory size of the state
        """
        self.size = size
        self.memory = [0 for _ in range(0, size)]

    def __repr__(self) -> str:
        return ''.join(str(x) for x in self.memory)

    def push(self, digit: int):
        """
        Push digit to the memory.

        Args:
            digit (int): digit to be pushed
        """
        self.memory = [digit] + self.memory[0:self.size - 1]

    def get_out_hob(self, in_digit: int) -> int:
        """
        Get HOB (high order bit) of the output.

        Args:
            in_digit (int): digit from the input of the coder

        Returns:
            int: high order bit (HOB)
        """
        digits = [in_digit, self.memory[0], self.memory[2], self.memory[4]]
        return sum(digits) % 2

    def get_out_lob(self, in_digit: int) -> int:
        """
        Get LOB (low order bit) of the output.

        Args:
            in_digit (int): digit from the input of the coder

        Returns:
            int: low order bit (LOB)
        """
        digits = [in_digit, self.memory[1], self.memory[2], self.memory[3]]
        return sum(digits) % 2


class Encoder:

    def __init__(self, in_str: str):
        """
        Args:
            in_str (str): input string
        """
        self.in_str = in_str
        self.in_bin = self.str_to_bin(self.in_str, MEMORY_SIZE)
        self.out_str = ''
        logger.info('input in binary: "{}"'.format(self.in_bin))

    def process(self):
        """
        Process the given input.
        """
        logger.info('Encoder processing')
        self._encode()

    def _encode(self):
        memory = State(MEMORY_SIZE)
        logger.info('input: "{}"\n'.format(self.in_bin))

        for in_digit in self.in_bin[::-1]:
            logger.info('in_digit: "{}"'.format(in_digit))
            logger.info('memory: "{}"'.format(memory))

            char_int = int(in_digit)
            out1 = str(memory.get_out_hob(char_int))
            out0 = str(memory.get_out_lob(char_int))
            memory.push(char_int)

            self.out_str = out1 + out0 + self.out_str
            logger.info('output: "{}"\n'.format(self.out_str))

    @staticmethod
    def str_to_bin(string: str, padding_size: int) -> str:
        """
        Convert string "string" to its binary format.
        Add zero padding with "padding_size".

        Args:
            string (str): string to be converted
            padding_size (int): zero padding

        Returns:
            str: string in binary
        """
        str_bin = ''.join(f'{ord(c):08b}' for c in string)
        return padding_size * '0' + str_bin


class Node:

    def __init__(self, depth: int, state: State, hdist_cum: int, nid=None, parent_nid=None):
        """
        Args:
            depth (int): depth node in the DAG
            state (State): state of the node
            hdist_cum (int): Hamming Distance cumulative
            nid ([type], optional): node ID
            parent_nid ([type], optional): id of the parent node
        """
        self.nid = nid_counter.get() if nid is None else nid
        self.depth = depth
        self.state = state
        self.hdist_cum = hdist_cum
        self.parent_nid = parent_nid

    def __repr__(self) -> str:
        return '<Node: nid={}, depth={}, state={}, hdist_cum={}, parent_nid={}>'.format(
            self.nid,
            self.depth,
            self.state,
            self.hdist_cum,
            self.parent_nid
        )

    def generate(self, chars: str) -> object:
        """
        Generate both successors based on the input characters.

        Args:
            chars (str): input characters

        Returns:
            object: generated successors
        """
        state0 = deepcopy(self.state)
        state0.push(0)

        state1 = deepcopy(self.state)
        state1.push(1)

        hdist0 = self.get_hdist(0, chars)
        hdist1 = self.get_hdist(1, chars)

        node0 = Node(depth=self.depth + 1, state=state0, hdist_cum=self.hdist_cum + hdist0, parent_nid=self.nid)
        node1 = Node(depth=self.depth + 1, state=state1, hdist_cum=self.hdist_cum + hdist1, parent_nid=self.nid)

        return node0, node1

    def get_hdist(self, in_digit: int, chars: str) -> int:
        """
        Get Hamming Distance of the output and expected output.

        Args:
            in_digit (int): input digit
            chars (str): input characters

        Returns:
            int: Hamming Distance
        """
        hob = int(chars[0])
        lob = int(chars[1])
        exp_hob = self.state.get_out_hob(in_digit)
        exp_lob = self.state.get_out_lob(in_digit)
        return abs(hob - exp_hob) + abs(lob - exp_lob)


class DAG:

    def __init__(self, root: Node):
        """
        Args:
            root (Node): root node of the DAG
        """
        self.nodes = [root]

    def __repr__(self) -> str:
        string = '<DAG\n'
        for node in self.nodes:
            string += '    {}\n'.format(node)
        return '{}>'.format(string)

    def get_node_by_nid(self, nid: int) -> Node:
        """
        Get node with specific nid

        Args:
            nid (int): node id

        Returns:
            Node: node with specified nid
        """
        for node in self.nodes:
            if node.nid == nid:
                return node

    def get_node_by_state_and_depth(self, state: State, depth: int) -> Node:
        """
        Get node based on state and depth

        Args:
            state (State): state of the node
            depth (int): depth of the node

        Returns:
            Node: the node
        """
        for node in self.nodes:
            if node.state.memory == state.memory and node.depth == depth:
                return node

        return None

    def get_nodes_by_depth(self, depth: int) -> list:
        """
        Get node based on depth

        Args:
            depth (int): depth of the node

        Returns:
            Node: the node
        """
        return [node for node in self.nodes if node.depth == depth]

    def connect_node(self, node: Node, parent_nid: int):
        """
        Connect node to the DAG

        Args:
            node (Node): node to be connected
            parent_nid (int): parent to be connected to
        """
        competive_node = self.get_node_by_state_and_depth(node.state, node.depth)

        if competive_node:
            if competive_node.hdist_cum > node.hdist_cum:
                self.nodes.remove(competive_node)
                self.nodes.append(node)
        else:
            self.nodes.append(node)

    def get_best_nodes(self, depth: int) -> Node:
        """
        Get best nodes in the specific depth

        Args:
            depth (int): get nodes in this depth

        Returns:
            Node: best node
        """
        nodes = self.get_nodes_by_depth(depth)
        best_nodes = [nodes.pop(0)]

        for node in nodes:
            if node.hdist_cum < best_nodes[0].hdist_cum:
                best_nodes = [node]
            elif node.hdist_cum == best_nodes[0].hdist_cum:
                best_nodes.append(node)

        return best_nodes

    def get_viterbi_path(self, node: Node) -> [Node]:
        """
        Get Vitterbi Path from the Node to the beggining

        Args:
            node (Node): Start (end) of the Vitterbi Path

        Returns:
            [Node]: Vitterbi Path as a list of nodes
        """
        viterbi_path = [node]
        prev_node = self.get_node_by_nid(node.parent_nid)

        while prev_node is not None:
            viterbi_path.append(prev_node)
            node = prev_node
            prev_node = self.get_node_by_nid(node.parent_nid)

        return viterbi_path


class Decoder:

    def __init__(self, in_str: str):
        """
        Args:
            in_str (str): input string
        """
        self.in_str = in_str
        self.out_str = ''
        self._check()

    def _check(self):
        """
        Check if the input string is a valid binary number with even length.
        """
        if set(self.in_str) in [{'0', '1'}, {'0'}, {'1'}]:
            pass
        else:
            error('error during decoding: non binary digit have occured')

        if len(self.in_str) % 2 == 0:
            pass
        else:
            error('error during decoding: input length has to be even number')

    def process(self):
        """
        Process the given input.
        """
        logger.info('Decoder processing')
        self._decode()

    def _decode(self):
        # Initialization of the Graph with master node
        master_node = Node(nid=MASTER_NODE_ID, depth=0, state=State(MEMORY_SIZE), hdist_cum=0)
        dag = DAG(master_node)
        logger.info('{}'.format(dag))

        # Create a list of pair characters from input and reverse it
        in_str_pairs = [self.in_str[i:i+2] for i in range(0, len(self.in_str), 2)]
        in_str_pairs.reverse()
        max_depth = len(in_str_pairs)
        logger.info('input string as a list of pairs:\n    {}'.format(in_str_pairs))

        # Iterate over the input and generate all nodes in everylayer
        for chars, depth in zip(in_str_pairs, range(0, max_depth + 1)):

            logger.info('***** {}. iteration *****'.format(depth + 1))
            logger.info('input: {}, depth: {}'.format(chars, depth))
            nodes = dag.get_nodes_by_depth(depth)

            # From all the nodes in the layer execute generate
            for node in nodes:
                succ0, succ1 = node.generate(chars)
                dag.connect_node(succ0, node.nid)
                dag.connect_node(succ1, node.nid)

            logger.info(dag)
            logger.info('Total number of nodes = {}'.format(len(dag.nodes)))

        # Select the best node/nodes in the last layer since there could be more best nodes -
        # nodes with the lowest cumulative Hammng Distance
        logger.info('')
        logger.info('The end of the generating')
        logger.info("Let's select the best node/nodes in the last layer")
        best_nodes = dag.get_best_nodes(max_depth)
        logger.info('Best nodes = {}'.format(best_nodes))

        viterbi_paths = []

        # For every best node in the best nodes
        for best_node in best_nodes:
            # Get the Viterbi path
            viterbi_path = dag.get_viterbi_path(best_node)
            logger.info('Viterbi path = {}'.format(viterbi_path))
            viterbi_paths.append(viterbi_path)

        result_ints = []

        # For every viterbi path in the viterbi paths
        for viterbi_path in viterbi_paths:
            # Extract the result string from the Viterbi path
            result_string = ''
            for node in viterbi_path:
                result_string += str(node.state)[0]

            # Cut 5 zeros from the beginning (5 bit memory) and 1 from the end (default state)
            logger.info('result before cut: {}'.format(result_string))
            result_bin = result_string[MEMORY_SIZE:-1]
            logger.info('result in binary: {}'.format(result_bin))
            result_int = self.bin_to_ints(result_bin)
            logger.info('result in int: {}'.format(result_int))

            result_ints.append(result_int)

        # Results containing only hexadecimal characters
        result_asciis_hexdec = []

        # Results containing not hexadecimal characters as well - not prefered
        result_asciis_other = []

        # For every list of ints
        for result_int in result_ints:
            result_ascii = ''
            all_hexdec = True

            # Check every integer if its hexadecimal
            for integer in result_int:
                result_ascii += chr(integer)
                if not self.is_hexadecimal(integer):
                    all_hexdec = False

            if all_hexdec:
                result_asciis_hexdec.append(result_ascii)
            else:
                result_asciis_other.append(result_ascii)

        # If there's some pure hexadecimal result
        if result_asciis_hexdec:
            self.out_str = random_choice(result_asciis_hexdec)
        elif result_asciis_other:  # noqa: SIM106
            self.out_str = random_choice(result_asciis_other)
        else:
            raise Exception

    @staticmethod
    def bin_to_ints(bins: str) -> list:
        """
        Convert string binary to list of integers (8 bits)

        Args:
            bins (str): binary in string

        Returns:
            list: list of integers
        """
        result = []

        str_eights = [bins[i:i+8] for i in range(0, len(bins), 8)]

        for str_eight in str_eights:
            result.append(int(str_eight, 2))

        return result

    @staticmethod
    def is_hexadecimal(char: int) -> bool:
        """
        Represents integer hexadecimal character?

        Args:
            char (int): integer to be checked

        Returns:
            bool: that's clear
        """
        if char >= 65 and char <= 90:
            return True
        elif char >= 97 and char <= 122:
            return True
        elif char >= 48 and char <= 57:  # noqa: SIM103
            return True
        else:
            return False


def main():
    arg_parser = ArgParser()
    args = arg_parser.parse(argv)

    logger.info('encode: {}'.format(args.encode))
    logger.info('decode: {}'.format(args.decode))

    reader = Reader()
    input_str = reader.read()

    logger.info('input string: "{}"'.format(input_str))

    if args.encode:
        coder = Encoder(input_str)
    elif args.decode:
        coder = Decoder(input_str)
    else:
        error('internal error')

    coder.process()

    logger.info('Print coder output')
    print(coder.out_str)


if __name__ == '__main__':
    main()
