/**
 * @file matrix.hpp
 * @brief Matrix internal reprentation as a vector of integers offering
 *        2D access.
 * @author Patrik Goldschmidt, <xgolds00@stud.fit.vutbr.cz>
 * @date 12-10-2020, last rev. 12-15-2020
 */

#ifndef MATRIX_HPP_INCLUDED
#define MATRIX_HPP_INCLUDED

#include <string>
#include <vector>

/** @brief Graph representation as adjacency matrix. */
class Matrix {
  public:
    /** @brief Constructs an empty Matrix object. */
    Matrix();

    /**
     * @brief Constructs an Adjacency Matrix object of a given size.
     *
     * @param[in] rows    Number of matrix rows.
     * @param[in] columns Number of matrix columns.
     */
    Matrix(size_t rows, size_t columns);

    /**
     * @brief Constructs an Adjacency Matrix object of a given size and
     *        initializes it with value.
     *
     * @param[in] rows    Number of matrix rows.
     * @param[in] columns Number of matrix columns.
     * @param[in] value   Initialization value.
     */
    Matrix(size_t rows, size_t columns, int value);

    /**
     * @brief Returns the number of matrix columns.
     *
     * @return Number of matrix columns.
     */
    size_t cols() const;

    /**
     * @brief Returns the number of matrix rows.
     *
     * @return Number of matrix rows.
     */
    size_t rows() const;

    /**
     * @brief Returns the value on the given index [x,y].
     *
     * @param[in] x Matrix row.
     * @param[in] y Matrix column.
     *
     * @return value on the
     */
    int get(size_t x, size_t y) const;

    /**
     * @brief Writes a value on the index [x,y].
     *
     * @param[in] x Matrix row.
     * @param[in] y Matrix column.
     * @param[in] val Value to write on the given index.
     */
    void set(size_t x, size_t y, int val);

    /**
     * @brief Dumps Matrix in human-readable format to string.
     *
     * @return String representing the matrix object.
     */
    std::string dumpMatrix() const;

  private:
    size_t rowsCnt;        /**< Number of matrix rows.    */
    size_t colsCnt;        /**< Number of matrix columns. */
    std::vector<int> data; /**< Adjacency matrix as 1D bool vector. */
};

#endif /* MATRIX_HPP_INCLUDED */
