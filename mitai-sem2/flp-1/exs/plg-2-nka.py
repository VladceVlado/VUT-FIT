#!/usr/bin/env python3

import sys
import os


class Grammar:
    """
    PLG
    """
    def __init__(self, nonterms, terms, default_nonterm, rules):
        self.nonterms = nonterms
        self.terms = terms
        self.default_nonterm = default_nonterm
        self.rules = rules

    def check_syntax(self):
        # ToDo
        return True

    def transform(self):
        # ToDo
        return True

    def __str__(self):
        return 'nonterms: ' + str(self.nonterms) + '\n' + 'terms: ' + \
               str(self.terms) + '\n' + 'default_nonterm: ' + \
               str(self.default_nonterm) + '\n' + 'rules: ' + \
               str(self.rules)


class Automata:
    """
    RKA
    """
    def __init__(self, grammar):
        self.states = states
        self.alphabet = alphabet
        self.function = function
        self.default_state = default_state
        self.accept_states = accept_states

    def __str__(self):
        return 'states: ' + str(self.states) + '\n' + 'alphabet: ' + \
               str(self.alphabet) + '\n' + 'function: ' + \
               str(self.function) + '\n' + 'default_state: ' + \
               str(self.default_state) + '\n' + 'accept_states: ' + \
               str(self.accept_states)


def parse_file(file_name):
    file = open(file_name, "r")

    nonterms = list(file.readline().strip().split(','))
    terms = list(file.readline().strip().split(','))
    default_nonterm = file.readline().strip()
    rules = []

    for line in file:
        rules += line.strip().split(',')

    tmp = []
    for rule in rules:
        tmp.append(tuple(rule.split('->')))

    rules = tmp

    return Grammar(nonterms, terms, default_nonterm, rules)


def parse_stdin():
    # ToDo
    return 0


# ./plg-2-nka.py {-i|-1|-2} [file_name]
if __name__ == '__main__':

    args = sys.argv[1:]

    # not 1 or 2 args -> Exception
    if not len(args) in [1, 2]:
        raise Exception

    # 1st arg not in {-i|-1|-2} -> Exception
    if not args[0] in ['-i', '-1', '-2']:
        raise Exception

    # 2 args -> parse input file
    if len(args) == 2:
        grammar = parse_file(args[1])
    # just 1 arg -> parse stdin
    else:
        grammar = parse_stdin()

    # determine program option
    if args[0] == '-i':
        print(grammar)

    elif args[0] == '-1':
        grammar.transform()

    elif args[0] == '-2':
        ...
