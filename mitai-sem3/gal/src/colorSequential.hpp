/**
 * @file colorSequential.hpp
 * @brief Sequential radio coloring algorithm header.
 * @author Vladimir Dusek, <xdusek27@stud.fit.vutbr.cz>
 * @date 12-07-2020, last rev. 12-17-2020
 */

#ifndef COLORSEQUENTIAL_HPP_INCLUDED
#define COLORSEQUENTIAL_HPP_INCLUDED

#include "radiocolor.hpp"

/**
 * @brief Sequential variant of radio coloring algorithm.
 *        Implementation based on: <INSERT LINK>
 */
class ColorSequential : public RadioColor {
  public:
    ~ColorSequential();

    graphColors process(const Matrix &graph, int k);

  private:
};

#endif /* COLORSEQUENTIAL_HPP_INCLUDED */
