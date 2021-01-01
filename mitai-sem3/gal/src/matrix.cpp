/**
 * @file matrix.cpp
 * @brief Matrix internal reprentation as a vector of integers offering
 *        2D access.
 * @author Patrik Goldschmidt, <xgolds00@stud.fit.vutbr.cz>
 * @date 12-10-2020, last rev. 12-15-2020
 */

#include "matrix.hpp"
#include <cassert>
#include <string>
#include <vector>

Matrix::Matrix() : rowsCnt(0), colsCnt(0), data() {}

Matrix::Matrix(size_t rows, size_t columns) : rowsCnt(rows), colsCnt(columns), data(rows * columns, 0) {}

Matrix::Matrix(size_t rows, size_t columns, int value) : rowsCnt(rows), colsCnt(columns), data(rows * columns, value) {}

size_t Matrix::cols() const { return this->colsCnt; }

size_t Matrix::rows() const { return this->rowsCnt; }

int Matrix::get(size_t x, size_t y) const {
    assert(x < this->rowsCnt && y < this->colsCnt);

    return this->data.at(x * this->colsCnt + y);
}

void Matrix::set(size_t x, size_t y, int val) {
    assert(x < this->rowsCnt && y < this->colsCnt);

    this->data[x * this->colsCnt + y] = val;
}

std::string Matrix::dumpMatrix() const {
    std::string output; /* Output string. */

    for (size_t rowIdx = 0; rowIdx < this->rowsCnt; rowIdx++) {
        for (size_t colIdx = 0; colIdx < this->colsCnt; colIdx++) {
            if (colIdx != 0) {
                output += " ";
            }

            output += std::to_string(this->data[rowIdx * this->colsCnt + colIdx]);
        }

        output += "\n";
    }

    return output;
}
