/**
 * @file argparse.cpp
 * @brief Argument parsing module.
 * @author Patrik Goldschmidt, <xgolds00@stud.fit.vutbr.cz>
 * @date 12-07-2020, last rev. 12-15-2020
 */

#include "argparse.hpp"
#include <cstdlib>
#include <getopt.h>
#include <stdexcept>

/* ------------------------------------------------------------------------- *
 * --------------------------   PRIVATE SECTION   -------------------------- *
 * ------------------------------------------------------------------------- */

/**
 * @brief Converts string to an unsigned int and performs input validation.
 *
 * @param[str] String containing the number.
 *
 * @return Number contained in the string.
 * @throws std::runtime_error if the input string is not a number.
 */
unsigned cstr2uint(const char *str) {
    /* Validate if the argument is a number. */
    char *errPtr; /* Error pointer during conversion. */
    unsigned long num = std::strtoul(str, &errPtr, 10);

    /* ErrPtr should end at the string end on successful conversion. */
    if (*errPtr != '\0') {
        throw std::runtime_error("Expected numeric argument is not a number.");
    }

    return static_cast<unsigned>(num);
}

/* ------------------------------------------------------------------------- *
 * --------------------------   PUBLIC SECTION   --------------------------- *
 * ------------------------------------------------------------------------- */

ArgParse::ArgParse(int argc, char **argv) noexcept : argCnt(argc), argArr(argv) {}

ArgParse::RunMode ArgParse::args_process(ArgParse::args &parsedArgs) {
    int c;                                /* Getopt() returned character. */
    std::string errMsg = this->argArr[0]; /* Program error message. */
    ArgParse::args arguments = {};        /* Processed arguments structure. */
    ArgParse::RunMode progMode = ArgParse::RunMode::MODE_NONE;

    static const char *optsShort("hk:mo:pst:"); /* Short options. */
    static const struct option optsLong[] = {
        /* Long options.  */
        {"help", no_argument, nullptr, 'h'},          {"knum", required_argument, nullptr, 'k'},
        {"measure", no_argument, nullptr, 's'},       {"parallel", no_argument, nullptr, 'p'},
        {"output", required_argument, nullptr, 'o'},  {"sequential", no_argument, nullptr, 's'},
        {"threads", required_argument, nullptr, 't'}, {nullptr, 0, nullptr, 0}};

    /* Prepare the error message. */
    errMsg += ": Error - ";

    /* Disable default getopt() stderr output. */
    opterr = 0;

    while ((c = getopt_long(this->argCnt, this->argArr, optsShort, optsLong, nullptr)) != -1) {
        switch (c) {
        case 'h':
            /* Help argument. */
            arguments.help = true;
            progMode = RunMode::MODE_HELP;

            break;

        case 'k':
            /* K number specification. */
            arguments.knum = cstr2uint(optarg);

            break;

        case 'm':
            /* Time measurement argument. */
            arguments.measure = true;

            break;

        case 's':
            /* Sequential algorithm argument. */
            arguments.sequential = true;
            progMode = RunMode::MODE_SEQUENTIAL;

            break;

        case 'o':
            /* Output file specification. */
            arguments.outFile = optarg;

            break;

        case 'p':
            /* Parallel algorithm argument. */
            arguments.parallel = true;
            progMode = RunMode::MODE_PARALLEL;

            break;

        case 't': {
            /* Number of threads argument. */
            arguments.threads = cstr2uint(optarg);

            if (arguments.threads == 0) {
                errMsg += "Number of threads must be a non-zero positive integer.";
                throw std::runtime_error(errMsg);
            }

            break;
        }

        case '?':
            errMsg += "Unknown argument: ";
            errMsg += this->argArr[optind - 1];
            throw std::runtime_error(errMsg);

        case ':':
            errMsg += "Missing argument for option ";
            errMsg += this->argArr[optind - 1];
            throw std::runtime_error(errMsg);

        default:
            break;
        }
    }

    /* Correct parsed arguments and check for unexpected combinations. */
    /* If help is requested, print it no matter what. */
    if (arguments.help) {
        progMode = RunMode::MODE_HELP;
    }

    /* Parallel or sequential modes must be exclusive. */
    if (arguments.parallel + arguments.sequential > 1) {
        errMsg += "Parallel or sequential mode must be exclusive.";
        throw std::runtime_error(errMsg);
    }

    /* Checks related to the input file are irelevant in the help mode. */
    if (progMode != RunMode::MODE_HELP) {
        /* Check for existence of a file parameter. */
        if (optind == this->argCnt) {
            errMsg += "Argument with problem input file not present.";
            throw std::runtime_error(errMsg);
        }

        /* Check if there are no more than 1 non-option arguments. */
        if (optind != this->argCnt - 1) {
            errMsg += "Only one non-option argument (file) is expected.";
            throw std::runtime_error(errMsg);
        }
    }

    /* Fill in the args structure and assign it to the output. */
    arguments.inFile = this->argArr[optind];
    parsedArgs = arguments;

    return progMode;
}
