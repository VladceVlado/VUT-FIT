/**
 * project: Implementace překladače imperativního jazyka IFJ17
 * authors: Martin Hošala (xhosal00), Peter Kubov (xkubov06), Tomáš Kukaň (xkukan00), Vladimír Dušek (xdusek27)
 * ------------------------------------
 * @author Martin Hošala (xhosal00)
 */

/**
 * @file precedent_tools.h
 * @brief Contains declaration of tools needed to check and evaluate an expression.
 */

#ifndef IFJ17_PRECEDENT_TOOLS_H
#define IFJ17_PRECEDENT_TOOLS_H

#include <stdbool.h>

#include "error_ifj.h"
#include "generator.h"
#include "ilist.h"
#include "precedent_stack.h"
#include "scanner.h"
#include "symtable.h"
#include "symtable_stack.h"

/**
 * @brief When an expression contains a function call, metadata about the function being parsed are stored in this type.
 */
typedef struct inFce {
	tParam *params; ///< Function parameters
	unsigned paramsCount; ///< Function parameters count
	unsigned paramsChecked; ///< Function parameters, that have been successfully checked
	struct inFce *prev;
} tInFceElement;

/**
 * @brief When an expression contains a function call, metadata about the function being actually parsed are stored on
 *        the top of the inFceStack.
 */
typedef struct {
	tInFceElement *top; ///< Top of the stack
} tInFceStack;

/**
 * @brief Tmp variables needed to generate some instructions which need variables.
 */
typedef struct {
	htab_item_t *var_string1;
	htab_item_t *var_string2;
	htab_item_t *var_string3;
	htab_item_t *var_int;
	htab_item_t *var_float;
} tPrGlobalVars;

/**
 * @brief Type containing tools needed to analyse an expression with precedent analysis.
 */
typedef struct {
	FILE *file; ///< The input file
	STStack *symtableStack; ///< The stack of symtables being used
	tIList *iList; ///< The list where generated 3AK will be stored
	tPrGlobalVars tmps; ///< Pointers to variables declared to
	tKeyWord wantedValue; ///< The data type which should be the evaluated expression casted to

	bool endOfInput; ///< Informative variable
	bool inFceAndRBracket; ///< Informative variable
	bool inFceAndComma; ///< Informative variable
	tInFceStack* inFceStack; ///< The inFceStack
	tPrecStack* stack; ///< The precedentStack, see precedent_stack.h
} tPrTools;

/**
 *
 * @param stack Stack
 * @return If stack == NULL, NULL, else top element of the stack.
 */
tInFceElement *inFceTop(tInFceStack* stack);

/**
 *
 * @return New initialized stack.
 */
tInFceStack *inFceInit();

/**
 * @brief Pushes a new element on the top of the stack.
 * @param stack Stack
 * @param element Link to the symtable item containing information about function.
 */
void inFcePush(tInFceStack *stack,  htab_item_t *element);

/**
 * @brief Deletes the element on the top of the stack.
 * @param stack Stack
 */
void inFcePop(tInFceStack *stack);

/**
 * @brief Deletes all elements from the stack.
 * @param stack Stack
 */
void inFceDestroy(tInFceStack *stack);


#endif //IFJ17_PRECEDENT_TOOLS_H
