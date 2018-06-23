/**
 * project: Implementace překladače imperativního jazyka IFJ17
 * authors: Martin Hošala (xhosal00), Peter Kubov (xkubov06), Tomáš Kukaň (xkukan00), Vladimír Dušek (xdusek27)
 * ------------------------------------
 * @author Martin Hošala (xhosal00)
 */

/**
 * @file precedent_stack.h
 * @brief Contains declaration of stack needed to check and evaluate an expression and functions to work with it.
 *
 */

#ifndef IFJ17_PRECEDENT_STACK_H
#define IFJ17_PRECEDENT_STACK_H

#include "scanner.h"

/// Token considered as terminal and a special "hold" terminal.
typedef struct {
	bool isToken; ///< We have only one term which is not a token - hold
	tToken* token;
} tPTerm;

/**
 * @brief PrecedentStack element.
 */
typedef struct precElement {
	bool isTerm; ///< Information if the element is terminal.
	tPTerm term; ///< Concrete terminal.

	tKeyWord type; ///< If the element is not terminal, holds the information about noTerminal type (int, string, ...)

	struct precElement *next;
	struct precElement *prev;
} tPrecElement;

/**
 * @brief PrecedentStack
 *
 * The precedent stack is not a simple stack, but it has also list features to help with the stack reduction.
 */
typedef struct {
	tPrecElement *first;
	tPrecElement *top;
} tPrecStack;

/**
 * @brief Serves to create and initialize a new precedentStack.
 * @return  New precedentStack.
 */
tPrecStack *psInit();

/**
 *
 * @param stack The given precedentStack
 * @return Element in the given stack closest to the stack top which is a terminal. NULL if there is not such element.
 */
tPrecElement *psMostTopTerm(tPrecStack *stack);

/**
 *
 * @param stack The given precedentStack
 * @return Element in the given stack closest to the stack top which is not a terminal. NULL if there is not such
 *         element.
 */
tPrecElement *psMostTopNoTerm(tPrecStack *stack);

/**
 *
 * @param stack The given precedentStack
 * @return Token from element in the given stack closest to the stack top which is a terminal and also token. NULL if
 *         there is not such element.
 */
tToken *psMostTopToken(tPrecStack *stack);

/**
 * @brief Serves to apply precedent rule reduce.
 *
 * Finds a hold closest to the stack top, deletes it and returns list starting with hold's next element and ending with
 * actual stack top. Element before the hold will be considered as the new stack top and any of returned elements are no
 * longer considered as elements in the stack.
 *
 * @param stack The given precedentStack
 * @return List containing all elements from a "hold" closest to the stack top to the stack top. NULL if the given
 *         stack does not contain any hold.
 */
tPrecElement *psPopReduce(tPrecStack *stack);

/**
 * @brief Creates a new terminal containing the given token and pushes it to the given stack.
 * @param stack The given precedentStack
 * @param token The given token
 */
void psPushTerm(tPrecStack *stack, tToken *token);

/**
 * @brief Creates a new noTerminal containing with the given type and pushes it to the given stack.
 * @param stack The given precedentStack
 * @param type The given type
 */
void psPushNoTerm(tPrecStack *stack, tKeyWord type);

/**
 * @brief Serves to apply precedent rule shift.
 *
 * Places hold after the most top terminal in the given stack and pushes new a terminal containing given token to the
 * given stack.
 *
 * @param stack The given precedentStack
 * @param token The given token
 */
void psShift(tPrecStack *stack, tToken *token);

/**
 * @brief Destroys all precedentStackElements in the list beginning from the given element.
 * @param begin The given element considered as the beginning of the list to be destroyed.
 */
void psDestroyList(tPrecElement *begin);

/**
 * @brief Destroys the whole stack and all elements in it.
 * @param stack The given precedentStack
 */
void psDestroyStack(tPrecStack *stack);

/**
 *
 * @param stack The given precedentStack
 * @return True if the given stack is empty.
 */
bool psEmpty(tPrecStack *stack);

/**
 * @brief Debug function.
 *
 * Servers to print current stack content.
 *
 * @param stack The given precedentStack
 */
void psPrint(tPrecStack *stack);

#endif //IFJ17_PRECEDENT_STACK_H
