#!/usr/bin/env python3

"""
VUT FIT MITAI
GAL 2020/2021
Project: Topic 38 - Graph Radio Coloring Parallelization
Authors: Vladimir Dusek, Patrik Goldschmidt

Module description:
    Python prototype of sequential coloring
"""

from logging import basicConfig, getLogger
from math import ceil as round_up
from math import floor as round_down

LOGGING_FORMAT = '%(levelname)s | %(funcName)s | %(lineno)s | %(message)s'  # noqa: WPS323
basicConfig(format=LOGGING_FORMAT, level='INFO')
logger = getLogger(__name__)

INF = float('inf')
NEG_INF = (-1) * INF
EIGHTY = 80


def radio_k_coloring(adj_mat: list, k: int) -> list:
    """
    Algorithm for k-radio coloring of unoriented graph.

    Args:
        adj_mat (list): the adjacency matrix of graph
        k (int): positive integer k

    Returns:
        list: labels
    """
    logger.info(EIGHTY * '-')
    logger.info('Radio k-Coloring Algorithm')

    logger.info('Initialization')
    logger.info('   adj_mat = {}'.format(adj_mat))
    logger.info('   k = {}'.format(k))

    # Get number of vertices
    n = len(adj_mat)
    logger.info('   n = {}'.format(n))

    # Init
    c = [[INF for _ in range(n)] for _ in range(n)]
    logger.info('   c = {}'.format(c))

    # Get distance matrix
    logger.info('Step 1) Get distance matrix using Floyd-Warshall')
    dist_mat = get_dist_mat(adj_mat)
    logger.info('   dist_mat = {}'.format(dist_mat))

    logger.info('Step 2) Compute auxiliary matrix C')

    # Part 1
    for i in range(n):
        for j in range(n):
            if k + 1 >= dist_mat[i][j]:
                c[i][j] = k + 1 - dist_mat[i][j]
            else:
                c[i][j] = 0
            c[j][i] = c[i][j]
        c[i][i] = INF

    logger.info('   c = {}'.format(c))

    logger.info('Step 3) Compute labels')
    r = 0
    labels = [0 if i == 0 else INF for i in range(n)]
    logger.info('   init labels = {}'.format(labels))
    logger.info('   r = {}'.format(r))
    logger.info('   vertex {} has label {} (init)'.format(r, labels[0]))

    # Part 2
    for _ in range(n - 1):
        min_label = INF
        p = None

        for j in range(n):
            # There's a bug in the paper on the following line
            if min_label > c[r][j]:
                min_label = c[r][j]
                p = j

        for j in range(n):
            c[p][j] += min_label

        for j in range(n):
            if c[p][j] < c[r][j]:
                c[p][j] = c[r][j]

        logger.info('   c = {}'.format(c))

        labels[p] = min_label
        logger.info('   vertex {} has label {}'.format(p, min_label))
        r = p

    logger.info('Result')
    logger.info('   labels = {}'.format(labels))
    return labels


