/**
 * project: Implementace překladače imperativního jazyka IFJ17
 * authors: Martin Hošala (xhosal00), Peter Kubov (xkubov06), Tomáš Kukaň (xkukan00), Vladimír Dušek (xdusek27)
 * ------------------------------------
 * @Author	Peter Kubov (xkubov06@stud.fit.vutbr.cz)
 * @date	November, 2017
 * @brief	Essential structures for syntax analysis for IFJ project.
 *
 * Module provides implementation of stack used by syntax analysis.
 */

#ifndef _IFJ42_PARSER_ESSENTIALS_H
#define _IFJ42_PARSER_ESSENTIALS_H

#include "scanner.h"
#include "semantic_rules.h"
#include "terms.h"

/**
 * @brief	Type representing stack required by syntax analysis.
 * @ingroup	parser
 */
typedef struct stack *TStack;

/**
 * @brief	Allocates memory required by syntax stack.
 * @ingroup	parser
 *
 * @return	On success returns pointer to memory reserved for syntax stack.
 */
TStack newStack();

/**
 * @brief	Frees memory of syntax stack and it's elements.
 * @ingroup	parser
 *
 * @param	stack	Pointer to syntax stack to be freed.
 */
void freeStack(TStack stack);

/**
 * @brief	Initializes syntax stack passed to function as parameter.
 * @ingroup	parser
 *
 * @param	stack	Represents stack to be initialized.
 */
void stackInit(TStack stack);

/**
 * @brief	Checks if stack passed as argument is empty (has 0 elements)
 * @ingroup	parser
 *
 * @param	stack	Stack to be checked.
 *
 * @return	0 if stack is not empty non-zero velue otherwise.
 */
int stackIsEmpty(TStack stack);

/**
 * @brief	Creates new element of type representing terminal and pushes it on stack.
 * @ingroup	parser
 *
 * @param	stack	Stack that is going to have one more element.
 * @param	term	Specification of terminal to be pushed to stack.
 *
 * @error	When error occoures errno is set appropriately.
 */
void stackPushTerm(TStack stack, TTermVal term);

/**
 * @brief	Creates new element of type representing non-terminal and pushes it on stack.
 * @ingroup	parser
 *
 * @param	stack	Stack that is going to have one more element.
 * @param	nterm	Specification of non-terminal to be pushed to stack.
 *
 * @error	When error occoures errno is set appropriately.
 */
void stackPushNTerm(TStack stack, TNTermVal nterm);

/**
 * @brief	Creates new element of type representing semantic action and pushes it on stack.
 * @ingroup	parser
 *
 * @param	stack	Stack that is going to have one more element.
 * @param	rule	Semantic action to be pushed to stack.
 *
 * @error	When error occoures errno is set appropriately.
 */
void stackPushSemRule(TStack stack, TSemRule rule);

/**
 * @brief	Pops stack, frees semantic stack's top.
 * @ingroup	parser
 *
 * @param	stack	Stack to be popped.
 */
void stackPop(TStack stack);

/**
 *  @brief checks if terminal is on top of the stack
 *	
 *	@returns Zero if terminal is not on the top of the stack, non-zero value otherwise.
 */
int isTermOnTop(TStack stack);

/**
 *  @brief checks if non-terminal is on top of the stack
 *	
 *	@returns Zero if non-terminal is not on the top of the stack, non-zero value otherwise.
 */
int isNTermOnTop(TStack stack);

/**
 *  @brief checks if semantic rule is on top of the stack
 *	
 *	@returns Zero if semantic rule is not on the top of the stack, non-zero value otherwise.
 */
int isSemRuleOnTop(TStack stack);

/**
 * @brief	Exposes value of the top terminal of the stack.
 * @ingroup	parser
 *
 * @param	stack	Specifies stack thats top value is going to be exposed..
 *
 * @return	Value of the top terminal of the stack, NULL when stack is empty or term is not on top. 
 */
TTermVal stackTopTerm(TStack stack);

/**
 * @brief	Exposes value of the top non-terminal of the stack.
 * @ingroup	parser
 *
 * @param	stack	Specifies stack thats top value is going to be exposed..
 *
 * @return	Value of the top terminal of the stack, NULL when stack is empty or non-terminal is not on top. 
 */
TNTermVal stackTopNTerm(TStack stack);

/**
 * @brief	Exposes semantic asction on the top term of the stack.
 * @ingroup	parser
 *
 * @param	stack	Specifies stack thats top value is going to be exposed..
 *
 * @return	Semantic action on top of the stack, NULL when stack is empty or semantic action is not on top. 
 */
TSemRule stackTopSemRule(TStack stack);

#endif
