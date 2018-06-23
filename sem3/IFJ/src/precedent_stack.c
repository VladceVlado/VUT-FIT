/**
 * project: Implementace překladače imperativního jazyka IFJ17
 * authors: Martin Hošala (xhosal00), Peter Kubov (xkubov06), Tomáš Kukaň (xkukan00), Vladimír Dušek (xdusek27)
 * ------------------------------------
 * @author Martin Hošala (xhosal00)
 */

#include <stdlib.h>
#include <errno.h>

#include "debug_ifj.h"
#include "error_ifj.h"
#include "precedent_stack.h"

bool psEmpty(tPrecStack *stack)
{
	return stack->first == NULL;
}

void destroyPSE(tPrecElement *element)
{
	if (!element)
		return;

	if (element->prev)
		element->prev->next = element->next;

	if (element->next)
		element->next->prev = element->prev;


	if (!element->isTerm || (element->isTerm && !element->term.isToken)) {
		free(element);
	}
	else {
		freeToken(element->term.token);
		free(element);
	}

	element = NULL;
}

void psDestroyList(tPrecElement *begin)
{
	tPrecElement *toFreeNow = begin;
	tPrecElement *toFreeNext = NULL;

	while (toFreeNow) {
		toFreeNext = toFreeNow->next;
		destroyPSE(toFreeNow);
		toFreeNow = toFreeNext;
	}
}

void psDestroyStack(tPrecStack *stack)
{
	if (!psEmpty(stack))
		psDestroyList(stack->first);

	free(stack);
}

tPrecStack *psInit()
{
	tPrecStack *stack = malloc(sizeof(tPrecStack));
	if (!stack)
		return NULL;

	stack->top = NULL;
	stack->first = NULL;
	return stack;
}


tPrecElement *psMostTopTerm(tPrecStack *stack)
{

	if (psEmpty(stack))
		return NULL;

	tPrecElement *actual = stack->top;

	while (actual) {
		if (actual->isTerm)
			return actual;

		actual = actual->prev;
	}

	return NULL;
}

tPrecElement *psMostTopNoTerm(tPrecStack *stack)
{

	if (psEmpty(stack))
		return NULL;

	tPrecElement *actual = stack->top;

	while (actual) {
		if (!actual->isTerm)
			return actual;

		actual = actual->prev;
	}

	return NULL;
}

tToken *psMostTopToken(tPrecStack *stack)
{
	tPrecElement *element = psMostTopTerm(stack);

	if (element)
		return element->term.token;
	else
		return NULL;
}

tPrecElement *psPopReduce(tPrecStack *stack)
{
	tPrecElement *actual = stack->top;

	while (actual) {
		if (actual->isTerm && !actual->term.isToken) {

			if (stack->first == actual)
				stack->first = NULL;

			stack->top = actual->prev;

			if (stack->top)
				stack->top->next = NULL;

			tPrecElement *toReturn = actual->next;

			if (actual->prev)
				actual->prev->next = NULL;

			free(actual);
			toReturn->prev = NULL;
			return toReturn;
		}
		actual = actual->prev;
	}

	return NULL;
}

void psPushTerm(tPrecStack *stack, tToken *token)
{

	tPrecElement *newElement = malloc(sizeof(tPrecElement));

	if (!newElement) {
		errno = E_SYS;
		return;
	}

	DEBUG_PRINT("--------***-------PUSHING: %d", token->state);

	tPTerm newTerm;

	newTerm.isToken = true;
	newTerm.token = token;

	newElement->isTerm = true;
	newElement->term = newTerm;

	newElement->next = NULL;
	newElement->prev = stack->top;

	if (!psEmpty(stack))
		stack->top->next = newElement;

	stack->top = newElement;

	if (psEmpty(stack))
		stack->first = newElement;
}

void psPushNoTerm(tPrecStack *stack, tKeyWord type)
{
	tPrecElement *newElement = malloc(sizeof(tPrecElement));


	if (!newElement) {
		errno = E_SYS;
		return;
	}

	newElement->isTerm = false;
	newElement->type = type;

	newElement->next = NULL;
	newElement->prev = stack->top;

	if (!psEmpty(stack))
		stack->top->next = newElement;

	stack->top = newElement;

	if (psEmpty(stack))
		stack->first = newElement;
}

tPrecElement *psCreateHold()
{
	tPrecElement *newElement = malloc(sizeof(tPrecElement));
	tPTerm newTerm;


	if (!newElement) {
		errno = E_SYS;
		return NULL;
	}

	newTerm.isToken = false;

	newElement->isTerm = true;
	newElement->term = newTerm;

	newElement->next = NULL;
	newElement->prev = NULL;
	return newElement;
}

void psShift(tPrecStack *stack, tToken *token)
{
	tPrecElement *hold = psCreateHold();

	if (errno)
		return;

	if (psEmpty(stack)) {
		hold->next = NULL;
		hold->prev = NULL;
		stack->first = hold;
		stack->top = hold;

		psPushTerm(stack, token);
		return;
	}

	tPrecElement *mostTopTerm = psMostTopTerm(stack);

	if (!mostTopTerm) {
		hold->prev = NULL;
		stack->first->prev = hold;
		hold->next = stack->first;
		stack->first = hold;

		psPushTerm(stack, token);
		return;
	}

	if (mostTopTerm == stack->top) {
		hold->next = NULL;
		hold->prev = mostTopTerm;
		mostTopTerm->next = hold;
		stack->top = hold;

		psPushTerm(stack, token);
		return;
	}

	hold->next = mostTopTerm->next;
	hold->prev = mostTopTerm;
	mostTopTerm->next = hold;
	hold->next->prev = hold;

	psPushTerm(stack, token);
}

void psPrint(tPrecStack *stack)
{
	tPrecElement *elem = stack->first;

	fprintf(stderr, "\n$ ");

	while (elem) {
		if (elem->isTerm) {
			if (elem->term.isToken)
				fprintf(stderr, " <- TERM: %d", elem->term.token->state);
			else
				fprintf(stderr, " <- HOLD");
		}
		else
			fprintf(stderr, " <- NOTERM");

		elem = elem->next;
	}

	fprintf(stderr, "\n");

}

