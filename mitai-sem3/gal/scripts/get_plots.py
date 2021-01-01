#!/usr/bin/env python3

"""
VUT FIT MITAI
GAL 2020/2021
Project: Topic 38 - Graph Radio Coloring Parallelization
Authors: Vladimir Dusek, Patrik Goldschmidt

Module description:
    Process the measurement and generate plots

Raises:
    FileExistsError: if plots are already generated
"""

from contextlib import suppress
from logging import basicConfig, getLogger
from os import chdir, getcwd, listdir, mkdir
from os.path import join as path_join
from statistics import median

from matplotlib import pyplot as plt

# From where data measurements should be read
SRC_DIR = 'data_times/'

# Where plots should be stored
DST_DIR = 'plots/'

# Logging
LOGGING_FORMAT = '%(levelname)s | %(funcName)s | %(lineno)s | %(message)s'  # noqa: WPS323
basicConfig(format=LOGGING_FORMAT, level='INFO')
logger = getLogger(__name__)

# Will be used for title in plots
get_title = {
    's': 'sequential',
    'p1': 'parallel (1 processor)',
    'p2': 'parallel (2 processor)',
    'p4': 'parallel (4 processor)',
    'p8': 'parallel (8 processor)',
    'p16': 'parallel (16 processor)',
}


def process(directory: str) -> int:
    """
    Read the all files in the directory and compute the average value from them

    Args:
        directory (str): path to the directory

    Returns:
        int: average value
    """
    logger.debug('directory = {}'.format(directory))

    numbers = []

    for file_name in listdir(directory):
        file_path = path_join(directory, file_name)

        with open(file_path, 'r') as fp:
            timeVal = fp.read().strip()

            with suppress(ValueError):
                numbers.append(float(timeVal))

    return median(numbers)


def create_plot(variant: str, results: list, file_name: str) -> None:
    """
    Create a specific plot and save it to a file.

    Args:
        variant (str): variant (s | p1 | ...)
        results (list): list of tuples with results
        file_name (str): name of the plot file
    """
    plt.title(get_title[variant])
    xs = [n for n, _ in results]
    ys = [time for _, time in results]
    plt.plot(xs, ys)
    plt.xlabel('number of vertices')
    plt.ylabel('time [ms]')
    plt.savefig(file_name)
    plt.clf()


if __name__ == '__main__':

    logger.info('destination directory = {}'.format(DST_DIR))
    logger.info('source directory = {}'.format(SRC_DIR))

    # Set working directory
    if getcwd().split('/')[-1] == 'scripts':
        chdir('../')

    logger.info('cwd = {}'.format(getcwd()))

    # Create destination directory
    mkdir(DST_DIR)

    results = {}

    # For every source subdirectory
    for variant in listdir(SRC_DIR):
        subdir_path = path_join(SRC_DIR, variant)
        times = []

        # For every source subsubdirectory
        for subsubdir in listdir(subdir_path):
            subsubdir_path = path_join(subdir_path, subsubdir)

            # Get the average time
            avg_time = process(subsubdir_path)
            n = int(subsubdir.replace('n_', ''))
            times.append((n, avg_time))

        results[variant] = times

    logger.info('aggregated results = {}'.format(results))

    # This is gonna be dumped into a file
    dump_results = {}

    # Get plot for every variant
    for variant in results.keys():

        # Sort result according to n
        sorted_result = sorted(results[variant], key=lambda tpl: tpl[0])
        logger.debug(sorted_result)

        # Create plot
        plot_file_name = path_join(DST_DIR, 'plot-{}.png'.format(variant))
        logger.info('plot file_name = {}'.format(plot_file_name))
        create_plot(variant, sorted_result, plot_file_name)

        # Remove tuple structure for dump format
        dump_results[variant] = (lambda lst: [tpl[1] for tpl in lst])(sorted_result)

    # Dump results to a file
    logger.info('dump results = {}'.format(dump_results))
    result_file_name = path_join(DST_DIR, 'results.txt')
    logger.info('result file_name = {}'.format(result_file_name))
    with open(result_file_name, 'w') as fp:
        fp.write(str(dump_results))