# It seems like there's a bug in this algorithm as well. However, it's not necessary
# for the k-radio coloring so I'm not gonna dive into it.
def lower_bound_of_rc(adj_mat: list, k: int) -> int:
    """
    Get rc which is the maximum integer assigned by algorithm to some vertex of graph.

    Args:
        adj_mat (list): the adjacency matrix of graph
        k (int): positive integer k

    Raises:
        Exception: when unreachable vertex

    Returns:
        int: lower bound of rc_k(G)
    """
    logger.info(EIGHTY * '-')
    logger.info('Algorithm to Find a Lower Bound of rc')
    logger.info('adj_mat = {}'.format(adj_mat))
    logger.info('k = {}'.format(k))

    # Get number of vertices
    n = len(adj_mat)
    logger.info('n = {}'.format(n))

    # Get distance matrix
    dist_mat = get_dist_mat(adj_mat)
    logger.info('dist_mat = {}'.format(dist_mat))

    # This algorithm cannot work with Graphs with unreachable vertexes
    if INF in (item for lst in dist_mat for item in lst):
        raise Exception("There's unreachable vertex in the Graph")

    # Init rc
    rc = NEG_INF
    logger.info('rc init = {}'.format(rc))

    # Compute rc
    for l in range(n):
        for i in range(n):
            for j in range(n):
                s = dist_mat[l][j] + adj_mat[i][j] + adj_mat[j][i]
                if rc <= s and i not in {l, j} and j != l:
                    rc = s
    logger.info('rc pre = {}'.format(rc))

    # Compute the graph diameter
    diameter = get_diameter(dist_mat)
    logger.info('diameter = {}'.format(diameter))

    # Round rc
    if k == diameter and n % 2 == 0:
        rc = int(round_up((3 * (k + 1) - rc) / 2) * ((n - 2) / 2) + 1)
    else:
        rc = int(round_up((3 * (k + 1) - rc) / 2) * round_down((n - 2) / 2))

    logger.info('rc final = {}'.format(rc))
    return rc


def get_diameter(dist_mat: list) -> int:
    """
    Get diameter from distance matrix of graph.

    Args:
        dist_mat (list): distance matrix of graph

    Returns:
        int: diameter
    """
    return max(NEG_INF if item == INF else item for lst in dist_mat for item in lst)


def get_dist_mat(adj_mat: list) -> list:
    """
    Using Floyed-Warshall's algorithm to compute the distance matrix of graph.

    Args:
        adj_mat (list): the adjacency matrix of graph

    Returns:
        list: the distance matrix of graph
    """
    # Get number of vertices
    n = len(adj_mat)

    # Initiate distance matrix
    dist_mat = list(map(lambda i: list(map(lambda j: INF if j == 0 else j, i)), adj_mat))

    logger.debug('dist_mat (init) = {}'.format(dist_mat))

    # Compute distance matrix
    for i in range(n):
        for j in range(n):
            for k in range(n):
                dist_mat[j][k] = min(dist_mat[j][k], dist_mat[j][i] + dist_mat[i][k])
        dist_mat[i][i] = 0

    return dist_mat


if __name__ == '__main__':
    k = 4
    adj_mat = [
        [0, 1, 0, 0],
        [1, 0, 1, 1],
        [0, 1, 0, 0],
        [0, 1, 0, 0],
    ]

    # lower_bound_of_rc(adj_mat, k)
    radio_k_coloring(adj_mat, k)

    k = 4
    adj_mat = [
        [0, 1, 0, 1, 0, 0],
        [1, 0, 0, 1, 1, 0],
        [0, 0, 0, 0, 1, 1],
        [1, 1, 0, 0, 1, 0],
        [0, 1, 1, 1, 0, 0],
        [0, 0, 1, 0, 0, 0],
    ]

    # lower_bound_of_rc(adj_mat, k)
    radio_k_coloring(adj_mat, k)

    # Graph from the paper's example
    k = 4
    adj_mat = [
        [0, 1, 0, 1, 0, 1, 0, 0],
        [1, 0, 1, 1, 0, 0, 0, 0],
        [0, 1, 0, 0, 0, 0, 0, 0],
        [1, 1, 0, 0, 1, 0, 0, 0],
        [0, 0, 0, 1, 0, 0, 0, 0],
        [1, 0, 0, 0, 0, 0, 1, 1],
        [0, 0, 0, 0, 0, 1, 0, 0],
        [0, 0, 0, 0, 0, 1, 0, 0],
    ]

    # lower_bound_of_rc(adj_mat, k)
    radio_k_coloring(adj_mat, k)

    # My demonstration adjacency matrix
    k = 2
    adj_mat = [
        [0, 1, 1, 0],
        [1, 0, 1, 0],
        [1, 1, 0, 1],
        [0, 0, 1, 0],
    ]

    radio_k_coloring(adj_mat, k)
