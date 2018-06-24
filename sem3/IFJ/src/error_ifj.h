/**
 * @file error_ifj17.h
 * @brief Defines the types of the errors and macros for the correct ending
 *        of the program.
 */

#ifndef _IFJ17_ERROR_H
#define _IFJ17_ERROR_H

/// Error types.
typedef enum {
	E_OK,
	E_LEX, ///< Lexical error, exit code 1
	E_SYN, ///< Syntax error, exit code 2
	E_SEM_DEF, ///< Sematic error, definition issue 3
	E_SEM_COMPA, ///< Sematic error, compability issue 4
	E_SEM, ///< Sematic error, other issues 6
	E_SYS ///< System error, exit code 99
} tERROR;

/**
 * @brief Exits the program with the appropriate code.
 *
 * The exit code depends on the given error type.
 *
 * @param error Type of the error that occured.
 * @param msg More detailed description of the issue. If NULL basic message will
 *            be written.
 */
void errorExit(tERROR error, char *msg);

/// Macro for the correct ending of the program in the case of an error occured.
#define ERROR(CODE, MSG) errorExit(CODE, MSG)

#endif /*_IFJ17_ERROR_H*/
