/**
 * project: Implementace překladače imperativního jazyka IFJ17
 * authors: Martin Hošala (xhosal00), Peter Kubov (xkubov06), Tomáš Kukaň (xkukan00), Vladimír Dušek (xdusek27)
 * ------------------------------------
 * @author Martin Hošala (xhosal00)
 */

#include <errno.h>

#include "debug_ifj.h"
#include "error_ifj.h"
#include "precedent.h"
#include "precedent_rules.h"
#include "precedent_stack.h"
#include "precedent_table.h"
#include "scanner.h"

void addPartToRule(tRuleValue *value, tRulePartValue part)
{
	if (errno)
		return;

	*value *= 10;
	*value += part;
}

tRule evalRule(tPrecElement* element)
{
	DEBUG_MESSAGE("EVALUATING RULE");

	tRule rule = {.value = 0, .isRelation = false};

	if (errno)
		return rule;

	while (element)	{
		if (!element->isTerm) {
			addPartToRule(&rule.value, RP_NO_TERM);
			element = element->next;
			continue;
		}

		switch (element->term.token->state) {
			case S_OPEN_BRACKET:
				addPartToRule(&rule.value, RP_L_BRACKET);
				break;

			case S_CLOSE_BRACKET:
				addPartToRule(&rule.value, RP_R_BRACKET);
				break;

			case S_IDENTIFIER_HT:
				if (element->term.token->val.identifierHT->is_function) {
					rule.value = 10;
					return rule;
				}

				addPartToRule(&rule.value, RP_ID);
				break;

			case S_PLUS:
				addPartToRule(&rule.value, RP_PLUS);
				break;

			case S_MINUS:
				addPartToRule(&rule.value, RP_MINUS);
				break;

			case S_MULTIPLICATION:
				addPartToRule(&rule.value, RP_MUL);
				break;

			case S_INT_DIVISION:
				addPartToRule(&rule.value, RP_DIV_INT);
				break;

			case S_DOUBLE_DIVISION:
				addPartToRule(&rule.value, RP_DIV_REAL);
				break;

			case S_DOUBLE:
			case S_INT:
			case S_STRING:
				addPartToRule(&rule.value, RP_ID);
				break;

			case S_BIGGER:
				addPartToRule(&rule.value, RP_RE_G);
				rule.isRelation = true;
				break;

			case S_LESS:
				addPartToRule(&rule.value, RP_RE_L);
				rule.isRelation = true;
				break;

			case S_EQUAL_ASSIGN:
				addPartToRule(&rule.value, RP_RE_E);
				rule.isRelation = true;
				break;

			case S_BIGGER_EQUAL:
				addPartToRule(&rule.value, RP_RE_GE);
				rule.isRelation = true;
				break;

			case S_LESS_EQUAL:
				addPartToRule(&rule.value, RP_RE_LE);
				rule.isRelation = true;
				break;

			case S_NOT_EQUAL:
				addPartToRule(&rule.value, RP_RE_NE);
				rule.isRelation = true;
				break;

			default:
				errno = E_SYN;
				break;

		}

		element = element->next;
	}

	DEBUG_PRINT("EVALUATED TO: %d; IS RELATION: %d", rule.value, rule.isRelation);

	return rule;
}

bool doJob(tJob job, tPrTools *tools, tToken *token)
{
	if (errno)
		return false;

	switch (job) {
		case J_REDUCE:
			DEBUG_MESSAGE("ATTEMPTING TO REDUCE");

			tPrecElement *list = psPopReduce(tools->stack);
			tKeyWord ruleResultType = applyRule(evalRule(list), tools, list);

			if (ruleResultType)
				psPushNoTerm(tools->stack, ruleResultType);
			else if (!errno)
				errno = E_SYN;

			psDestroyList(list);
			return false;

		case J_PUSH:
			DEBUG_MESSAGE("ATTEMPTING TO PUSH");

			if (tools->inFceAndComma || tools->inFceAndRBracket) {
				DEBUG_MESSAGE("GOING TO PRETYPE TOP");

				if (!castTopGen(psMostTopNoTerm(tools->stack), tools)) {
					errno = E_SEM_COMPA;
					break;
				}
				tools->inFceAndRBracket = false;
				tools->inFceAndComma = false;
			}

			psPushTerm(tools->stack, token);

			break;

		case J_SHIFT:
			DEBUG_MESSAGE("ATTEMPTING TO SHIFT");

			psShift(tools->stack, token);

			break;

		case J_NONE:
			if (!tools->endOfInput) {
				DEBUG_MESSAGE("ERROR SET");

				if (inFceTop(tools->inFceStack))
					errno = E_SEM_DEF;
				else
					errno = E_SYN;
			}

			break;
	}

	return true;
}

