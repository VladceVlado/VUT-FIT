/**
 * @file radiocolor.hpp
 * @brief Base class for radio coloring algorithms header.
 * @author Patrik Goldschmidt, <xgolds00@stud.fit.vutbr.cz>
 * @date 12-07-2020, last rev. 12-15-2020
 */

#ifndef RADIOCOLOR_HPP_DEFINED
#define RADIOCOLOR_HPP_DEFINED

#include "matrix.hpp"
#include <vector>

/**
 * @brief Representation of the colored graph. Allows to assign a color or
 *        radio color for each vertex by writing to its corresponding index.
 */
using graphColors = std::vector<unsigned>;

/**
 * @brief Base class for RadioColoring algorithm, providing a basic interface
 *        to operate its derived instances via methods to process the graph
 *        according to the given algorithm.
 */
class RadioColor {
  public:
    virtual ~RadioColor();

    /**
     * @brief Processes the input graph by radio color algorithm.
     *
     * @param[in] graph Graph in the desired representation.
     * @param[in] k     Radio K-coloring number.
     *
     * @return Radio colored graph.
     */
    virtual graphColors process(const Matrix &graph, int k) = 0;

  private:
};

#endif /* RADIOCOLOR_HPP_DEFINED */
