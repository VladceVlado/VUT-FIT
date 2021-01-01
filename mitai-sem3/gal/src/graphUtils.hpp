/**
 * @file graphutils.hpp
 * @brief Graph utilities.
 * @author Patrik Goldschmidt, <xgolds00@stud.fit.vutbr.cz>
 * @date 12-09-2020, last rev. 12-15-2020
 */

#ifndef GRAPHUTILS_INCLUDED
#define GRAPHUTILS_INCLUDED

#include "matrix.hpp"

namespace GraphUtils {
/**
 * @brief Computes graph of the input graph in the form of adjacency matrix.
 *
 * @param[in] graph Adjacency matrix graph representation.
 *
 * @return Diameter of the input graph.
 */
int computeDiameter(const Matrix &graph);

/**
 * @brief Computes disatnce matrix for the input graph using Floyd-Warshall
 *        algorithm.
 *
 * @param[in] graph Graph definition as adjacency matrix.
 *
 * @return Distance matrix with the same dimensions as the input.
 */
Matrix computeDistanceMatrix(const Matrix &graph);
} // namespace GraphUtils

#endif /* GRAPHUTILS_INCLUDED */