tPrecedentTableTerm tokenToPrTableTerm(tToken *token)
{
	if (errno)
		return T_DOLLAR;

	if (!token) {
		DEBUG_MESSAGE("STACK TOKEN STATE: NULL - $");

		return T_DOLLAR;
	}

	DEBUG_PRINT("STACK TOKEN STATE: %d", token->state );

	switch (token->state) {
		case S_PLUS:
		case S_MINUS:
			return T_PLUS;

		case S_MULTIPLICATION:
		case S_DOUBLE_DIVISION:
			return T_MUL;

		case S_INT_DIVISION:
			return T_DIV_INT;

		case S_OPEN_BRACKET:
			return T_BRC_L;

		case S_CLOSE_BRACKET:
			return T_BRC_R;

		case S_DOUBLE:
		case S_INT:
		case S_STRING:
			return T_ID;

		case S_IDENTIFIER_HT:
			if (token->val.identifierHT->is_function) {
				return T_FCE;
			}
			else {
				return T_ID;
			}

		case S_COMMA:
			return T_COMMA;

		case S_EOL:
			return T_DOLLAR;

		case S_BIGGER:
		case S_LESS:
		case S_EQUAL_ASSIGN:
		case S_BIGGER_EQUAL:
		case S_LESS_EQUAL:
		case S_NOT_EQUAL:
			return T_REL;

		default:
			DEBUG_MESSAGE("CANNOT PARSE");
			errno = E_SYN;
			break;
	}
	return T_DOLLAR;
}

tToken *analyseExpr(tPrTools *tools)
{
	DEBUG_MESSAGE("starting precedent analysis");
	errno = 0;

	static bool init = true;
	static tPTable prTable;

	if (init) {
		ptInit(&prTable);
		init = false;
	}

	tools->stack = psInit();
	tools->inFceStack = inFceInit();
	tools->endOfInput = false;

	bool getNew = true;

	tToken *token = NULL;
		bool notFirst = false;

	while (true) {
		DEBUG_MESSAGE("---------------ONE CYCLE-----------------------------");

		if (errno)
			break;

		tools->inFceAndRBracket = false;
		tools->inFceAndComma = false;

		if (getNew) {
			DEBUG_MESSAGE("ASKING FOR A TOKEN");

			token = newToken();
			if (!token) {
				errno = E_SYS;
				break;
			}

			getToken(tools->file, token, tools->symtableStack);
		}

		DEBUG_PRINT("INPUT TOKEN STATE: %d", token->state);

		switch (token->state) {

			case S_PLUS:
			case S_MINUS:
				getNew = doJob(prTable[T_PLUS][tokenToPrTableTerm(psMostTopToken(tools->stack))], tools, token);

				break;

			case S_MULTIPLICATION:
			case S_DOUBLE_DIVISION:
				getNew = doJob(prTable[T_MUL][tokenToPrTableTerm(psMostTopToken(tools->stack))], tools, token);

				break;

			case S_INT_DIVISION:
				getNew = doJob(prTable[T_DIV_INT][tokenToPrTableTerm(psMostTopToken(tools->stack))], tools, token);

				break;

			case S_OPEN_BRACKET:
				getNew = doJob(prTable[T_BRC_L][tokenToPrTableTerm(psMostTopToken(tools->stack))], tools, token);
				break;

			case S_CLOSE_BRACKET:
				if (inFceTop(tools->inFceStack)) {
					tools->inFceAndRBracket = true;
				}

				getNew = doJob(prTable[T_BRC_R][tokenToPrTableTerm(psMostTopToken(tools->stack))], tools, token);

				break;

			case S_DOUBLE:
			case S_INT:
			case S_STRING:
				getNew = doJob(prTable[T_ID][tokenToPrTableTerm(psMostTopToken(tools->stack))], tools, token);

				break;

			case S_IDENTIFIER_HT:
				if (token->val.identifierHT->is_function) {
					inFcePush(tools->inFceStack, token->val.identifierHT);
					getNew = doJob(prTable[T_FCE][tokenToPrTableTerm(psMostTopToken(tools->stack))], tools, token);
				}
				else {
					getNew = doJob(prTable[T_ID][tokenToPrTableTerm(psMostTopToken(tools->stack))], tools, token);
				}

				break;

			case S_COMMA:
				if (inFceTop(tools->inFceStack)) {
					tools->inFceAndComma = true;
				}

				getNew = doJob(prTable[T_COMMA][tokenToPrTableTerm(psMostTopToken(tools->stack))], tools, token);

				break;

			case S_BIGGER:
			case S_LESS:
			case S_EQUAL_ASSIGN:
			case S_BIGGER_EQUAL:
			case S_LESS_EQUAL:
			case S_NOT_EQUAL:
				getNew = doJob(prTable[T_REL][tokenToPrTableTerm(psMostTopToken(tools->stack))], tools, token);

				break;

			case S_IDENTIFIER:
				errno = E_SEM_DEF;

				break;

			case S_EOL:
			default:
				if(notFirst)
					tools->endOfInput = true;

				getNew = doJob(prTable[T_DOLLAR][tokenToPrTableTerm(psMostTopToken(tools->stack))], tools, token);

				if (getNew) {
					if (!errno && !psEmpty(tools->stack) && tools->stack->top == tools->stack->first && psMostTopNoTerm(tools->stack)) {
						DEBUG_MESSAGE("----------GOT NO MORE JOB -> ENDING");

						if (!finalCastGen(tools)) {
							DEBUG_MESSAGE("NASTAVUJEM CHYBU");
							errno = E_SEM_COMPA;
						}

						psDestroyStack(tools->stack);
						inFceDestroy(tools->inFceStack);

						return token;
					}
					if (!errno) {
						if (inFceTop(tools->inFceStack) != NULL) {
							DEBUG_MESSAGE("SETTING E_SEM_DEF");
							errno = E_SEM_DEF;
						}
						else {
							DEBUG_MESSAGE("SETTING E_SYN");
							errno = E_SYN;
						}
					}
				}

				break;
		}

		notFirst = true;
	}

	psDestroyStack(tools->stack);
	inFceDestroy(tools->inFceStack);

	return token;
}
