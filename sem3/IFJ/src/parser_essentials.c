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

#include <errno.h>
#include <stdlib.h>

#include "error_ifj.h"
#include "parser_essentials.h"

/// Values obtainable by stack elements
typedef enum {TTER, TNTER, TSEMRULE} ElemValues;

typedef union spec {
		TNTermVal nonterm;
		TTermVal term;
		TSemRule rule;
} ElemVal;

/// Implementation of element in stack
typedef struct element {
	ElemValues value; /// Value of element
	/// specification of value e.g. internals
	ElemVal spec;
	/// poinnter to previous (stack -> only top element is available)
	struct element *prev;
} *TElemPtr;

/// Implementation of stack required for syntax analysis
struct stack {
	TElemPtr top;
};

int stackIsEmpty(TStack stack)
{
	return stack->top == NULL;
}

TStack newStack()
{
	return calloc(1, sizeof(struct stack));
}

void freeStack(TStack stack)
{
	while (!stackIsEmpty(stack)) {
		stackPop(stack);
	}
	
	free(stack);
}

void stackInit(TStack stack)
{
	stack->top = NULL;
}

void stackPush(TStack stack, TElemPtr newTop)
{
	newTop->prev = stack->top;
	stack->top = newTop;
}

void stackPushTerm(TStack stack, TTermVal term)
{
	TElemPtr newTop = malloc(sizeof(struct element));
	if (newTop == NULL) {
		errno = E_SYS;
		return;
	}

	newTop->value = TTER;
	newTop->spec.term = term;
	
	stackPush(stack, newTop);
}

void stackPushNTerm(TStack stack, TNTermVal nterm)
{
	TElemPtr newTop = malloc(sizeof(struct element));
	if (newTop == NULL) {
		errno = E_SYS;
		return;
	}
	
	newTop->value = TNTER;
	newTop->spec.nonterm = nterm;

	stackPush(stack, newTop);
}

void stackPushSemRule(TStack stack, TSemRule rule)
{

	TElemPtr newTop = malloc(sizeof(struct element));
	if (newTop == NULL) {
		errno = E_SYS;
		return;
	}

	newTop->value = TSEMRULE;
	newTop->spec.rule = rule;

	stackPush(stack, newTop);
}

void stackPop(TStack stack)
{

	if (!stackIsEmpty(stack)) {	

		TElemPtr newTop = stack->top->prev;
		free(stack->top);
		stack->top = newTop;

	}
}

int isTermOnTop(TStack stack)
{
	return stack->top != NULL && stack->top->value == TTER;
}

int isNTermOnTop(TStack stack)
{
	return stack->top != NULL && stack->top->value == TNTER;
}

int isSemRuleOnTop(TStack stack)
{
	return stack->top != NULL && stack->top->value == TSEMRULE;
}

TTermVal stackTopTerm(TStack stack)
{
	return isTermOnTop(stack) ? stack->top->spec.term : NULL_TERM;
}

TNTermVal stackTopNTerm(TStack stack)
{
	return isNTermOnTop(stack) ? stack->top->spec.nonterm : NULL_NTERM;
}

TSemRule stackTopSemRule(TStack stack)
{
	return isSemRuleOnTop(stack) ? stack->top->spec.rule : NULL;
}

