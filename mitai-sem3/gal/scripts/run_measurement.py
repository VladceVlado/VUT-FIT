#!/usr/bin/env python3

"""
VUT FIT MITAI
GAL 2020/2021
Project: Topic 38 - Graph Radio Coloring Parallelization
Authors: Vladimir Dusek, Patrik Goldschmidt

Module description:
    Run C++ binaries

Raises:
    FileExistsError: if data are already generated
"""

from logging import basicConfig, getLogger
from os import chdir, getcwd, listdir, mkdir, system
from os.path import join as path_join

# From where graphs should be read
SRC_DIR = 'data_graphs/'

# Where times should be stored
DST_DIR = 'data_times/'

# List of all the variants to be executed
VARIANTS = ['s', 'p1', 'p2', 'p4', 'p8', 'p16']

# Path to binary
BIN = './src/gal'

# Logging
LOGGING_FORMAT = '%(levelname)s | %(funcName)s | %(lineno)s | %(message)s'  # noqa: WPS323
basicConfig(format=LOGGING_FORMAT, level='INFO')
logger = getLogger(__name__)


def execute(variant: str, src_path: str, dst_path: str) -> None:
    """
    Execute the binary in the specified form.

    Args:
        variant (str): sequential / parallel + number of processors
        src_path (str): path to the source file (graph)
        dst_path (str): path to the destination file (time)
    """
    if variant[0] == 'p':
        threads = variant[1:]
        executable = '{} -m -k 2 -p -t {} -o {} {}'.format(BIN, threads, dst_path, src_path)
    else:
        executable = '{} -m -k 2 -s -o {} {}'.format(BIN, dst_path, src_path)

    logger.info('execution = {}'.format(executable))

    system(executable)  # noqa: S605


if __name__ == '__main__':

    logger.info('destination directory = {}'.format(DST_DIR))
    logger.info('source directory = {}'.format(SRC_DIR))
    logger.info('binary = {}'.format(BIN))

    # Set working directory
    if getcwd().split('/')[-1] == 'scripts':
        chdir('../')

    logger.info('cwd = {}'.format(getcwd()))

    # Create destination directory
    mkdir(DST_DIR)

    # Get all the variants of execution
    variants = VARIANTS
    logger.info('variants = {}'.format(variants))

    # Create destination subdirectories
    for variant in variants:
        mkdir(path_join(DST_DIR, variant))

    # For every source subdirectory
    for src_subdir in listdir(SRC_DIR):
        src_subdir_path = path_join(SRC_DIR, src_subdir)

        # Create destination subsubdirectories
        for variant in variants:
            mkdir(path_join(DST_DIR, variant, src_subdir))

        # For every file in source subdirectory
        for file_name in listdir(src_subdir_path):
            src_path = path_join(src_subdir_path, file_name)

            # For every variant execute
            for variant in variants:
                dst_path = path_join(DST_DIR, variant, src_subdir, file_name.replace('.json', ''))
                execute(variant, src_path, dst_path)
