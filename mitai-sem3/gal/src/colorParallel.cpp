/**
 * @file colorParallel.cpp
 * @brief Parallel radio coloring algorithm source.
 * @author Patrik Goldschmidt, <xgolds00@stud.fit.vutbr.cz>
 * @date 12-07-2020, last rev. 12-15-2020
 */

#include "colorParallel.hpp"
#include <algorithm>
#include <iterator>
#include <limits>
#include <numeric>
#include <omp.h>
#include <vector>

#include <iostream>

/* ------------------------------------------------------------------------- *
 * --------------------------   MACROS & DEFINES   ------------------------- *
 * ------------------------------------------------------------------------- */

#ifdef PARALLEL_OPTIMIZED

/** @brief Minimum MergeSort array size to perform parallel execution. */
#define MERGESORT_PARALLEL_MIN 32

#else

#define MERGESORT_PARALLEL_MIN 0

#endif /* PARALLEL_OPTIMIZED */

/* ------------------------------------------------------------------------- *
 * --------------------------   PRIVATE SECTION   -------------------------- *
 * ------------------------------------------------------------------------- */

/**
 * @brief Custom MergeSort merge procedure according to Wiki pseudocode:
 *    https://en.wikipedia.org/wiki/Merge_algorithm#Merging_two_lists
 *
 * @param[in] indices Vector of indices.
 * @param[in] begin   Index of the vector subsequence beginning.
 * @param[in] mid     Index of the vector subsequence middle.
 * @param[in] right   Index of the vector subsequence end.
 * @param[in] values  Value vector corresponding to the iterators.
 */
void merge(std::vector<unsigned> &indices, size_t left, size_t mid, size_t right, const std::vector<unsigned> values) {
    size_t sub1Idx = left;                          /* Subsequence 1 index. */
    size_t sub2Idx = mid + 1;                       /* Subsequence 2 index. */
    size_t mergedIdx = 0;                           /* Merged sequence index. */
    std::vector<unsigned> merged(right - left + 1); /* Merged sequence. */

    /* Merge sorted subseqs into a sorted sequence with sum of their lengths. */
    while (sub1Idx <= mid && sub2Idx <= right) {
        /* Both subseqs are not at the end -> index of the higher to start. */
        if (values[indices[sub1Idx]] > values[indices[sub2Idx]]) {
            merged[mergedIdx] = indices[sub1Idx];
            sub1Idx++;
        } else {
            merged[mergedIdx] = indices[sub2Idx];
            sub2Idx++;
        }

        mergedIdx++;
    }

    /* Either subseq 1 or subseq 2 is empty. Append the rest to the output. */
    if (sub1Idx <= mid) {
        /* Copy subsequence 1 contents. */
        std::copy(indices.begin() + sub1Idx, indices.begin() + mid + 1, merged.begin() + mergedIdx);
    } else {
        /* Copy subsequence 2 contents. */
        std::copy(indices.begin() + sub2Idx, indices.begin() + right + 1, merged.begin() + mergedIdx);
    }

    /* Replace the original input with the merged sequence. */
    std::copy(merged.begin(), merged.end(), indices.begin() + left);
}

/**
 * @brief Parallel merge sort algorithm, as suggested by CVUT Course B4M35PAG
 *        Parallel algorithms:
 * cw.fel.cvut.cz/old/_media/courses/b4m35pag/lab6_slides_advanced_openmp.pdf
 *
 * @param[in] input vector to sort.
 * @param[in] left Index of the subarray start.
 * @param[in] left Index of the subarray end.
 */
void mergeSortRecursive(std::vector<unsigned> &indices, const std::vector<unsigned> &data, size_t left, size_t right) {
    /* Regular MergeSort, call our variant only for 32 and more elems. */
    if (left < right) {
        if (right - left >= MERGESORT_PARALLEL_MIN) {
            size_t mid = (left + right) / 2;

/* Spawn task for each MergeSort recursive call. */
#pragma omp taskgroup
            {
                /*
                 * untied - Do not bind the task to a particular CPU core.
                 * if - create tasks only if the amount of work is sufficient.
                 */
#ifdef PARALLEL_OPTIMIZED
#pragma omp task shared(data, indices) untied if (right - left >= (1 << 12))
#else
#pragma omp task shared(data, indices)
#endif
                mergeSortRecursive(indices, data, left, mid);

#ifdef PARALLEL_OPTIMIZED
#pragma omp task shared(data, indices) untied if (right - left >= (1 << 12))
#else
#pragma omp task shared(data, indices)
#endif
                mergeSortRecursive(indices, data, mid + 1, right);

#pragma omp taskyield
            }

            /* Perform the merge with the results. */
            merge(indices, left, mid, right, data);
        } else {
            /* Sequence is too short for parallel sorting - sort sequentially. */
            std::sort(indices.begin() + left, indices.begin() + right + 1,
                      [&data](const unsigned &a, const unsigned &b) noexcept { return data[a] > data[b]; });
        }
    }
}

/**
 * @brief Sorts a vector of values and returns their respective indexes.
 *
 * @param[in]  values  Values to be sorted.
 * @param[out] indices Index vector after sorting.
 */
