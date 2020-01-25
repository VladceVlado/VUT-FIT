/**
 * project: Implementace překladače imperativního jazyka IFJ17
 * authors: Martin Hošala (xhosal00), Peter Kubov (xkubov06), Tomáš Kukaň (xkukan00), Vladimír Dušek (xdusek27)
 * ------------------------------------
 * @Author	Peter Kubov (xkubov06@stud.fit.vutbr.cz)
 * @date	November, 2017
 * @brief	Essential structures for semantic analysis for IFJ project.
 *
 * Module provides implementation of stack used in semantic analysis.
 */

#include <stdlib.h>
#include <errno.h>

#include "debug_ifj.h"
#include "error_ifj.h"
#include "semantic_essentials.h"

SAStack newSAStack()
{
	return calloc(1, sizeof(struct semstack));
}

void freeSAStack(SAStack stack)
{
	if (stack == NULL)
		return;

	while (stack->top != NULL) {
		if (stack->top->spec->val.prefix != NULL) {
			if (stack->top->spec->val.prefix->name != NULL)
				free(stack->top->spec->val.prefix->name);
			if (stack->top->spec->val.prefix->prefix != NULL)
				free(stack->top->spec->val.prefix->prefix);
			free(stack->top->spec->val.prefix);
		}
		saSPop(stack);
	}

	free(stack);
}

void saSinit(SAStack stack)
{
	if (stack != NULL)
		stack->top = NULL;
}

int saSIsEmpty(SAStack stack)
{
	return stack == NULL || stack->top == NULL;
}

void saSPop(SAStack stack)
{
	if (!saSIsEmpty(stack)) {
		SEMElemPtr pToDelete = stack->top;
		if (stack->top->spec != NULL)
			free(stack->top->spec);
		stack->top = stack->top->prev;
		free(pToDelete);
	}
}

void saSPush(SAStack stack, SAElemValPtr val)
{
	errno = 0;

	SEMElemPtr newTop = calloc(1, sizeof(struct semElement));

	if (newTop == NULL) {
		free(val);
		errno = E_SYS;
		return;
	}

	newTop->prev = stack->top;
	newTop->spec = val;
	stack->top = newTop;
}

void saSPushDefPtr(SAStack stack, htab_item_t * item)
{
	errno = 0;

	SAElemValPtr val = calloc(1, sizeof(struct elemval));
	if (val == NULL) {
		errno = E_SYS;
		return;
	}
	
	val->state = SA_ID_PTR;
	val->val.item = item;

	saSPush(stack, val);
}

void saSPushPrefix(SAStack stack, htab_item_t *prefix)
{
	errno = 0;

	SAElemValPtr val = calloc(1, sizeof(struct elemval));
	if (val == NULL) {
		errno = E_SYS;
		return;
	}
	
	val->state = SA_PREFIX;
	val->val.prefix = prefix;

	saSPush(stack, val);
}

SAElemValPtr saSTop(SAStack stack)
{
	if (saSIsEmpty(stack))
		return NULL;

	return stack->top->spec;
}
