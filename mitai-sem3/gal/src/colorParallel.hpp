/**
 * @file colorParallel.hpp
 * @brief Parallel radio coloring algorithm header.
 * @author Patrik Goldschmidt, <xgolds00@stud.fit.vutbr.cz>
 * @date 12-07-2020, last rev. 12-15-2020
 */

#ifndef COLORPARALLEL_HPP_INCLUDED
#define COLORPARALLEL_HPP_INCLUDED

#include "radiocolor.hpp"

/**
 * @brief Parallel variant of radio coloring algorithm.
 *        Implementation based on:
 * https://www.researchgate.net/publication/249869674
 */
class ColorParallel : public RadioColor {
  public:
    ~ColorParallel();

    /**
     * @brief Constructor for parallel radio coloring algorithm.
     *
     * @param[in] threads Number of computational threads to use.
     */
    ColorParallel(unsigned threads);

    graphColors process(const Matrix &graph, int k);

  private:
};

#endif /* COLORPARALLEL_HPP_INCLUDED */