void sortIndicesDesc(const std::vector<unsigned> &values, std::vector<unsigned> &indices) {
    /* Populate the vector with indice numbers. */
    std::iota(std::begin(indices), std::end(indices), 0);

/* Feed the vectors into merge sort procedure. */
#pragma omp parallel
#pragma omp single
    mergeSortRecursive(indices, values, 0, values.size() - 1);
}

/* ------------------------------------------------------------------------- *
 * --------------------------   PUBLIC SECTION   --------------------------- *
 * ------------------------------------------------------------------------- */

ColorParallel::~ColorParallel() {}

ColorParallel::ColorParallel(unsigned threads) {
    /* Set number of threads to use for parallel processing is specified. */
    if (threads != 0) {
        omp_set_num_threads(threads);
    }
}

graphColors ColorParallel::process(const Matrix &graph, [[maybe_unused]] int k) {
    size_t vertices = graph.rows();             /* Graph vertices number. */
    size_t colorsMax = 2 * vertices - 1;        /* Maximum color number. */
    std::vector<unsigned> degrees(vertices, 0); /* List of vert. degrees. */
    std::vector<unsigned> sorted(vertices);     /* List of sorted vertices. */
    graphColors colors(vertices, 0);            /* Vertices colors. */
    Matrix dist2(vertices, vertices);           /* Distance 2 vert. matrix. */
    Matrix forbidden(vertices, colorsMax);      /* Forbidden colors matrix. */

    /* Step 1: Compute vertices degree and sort them accordingly. */
    /* Compute vertices degree. */
#ifdef PARALLEL_OPTIMIZED
#pragma omp parallel for if (vertices > 64)
#else
#pragma omp parallel for
#endif
    for (size_t rowIdx = 0; rowIdx < vertices; rowIdx++) {
        unsigned neighbors = 0; /* Number of vertex neighbors. */

        for (size_t colIdx = 0; colIdx < vertices; colIdx++) {
            neighbors += graph.get(rowIdx, colIdx);
        }

        degrees[rowIdx] = neighbors;
    }

    /* Sort vertex indices in descending order according to their degree. */
    sortIndicesDesc(degrees, sorted);

    /* Step 2: Calculate the distance-2 matrix. */
#ifdef PARALLEL_OPTIMIZED
#pragma omp parallel for if (vertices > 16)
#else
#pragma omp parallel for
#endif
    for (size_t i = 0; i < vertices; i++) {
        for (size_t j = 0; j < vertices; j++) {
            if (graph.get(i, j) == 1) {
                for (size_t m = 0; m < vertices; m++) {
                    if (graph.get(j, m) == 1 && m != i) {
                        dist2.set(i, m, 1);
                    }
                }
            }
        }
    }

#ifdef DEBUG
    std::cout << "DEGREES: ";
    for (auto deg : degrees) {
        std::cout << deg << " ";
    }
    std::cout << std::endl;

    std::cout << "INPUT: " << std::endl;
    std::cout << graph.dumpMatrix() << std::endl;
#endif

    /* Step 3: Assign colors to vertices. */
    for (size_t j = 0; j < vertices; j++) {
        int minColor = std::numeric_limits<int>::max();
        bool minFound = false;

#ifdef PARALLEL_OPTIMIZED
#pragma omp parallel for reduction(min : minColor) schedule(static) firstprivate(minFound) if (vertices > 64)
#else
#pragma omp parallel for reduction(min : minColor) schedule(static) firstprivate(minFound)
#endif
        /* Find the color with the lowest index that is not forbidden. */
        for (size_t colorIdx = 0; colorIdx < colorsMax; colorIdx++) {
            /* OpenMP does not support loop break, thus this optimization hack. */
            if (minFound) {
                continue;
            }

            if (forbidden.get(sorted[j], colorIdx) == 0) {
                minColor = static_cast<int>(colorIdx);
                minFound = true;
            }
        }

#ifdef DEBUG
        std::cout << "Forbid for j=" << j << " | node: " << sorted[j] << std::endl;
        std::cout << "MinColor: " << minColor << std::endl;
        std::cout << forbidden.dumpMatrix() << std::endl;
#endif

        /* Assign a minimum color to the vertex regarding current iteration. */
        colors[sorted[j]] = minColor;

        /* Update forbidden matrix to prevent coloring conflicts. */
#ifdef PARALLEL_OPTIMIZED
#pragma omp parallel for if (vertices > 32)
#else
#pragma omp parallel for
#endif
        for (size_t i = 0; i < vertices; i++) {
            /* Forbid colors for directly connected nodes. */
            if (graph.get(sorted[j], i) == 1) {
                /* Forbid currently assigned color. */
                forbidden.set(i, minColor, 1);

                /* Forbid previous color if the assigned is not first. */
                if (minColor != 0) {
                    forbidden.set(i, minColor - 1, 1);
                }

                /* Forbid next color if the assigned is not last. */
                if (static_cast<size_t>(minColor) != 2 * vertices - 2) {
                    forbidden.set(i, minColor + 1, 1);
                }
            }

            /* Forbid currently assigned color for nodes at distance 2. */
            if (dist2.get(sorted[j], i) == 1) {
                forbidden.set(i, minColor, 1);
            }
        }
    }

    return colors;
}
