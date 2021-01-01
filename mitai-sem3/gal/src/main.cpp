/**
 * @file main.cpp
 * @brief Main file for GAL project @ FIT BUT.
 * @author Patrik Goldschmidt, <xgolds00@stud.fit.vutbr.cz>
 * @author Vladimir Dusek,     <xdusek27@stud.fit.vutbr.cz>
 * @date 12-06-2020, last rev. 12-15-2020
 */

#include "argparse.hpp"
#include "colorParallel.hpp"
#include "colorSequential.hpp"
#include "graphUtils.hpp"
#include "io.hpp"
#include "matrix.hpp"
#include "radiocolor.hpp"
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <vector>

/** @brief Simple macro (I know they are bad) to calculate chrono diff. */
#define CHRONO_CALC_DIFF_USEC(tstart, tend) (std::chrono::duration_cast<std::chrono::microseconds>((tend) - (tstart)))

/* Extract tick count from the usec chrono duration and returns msec value. */
#define CHRONO_DUR_EXTRACT_MSEC(duration) (static_cast<double>(duration.count()) / 1000.0)

/** @brief Program help message. */
constexpr char PROG_HELP_MSG[] = "GAL is a program used to perform graph coloring in either sequentially or\n\
in parallel and measure their performance on given input data.\n\
USAGE:\n\
   gal [-m|--measure] [-o|--output OUTFILE] [-t|--threads THREADS] \
([-s|--sequential] | [-p|--parallel]) GRAPH_FILE | -h|--help\n\
      GRAPH_FILE       - Adjacency matrix with the problem specification\n\
      -h|--help        - Display this help message\n\
      -k|--knumber     - K for radio k-coloring. Default graph diameter\n\
      -m|--measure     - Measure the time taken to perform the coloring\n\
      -o|--output      - Output file when -m is present. Default stdout\n\
      -p|--parallel    - Perform the coloring in parallel\n\
      -s|--sequential  - Perform the sequential coloring (default)\n\
      -t|--threads     - Number of available threads for parallel computation";

int main(int argc, char **argv) {
    unsigned radioKColor;                      /* Radio K-coloring value. */
    Matrix inputGraph;                         /* Input graph.         */
    ArgParse argParser = ArgParse(argc, argv); /* Argument parsing.    */
    ArgParse::args programArgs;                /* Program arguments.   */
    RadioColor *algorithm = nullptr;           /* Radio coloring algorithm. */
    graphColors coloring;                      /* Radio coloring resuls. */
    std::chrono::time_point<std::chrono::high_resolution_clock> time_start;
    std::chrono::time_point<std::chrono::high_resolution_clock> time_end;

    /* Process command line arguments and prepare the program accordingly. */
    try {
        ArgParse::RunMode progMode = argParser.args_process(programArgs);

        switch (progMode) {
        case ArgParse::RunMode::MODE_HELP:
            std::cout << PROG_HELP_MSG << std::endl;

            return EXIT_SUCCESS;

        case ArgParse::RunMode::MODE_NONE:
            /* When no mode is specified, solve for sequential part by default. */
            [[fallthrough]]

            case ArgParse::RunMode::MODE_SEQUENTIAL :
                /* Sequential algorithm variant will be run. */
                algorithm = new ColorSequential();

            break;

        case ArgParse::RunMode::MODE_PARALLEL:
            /* Parallel algorithm variant will be run. */
            algorithm = new ColorParallel(programArgs.threads);

            break;
        }
    } catch (const std::exception &ex) {
        /* Exception during the argument processing was caught. */
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    try {
        /* Load the data representation from the file. */
        inputGraph = IO::load_graph(std::string(programArgs.inFile));
    } catch (const std::exception &ex) {
        /* Exception during the file reading. */
        delete algorithm;
        std::cerr << ex.what() << std::endl;

        return EXIT_FAILURE;
    }

    /* Find Radio K-coloring value. Set if specified, otherwise calculate. */
    if (programArgs.knum == 0) {
        radioKColor = GraphUtils::computeDiameter(inputGraph);
    } else {
        radioKColor = programArgs.knum;
    }

    /* Start the time measurement if desired. */
    if (programArgs.measure) {
        time_start = std::chrono::high_resolution_clock::now();
    }

    /* Process the graph with the desired algorithm. */
    coloring = algorithm->process(inputGraph, radioKColor);

    /* End the time measurement if desired. */
    if (programArgs.measure) {
        time_end = std::chrono::high_resolution_clock::now();
    }

    /* Print the algorithm results. */
    for (size_t idx = 0; idx < coloring.size(); idx++) {
        if (idx != 0) {
            std::cout << " ";
        }

        std::cout << coloring[idx];
    }

    std::cout << "\n";

    /* Print the run duration if desired. */
    if (programArgs.measure) {
        std::chrono::microseconds duration = CHRONO_CALC_DIFF_USEC(time_start, time_end);

        if (programArgs.outFile == nullptr) {
            /* No output file was specified, dump to stdout. */
            std::cout << CHRONO_DUR_EXTRACT_MSEC(duration) << std::endl;
        } else {
            /* Output file was specified, write to it. */
            try {
                IO::writeFile(std::to_string(CHRONO_DUR_EXTRACT_MSEC(duration)), programArgs.outFile);
            } catch (const std::exception &ex) {
                std::cerr << "The file could not be written." << std::endl;
            }
        }
    }

    /* Free the allocated memory. */
    delete algorithm;

    return EXIT_SUCCESS;
}
