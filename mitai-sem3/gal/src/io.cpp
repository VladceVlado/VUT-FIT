/**
 * @file io.cpp
 * @brief Input graph manipulation source.
 * @author Patrik Goldschmidt, <xgolds00@stud.fit.vutbr.cz>
 * @date 12-07-2020, last rev. 12-15-2020
 */

#include "io.hpp"
#include "matrix.hpp"
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

/**
 * @brief Redefinition of the previous regex size limit of 100000 because
 *        graphs it could cause stack overflows on bigger graphs.
 */
#define _GLIBCXX_REGEX_STATE_LIMIT 65536

/**
 * @brief Limits the length of the input which input can be processed in regex
 *        at once.
 */
constexpr size_t REGEX_MAX_SIZE = 50000;

#include <regex>

/* ------------------------------------------------------------------------- *
 * --------------------------   PRIVATE SECTION   -------------------------- *
 * ------------------------------------------------------------------------- */

/**
 * @brief Input string split splitting, because regex API in C++ can not handle
 *        big inputs at once. Big inputs that may cause the program to fail
 *        are created by graphs with more than 400 nodes.
 *
 * @param[in]  input   Input string to be split.
 * @param[in]  maxSize Specifies maximum size of the splitted string.
 * @param[out] output  Vector of splitted strings with the length of maxSize.
 */
void splitInput(const std::string input, size_t maxSize, std::vector<std::string> &output) {
    if (input.size() < maxSize) {
        /* Size of the string is fine - add it to output. */
        output.push_back(input);
    } else {
        /* String is too big - split it near half so the regexes will be fine. */
        size_t splitIdx = input.size() / 2; /* Index to perform split at. */

        for (; splitIdx > 0; splitIdx--) {
            /* Perform the split only on numbers -- safe option. */
            if (input[splitIdx] == '0' || input[splitIdx] == '1') {
                /* Split can be made - recursively call with it. */
                splitInput(input.substr(0, splitIdx), maxSize, output);
                splitInput(input.substr(splitIdx, std::string::npos), maxSize, output);

                break;
            }
        }

        if (splitIdx == 0) {
            /* Cycle iterated to the beginning - input format is bad. */
            throw std::runtime_error("Invalid input graph format.");
        }
    }
}

/* ------------------------------------------------------------------------- *
 * --------------------------   PUBLIC SECTION   --------------------------- *
 * ------------------------------------------------------------------------- */

Matrix IO::load_graph(std::string filePath) {
    size_t vertices;                   /* Number of vertices in the graph. */
    std::ifstream inFile;              /* Input file handle. */
    std::string rawGraph;              /* String loaded from the file. */
    std::string parsedGraph("");       /* Parsed form of the textual graph. */
    std::string validRegexStr;         /* Input graph validation regex string. */
    std::vector<std::string> splitted; /* Vector of splitted strings. */
    Matrix resGraph;                   /* Graph in the internal representation. */

    /* Open the file. */
    inFile.open(filePath, std::ifstream::in);

    if (inFile.fail()) {
        throw std::ios_base::failure("Input file opening error.");
    }

    /* Load graph representation from the file (1 graph per line). */
    std::getline(inFile, rawGraph);

    /* Perform the split so the regex does not handles very big data. */
    splitInput(rawGraph, REGEX_MAX_SIZE, splitted);

    /* Preprocess splitted strings and merge them back together. */
    for (std::string splitPart : splitted) {
        splitPart = std::regex_replace(splitPart, std::regex(" "), "");
        splitPart = std::regex_replace(splitPart, std::regex("\\[\\["), "");
        splitPart = std::regex_replace(splitPart, std::regex("\\]\\]"), "");
        splitPart = std::regex_replace(splitPart, std::regex("\\],\\["), " ");
        splitPart = std::regex_replace(splitPart, std::regex(","), "");

        /* Merge the string splits into one. */
        parsedGraph += splitPart;
    }

    /* Graph cannot be empty. */
    if (parsedGraph.size() == 0) {
        throw std::runtime_error("Invalid input graph format.");
    }

    /* Determine the number of vertices and allocate the graph. */
    vertices = parsedGraph.substr(0, parsedGraph.find(" ")).size();
    resGraph = Matrix(vertices, vertices);

    /* Validate the only "not too big" graphs -- regex cant 120+ vertices. */
    if (vertices < 25) {
        /*
         * Build up a following regex string string to validate input graph with.
         * Regex: "^(([01]){vertices} ){vertices-1}([01]{vertices})$"
         */
        validRegexStr += "^";                          /* Start of the line.  */
        validRegexStr += "(([01]){";                   /* Match 0 or 1.       */
        validRegexStr += std::to_string(vertices);     /* Vertices times.     */
        validRegexStr += "} ){";                       /* Space must follow.  */
        validRegexStr += std::to_string(vertices - 1); /* Vertices - 1 times. */
        validRegexStr += "}([01]{";                    /* Again, 0 or 1.      */
        validRegexStr += std::to_string(vertices);     /* Vertices times.     */
        validRegexStr += "})$";                        /* Terminate the line. */

        /* Validate the input line with the built regex. */
        if (!std::regex_match(parsedGraph, std::regex(validRegexStr))) {
            throw std::runtime_error("Invalid input graph format.");
        }
    }

    /* Initialize the matrix to return. */
    for (size_t verticeIdx = 0; verticeIdx < vertices; verticeIdx++) {
        /* Determine current substring (row) index and string itself. */
        size_t curSubstrIdx = verticeIdx * vertices + verticeIdx;
        std::string curVerticeNeighbors = parsedGraph.substr(curSubstrIdx, vertices);

        /* Iterate over the string and fill in the adjacency matrix. */
        for (size_t neighborIdx = 0; neighborIdx < vertices; neighborIdx++) {
            resGraph.set(verticeIdx, neighborIdx, (curVerticeNeighbors[neighborIdx] == '1') ? 1 : 0);
        }
    }

    inFile.close();

    return resGraph;
}

void IO::writeFile(std::string content, std::string filePath) {
    std::ofstream outFile;

    /* Open the file. */
    outFile.open(filePath, std::ofstream::out);

    outFile << content << std::endl;

    if (outFile.fail()) {
        throw std::ios_base::failure("Output file could not be opened.");
    }

    outFile.close();
}
