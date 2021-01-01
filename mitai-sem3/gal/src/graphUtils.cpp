/**
 * @file graphutils.cpp
 * @brief Graph utilities.
 * @author Patrik Goldschmidt, <xgolds00@stud.fit.vutbr.cz>
 * @date 12-09-2020, last rev. 12-15-2020
 */

#include "graphUtils.hpp"
#include "matrix.hpp"
#include <algorithm>

/**
 * @brief Practical infinity used by the module, because int MAX / MIN from
 *        <limits> would cause overflows and thus incorrect computations.
 */
constexpr int INF = 2 << 28;

int GraphUtils::computeDiameter(const Matrix &graph) {
    /* Retrieve distance matrix for the input graph. */
    Matrix distanceMat = GraphUtils::computeDistanceMatrix(graph);
    int diam = -INF; /* Graph diameter. */

    /* Iterate through the distance matrix and find the maximum element. */
    for (size_t rowIdx = 0; rowIdx < distanceMat.rows(); rowIdx++) {
        for (size_t colIdx = 0; colIdx < distanceMat.cols(); colIdx++) {
            diam = std::max(diam, distanceMat.get(rowIdx, colIdx));
        }
    }

    return diam;
}

Matrix GraphUtils::computeDistanceMatrix(const Matrix &graph) {
    /* Initialize distance matrix to "infinity". */
    Matrix dist = Matrix(graph.rows(), graph.cols(), INF);

    /* The length is 1 if the neighbor relationship exists in the graph. */
    for (size_t rowIdx = 0; rowIdx < graph.rows(); rowIdx++) {
        for (size_t colIdx = 0; colIdx < graph.cols(); colIdx++) {
            if (graph.get(rowIdx, colIdx) == 1) {
                dist.set(rowIdx, colIdx, 1);
            }
        }

        /* The length is 0 for the distance from the vertice to itself. */
        dist.set(rowIdx, rowIdx, 0);
    }

    /* The Floyd-Warshall algorithm itself. */
    for (size_t k = 0; k < dist.rows(); k++) {
        for (size_t i = 0; i < dist.rows(); i++) {
            for (size_t j = 0; j < dist.rows(); j++) {
                int current = dist.get(i, j);
                int throughK = dist.get(i, k) + dist.get(k, j);

                if (current > throughK) {
                    dist.set(i, j, throughK);
                }
            }
        }
    }

    return dist;
}
