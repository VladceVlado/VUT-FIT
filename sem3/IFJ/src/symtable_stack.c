/**
 * project: Implementace překladače imperativního jazyka IFJ17
 * authors: Martin Hošala (xhosal00), Peter Kubov (xkubov06), Tomáš Kukaň (xkukan00), Vladimír Dušek (xdusek27)
 * ------------------------------------
 * @author  Vladimír Dušek (xdusek27)
 * @date	November, 2017
 * @brief	Stack of symtables for syntax analysis.
 * @file    symtable_stack.c
 */


#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "error_ifj.h"
#include "symtable.h"
#include "symtable_stack.h"


typedef struct stselem {
	htab_t *table;
	struct stselem *prev;
} *STSElemPtr;


struct symtableStack {
	STSElemPtr top;
};


STStack *newSTStack()
{
	return calloc(sizeof(struct symtableStack), 1);
}


void freeSTStack(STStack *stack)
{
	while (stack->top) {
		stStackPop(stack);
	}

	free(stack);
}


void stStackInit(STStack *stack)
{
	stack->top = NULL;
}


int stStackIsEmpty(STStack *stack)
{
	return stack->top == NULL;
}


htab_t *stStackTop(STStack *stack)
{
	if (!stack)
		return NULL;

	return stack->top == NULL? NULL:stack->top->table;
}


void stPushTable(STStack *stack, htab_t *table)
{
	STSElemPtr newTop = calloc(sizeof(struct stselem), 1);
	if (newTop == NULL) {
		errno = E_SYS;
		return;
	}

	newTop->table = table;
	newTop->prev = stack->top;
	stack->top = newTop;
}


void stStackPop(STStack *stack)
{
	if (stack->top != NULL) {
		STSElemPtr newTop = stack->top->prev;
		htab_destroy(stack->top->table);
		free(stack->top);
		stack->top = newTop;
	}
}


htab_item_t *searchID(char *string, STStack *stack)
{
	if (stack == NULL)
		return NULL;

	STSElemPtr idx = stack->top;
	htab_item_t *tab = NULL;

	while (idx != NULL) {
		if ((tab = htab_find(idx->table, string)) != NULL) {
			return tab;
		}
		idx = idx->prev;
	}
	return NULL;
}
