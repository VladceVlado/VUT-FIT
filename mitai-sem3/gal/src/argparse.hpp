/**
 * @file argparse.hpp
 * @brief Argument parsing module.
 * @author Patrik Goldschmidt, <xgolds00@stud.fit.vutbr.cz>
 * @date 12-07-2020, last rev. 12-15-2020
 */

#ifndef ARGPARSE_HPP_INCLUDED

#include <string>

class ArgParse {
  public:
    /** @brief Program arguments structure. */
    struct args {
        bool help;        /**< Print help message.     */
        bool measure;     /**< Measure execution time. */
        bool parallel;    /**< Parallel program run.   */
        bool sequential;  /**< Sequential program run. */
        unsigned knum;    /**< Graph radio K-coloring number. */
        unsigned threads; /**< Number of processing threads. */
        char *inFile;     /**< Path to input problem.  */
        char *outFile;    /**< Output file path.       */
    };

    /** @brief Program mode specification. */
    enum class RunMode {
        MODE_NONE,       /**< No explicitly specified mode. */
        MODE_HELP,       /**< Help message  */
        MODE_SEQUENTIAL, /**< Sequential run mode. */
        MODE_PARALLEL,   /**< Parallel run mode.   */
    };

    /**
     * @brief Constructs ArgParse class with given program arguments.
     *
     * @param[in] argc Number of arguments.
     * @param[in] argv Array of argument values as received from OS.
     */
    ArgParse(int argc, char **argv) noexcept;

    /**
     * @brief Parses command line arguments, performing necessary validation
     *        procedures and returning result via an output structure.
     *
     * @param[out] parsedArgs Parsed arguments.
     *
     * @throw std::runtime_error on invalid argument validation.
     * @return Runmode the application should run in.
     */
    RunMode args_process(args &parsedArgs);

  private:
    int argCnt;    /**< Number of program arguments. */
    char **argArr; /**< Pointer to command line args. */
};

#endif /* ARGPARSE_HPP_INCLUDED */
