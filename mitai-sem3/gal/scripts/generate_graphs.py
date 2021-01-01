#!/usr/bin/env python3

"""
VUT FIT MITAI
GAL 2020/2021
Project: Topic 38 - Graph Radio Coloring Parallelization
Authors: Vladimir Dusek, Patrik Goldschmidt

Module description:
    Generate random graphs and save them to files

Raises:
    FileExistsError: if data are already generated
"""

from copy import deepcopy
from logging import basicConfig, getLogger
from os import chdir, getcwd, mkdir
from os.path import join as path_join
from random import randint

# Minimum graph size (number of vertexes)
MIN_GRAPH_SIZE = 5

# Step graph size
STEP = 5

# Maximum graph size (number of vertexes)
MAX_GRAPH_SIZE = 200

# How many graphs per size should be generated
GRAPHS_PER_SIZE = 10

# Where graphs should be stored
DST_DIR = 'data_graphs/'

# Logging
LOGGING_FORMAT = '%(levelname)s | %(funcName)s | %(lineno)s | %(message)s'  # noqa: WPS323
basicConfig(format=LOGGING_FORMAT, level='INFO')
logger = getLogger(__name__)


def get_graph(size: int) -> list:
    """
    Generate random undirected graph represented by adjacency matrix.

    Args:
        size (int): size of the graph

    Raises:
        Exception: when invalid size

    Returns:
        list: adjacency matrix
    """
    if size < 0:
        raise Exception('Invalid graph size')

    graph = []

    # Part 1 - random gen first part
    for i in range(size):
        row = []
        for _ in range(i):
            row.append(randint(0, 1))
        graph.append(row)

    logger.debug('Graph after part 1 = {}'.format(graph))

    # Part 2 - if there's a zero only row, switch random 0 to 1, we don't want to generate disconnected graphs
    for row in graph:
        if row:
            only_zeros = True
            for elem in row:
                if elem == 1:
                    only_zeros = False
                    break
            if only_zeros:
                idx = randint(0, len(row) - 1)
                row[idx] = 1

    logger.debug('Graph after part 2 = {}'.format(graph))

    graph_copied = deepcopy(graph)

    # Part 3 - fill diagonale
    for i in range(size):
        graph[i].append(0)

    logger.debug('Graph after part 3 = {}'.format(graph))

    # Part 4 - transpose the content from first part
    for i in range(size):
        row_transposed = []
        for row in graph_copied:
            if len(row) > i:
                row_transposed.append(row[i])
        graph[i] += row_transposed

    logger.debug('Graph after part 4 = {}'.format(graph))

    return graph


def save_graph(graph: list, file_name: str):
    """
    Save graph to a file.

    Args:
        graph (list): adjacency matrix
        file_name (str): file name
    """
    logger.info('saving graph {}'.format(file_name))
    logger.debug('graph content = {}'.format(graph))
    with open(file_name, 'w') as fp:
        fp.write('{}\n'.format(str(graph).replace(' ', '')))


if __name__ == '__main__':

    logger.info('destination directory = {}'.format(DST_DIR))
    logger.info('max graph size = {}'.format(MAX_GRAPH_SIZE))
    logger.info('graphs per size = {}'.format(GRAPHS_PER_SIZE))

    # Set working directory
    if getcwd().split('/')[-1] == 'scripts':
        chdir('../')

    logger.info('cwd = {}'.format(getcwd()))

    # Create destination directory
    mkdir(DST_DIR)

    graph_id = 1

    for size in range(MIN_GRAPH_SIZE, MAX_GRAPH_SIZE + 1, STEP):
        subdir = path_join(DST_DIR, 'n_{}'.format(size))

        # Create destination subdirectory
        mkdir(subdir)

        # Generate and store graphs
        for _ in range(GRAPHS_PER_SIZE):
            graph = get_graph(size)
            path = path_join(subdir, 'graph_{}.json'.format(graph_id))
            save_graph(graph, path)
            graph_id += 1
