/**
 * @file io.hpp
 * @brief Input graph manipulation header.
 * @author Patrik Goldschmidt, <xgolds00@stud.fit.vutbr.cz>
 * @date 12-07-2020, last rev. 12-15-2020
 */

#ifndef IO_HPP_INCLUDED
#define IO_HPP_INCLUDED

#include "matrix.hpp"
#include <string>

namespace IO {
/**
 * @brief Loads the graph representation from the file and checks for its
 *        validity.
 *
 * @param[in] filePath Path to the input file.
 *
 * @throw ios_base::failure  The file for could not be opened for reading.
 * @throw std::runtime_error The structure of the file is incorrect.
 */
Matrix load_graph(std::string filePath);

/**
 * @brief Writes given content to the file specified by filePath.
 *        It the file already exists, its contents will be replaced.
 *
 * @param[in] content  What to write to the file.
 * @param[in] filePath File to write data to.
 *
 * @throw ios_base::failure The file for could not be opened for writing.
 */
void writeFile(std::string content, std::string filePath);
} // namespace IO

#endif /* IO_HPP_INCLUDED */
