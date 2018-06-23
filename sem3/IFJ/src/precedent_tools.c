/**
 * project: Implementace překladače imperativního jazyka IFJ17
 * authors: Martin Hošala (xhosal00), Peter Kubov (xkubov06), Tomáš Kukaň (xkukan00), Vladimír Dušek (xdusek27)
 * ------------------------------------
 * @author Martin Hošala (xhosal00)
 */

#include <stdlib.h>
#include <errno.h>

#include "precedent_tools.h"

tInFceStack *inFceInit()
{
	tInFceStack *stack = malloc(sizeof(tInFceStack));

	if (!stack) {
		errno = E_SYS;
		return NULL;
	}

	stack->top = NULL;

	return stack;
}

tInFceElement *inFceTop(tInFceStack* stack)
{
	if (stack)
		return stack->top;

	return NULL;
}

void inFcePush(tInFceStack *stack,  htab_item_t *element)
{
	if (!stack)
		return;

	tInFceElement *newElement = malloc(sizeof(tInFceElement));

	if (!newElement) {
		errno = E_SYS;
		return;
	}

	newElement->paramsChecked = 0;
	newElement->paramsCount = element->params_num;
	newElement->params = element->parameters;
	newElement->prev = stack->top;
	stack->top = newElement;
}

void inFcePop(tInFceStack *stack)
{
	if (!stack || !stack->top)
		return;

	tInFceElement *toFree = stack->top;

	stack->top = stack->top->prev;
	free(toFree);
}

void inFceDestroy(tInFceStack *stack)
{
	if (!stack)
		return;

	while (stack->top) {
		inFcePop(stack);
	}

	free(stack);
}
