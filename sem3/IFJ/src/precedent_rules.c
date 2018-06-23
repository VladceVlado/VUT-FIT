/**
 * project: Implementace překladače imperativního jazyka IFJ17
 * authors: Martin Hošala (xhosal00), Peter Kubov (xkubov06), Tomáš Kukaň (xkukan00), Vladimír Dušek (xdusek27)
 * ------------------------------------
 * @author Martin Hošala (xhosal00)
 */

#include <errno.h>
#include <stdio.h>

#include "debug_ifj.h"
#include "error_ifj.h"
#include "generator.h"
#include "ilist.h"
#include "precedent_rules.h"

bool finalCastGen(tPrTools *tools)
{
	if (errno)
		return false;

	tKeyWord actualType = psMostTopNoTerm(tools->stack)->type;
	tKeyWord wantedType = tools->wantedValue;

	if (wantedType == NOT_FOUND)
		return true;

	if (wantedType == actualType) {
		return true;
	}

	if (wantedType == KW_INTEGER && actualType == KW_DOUBLE) {
		DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA SUDU INT");
		iListCreateAppend(tools->iList, I_FLOAT2R2EINTS, NULL, NULL, NULL);

		return true;
	}

	if (wantedType == KW_DOUBLE && actualType == KW_INTEGER) {
		DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA DOUBLE");
		iListCreateAppend(tools->iList, I_INT2FLOATS, NULL, NULL, NULL);

		return true;
	}

	/*ready for boolean conversion */

//	if (wantedType == KW_BOOLEAN && actualType == KW_DOUBLE) {
//		DEBUG_MESSAGE("instr: PUSH KONSTANTA DOUBLE 0");
//		htab_item_t *newConst = htab_add_constant(stStackTop(tools->symtableStack), "0", KW_DOUBLE); //vytvorim konstantu NULL
//		iListCreateAppend(tools->iList, I_PUSHS, newConst, NULL, NULL);
//
//		DEBUG_MESSAGE("instr: STACK EQUAL");
//		iListCreateAppend(tools->iList, I_EQS, NULL, NULL, NULL);
//
//		DEBUG_MESSAGE("instr: STACK NOT");
//		iListCreateAppend(tools->iList, I_NOTS, NULL, NULL, NULL);
//
//		return true;
//	}
//
//	if (wantedType == KW_BOOLEAN && actualType == KW_INTEGER) {
//		DEBUG_MESSAGE("instr: PUSH KONSTANTA INTEGER 0");
//		htab_item_t *newConst = htab_add_constant(stStackTop(tools->symtableStack), "0", KW_INTEGER); //vytvorim konstantu NULL
//		iListCreateAppend(tools->iList, I_PUSHS, newConst, NULL, NULL);
//
//		DEBUG_MESSAGE("instr: STACK EQUAL");
//		iListCreateAppend(tools->iList, I_EQS, NULL, NULL, NULL);
//
//		DEBUG_MESSAGE("instr: STACK NOT");
//		iListCreateAppend(tools->iList, I_NOTS, NULL, NULL, NULL);
//
//		return true;
//	}

	return false;
}


bool castTopGen(tPrecElement *noTerm, tPrTools *tools)
{
	if (errno)
		return false;

	if (tools->inFceAndRBracket && (inFceTop(tools->inFceStack)->paramsCount == 0))
		return true;

	if (noTerm == NULL)
		return false;

	tKeyWord actualType = noTerm->type;

	tKeyWord wantedType = NOT_FOUND;

	unsigned whichParam = inFceTop(tools->inFceStack)->paramsChecked;
	inFceTop(tools->inFceStack)->paramsChecked++;

	tParam *param = inFceTop(tools->inFceStack)->params;

	for (unsigned i = 0; (i < whichParam) && param; ++i) {
		param = param->next;
	}

	if (!param)
		return false;

	wantedType = param->type;

	if (wantedType == actualType) {
		return true;
	}

	if (wantedType == KW_INTEGER && actualType == KW_DOUBLE) {
		DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA SUDU INT");
		iListCreateAppend(tools->iList, I_FLOAT2R2EINTS, NULL, NULL, NULL);

		return true;
	}

	if (wantedType == KW_DOUBLE && actualType == KW_INTEGER) {
		DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA DOUBLE");
		iListCreateAppend(tools->iList, I_INT2FLOATS, NULL, NULL, NULL);

		return true;
	}

	return false;
}

tCompatibility checkCompatibility(tPrecElement *first, tPrecElement *second)
{
	if (errno)
		return COM_NONE;

	tKeyWord firstType = first->type;
	tKeyWord secondType = second->type;

	DEBUG_PRINT("COMPATIBILITY: %d - %d", firstType, secondType);

	if (firstType == secondType) {
		if (secondType == KW_DOUBLE)
			return COM_DOUBLE;

		if (secondType == KW_INTEGER)
			return COM_INT;

		if (secondType == KW_STRING)
			return COM_STRING;
	}

	if (firstType == KW_DOUBLE && secondType == KW_INTEGER)
		return COM_DOUBLE_INT;

	if (firstType == KW_INTEGER && secondType == KW_DOUBLE)
		return COM_INT_DOUBLE;

	return COM_NONE;
}

tKeyWord applyRule(tRule rule, tPrTools *tools, tPrecElement *list)
{
	if (errno)
		return 1;

	tCompatibility comp = (tCompatibility) NOT_FOUND;
	if (!rule.isRelation)
	{
		switch (rule.value) {
			case R_BRACKETS:
				DEBUG_MESSAGE("applying rule R_BRACKETS");
				return list->next->type;

			case R_DIV_INT:
				DEBUG_MESSAGE("applying rule R_DIV_INT: ");

				comp = checkCompatibility(list, list->next->next);

				switch (comp) {
					case COM_INT:
						DEBUG_MESSAGE("instr: POP TO VAR");
						iListCreateAppend(tools->iList, I_POPS, tools->tmps.var_int, NULL, NULL);

						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA DOUBLE");
						iListCreateAppend(tools->iList, I_INT2FLOATS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: PUSH VAR");
						iListCreateAppend(tools->iList, I_PUSHS, tools->tmps.var_int, NULL, NULL);

						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA DOUBLE");
						iListCreateAppend(tools->iList, I_INT2FLOATS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK DIV");
						iListCreateAppend(tools->iList, I_DIVS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA INT ODSEKNUTIM");
						iListCreateAppend(tools->iList, I_FLOAT2INTS, NULL, NULL, NULL);

						break;

					case COM_INT_DOUBLE:
						DEBUG_MESSAGE("instr: POP TO VAR");
						iListCreateAppend(tools->iList, I_POPS, tools->tmps.var_float, NULL, NULL);

						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA DOUBLE");
						iListCreateAppend(tools->iList, I_INT2FLOATS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: PUSH VAR");
						iListCreateAppend(tools->iList, I_PUSHS, tools->tmps.var_float, NULL, NULL);

						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA SUDU INT");
						iListCreateAppend(tools->iList, I_FLOAT2R2EINTS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA DOUBLE");
						iListCreateAppend(tools->iList, I_INT2FLOATS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK DIV");
						iListCreateAppend(tools->iList, I_DIVS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA INT ODSEKNUTIM");
						iListCreateAppend(tools->iList, I_FLOAT2INTS, NULL, NULL, NULL);

						break;

					case COM_DOUBLE_INT:
						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA DOUBLE");
						iListCreateAppend(tools->iList, I_INT2FLOATS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: POP TO VAR");
						iListCreateAppend(tools->iList, I_POPS, tools->tmps.var_int, NULL, NULL);

						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA SUDU INT");
						iListCreateAppend(tools->iList, I_FLOAT2R2EINTS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA DOUBLE");
						iListCreateAppend(tools->iList, I_INT2FLOATS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: PUSH VAR");
						iListCreateAppend(tools->iList, I_PUSHS, tools->tmps.var_int, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK DIV");
						iListCreateAppend(tools->iList, I_DIVS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA INT ODSEKNUTIM");
						iListCreateAppend(tools->iList, I_FLOAT2INTS, NULL, NULL, NULL);

						break;

					case COM_DOUBLE:
						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA SUDU INT");
						iListCreateAppend(tools->iList, I_FLOAT2R2EINTS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA DOUBLE");
						iListCreateAppend(tools->iList, I_INT2FLOATS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: POP TO VAR");
						iListCreateAppend(tools->iList, I_POPS, tools->tmps.var_float, NULL, NULL);

						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA SUDU INT");
						iListCreateAppend(tools->iList, I_FLOAT2R2EINTS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA DOUBLE");
						iListCreateAppend(tools->iList, I_INT2FLOATS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: PUSH VAR");
						iListCreateAppend(tools->iList, I_PUSHS, tools->tmps.var_float, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK DIV");
						iListCreateAppend(tools->iList, I_DIVS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA INT ODSEKNUTIM");
						iListCreateAppend(tools->iList, I_FLOAT2INTS, NULL, NULL, NULL);

						break;

					case COM_STRING:
					case COM_NONE:
					default:
						errno = E_SEM_COMPA;
						DEBUG_MESSAGE("CHYBA");
						return NOT_FOUND;
				}
				return KW_INTEGER;

			case R_DIV_REAL:
				DEBUG_MESSAGE("applying rule R_DIV_REAL");

				comp = checkCompatibility(list, list->next->next);

				switch (comp) {
					case COM_INT:
						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA DOUBLE");
						iListCreateAppend(tools->iList, I_INT2FLOATS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: POP TO VAR");
						iListCreateAppend(tools->iList, I_POPS, tools->tmps.var_int, NULL, NULL);

						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA DOUBLE");
						iListCreateAppend(tools->iList, I_INT2FLOATS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: PUSH VAR");
						iListCreateAppend(tools->iList, I_PUSHS, tools->tmps.var_int, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK DIV");
						iListCreateAppend(tools->iList, I_DIVS, NULL, NULL, NULL);

						break;

					case COM_INT_DOUBLE:
						DEBUG_MESSAGE("instr: POP TO VAR");
						iListCreateAppend(tools->iList, I_POPS, tools->tmps.var_float, NULL, NULL);

						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA DOUBLE");
						iListCreateAppend(tools->iList, I_INT2FLOATS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: PUSH VAR");
						iListCreateAppend(tools->iList, I_PUSHS, tools->tmps.var_float, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK DIV");
						iListCreateAppend(tools->iList, I_DIVS, NULL, NULL, NULL);

						break;

					case COM_DOUBLE_INT:
						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA DOUBLE");
						iListCreateAppend(tools->iList, I_INT2FLOATS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK DIV");
						iListCreateAppend(tools->iList, I_DIVS, NULL, NULL, NULL);

						break;

					case COM_DOUBLE:
						DEBUG_MESSAGE("instr: STACK DIV");
						iListCreateAppend(tools->iList, I_DIVS, NULL, NULL, NULL);

						break;

					case COM_STRING:
					case COM_NONE:
					default:
						errno = E_SEM_COMPA;
						DEBUG_MESSAGE("CHYBA");
						return NOT_FOUND;
				}
				return KW_DOUBLE;

			case R_FCE:
				DEBUG_MESSAGE("applying rule R_FCE");

				if (inFceTop(tools->inFceStack)->paramsChecked == inFceTop(tools->inFceStack)->paramsCount) {
					DEBUG_MESSAGE("instr: CALL FCE");
					iListCreateAppend(tools->iList, I_CALL, list->term.token->val.identifierHT, NULL, NULL);

					inFcePop(tools->inFceStack);
					return list->term.token->val.identifierHT->type;
				}
				errno = E_SEM_COMPA;
				return NOT_FOUND;

			case R_ID:
				DEBUG_MESSAGE("applying rule R_ID");
				DEBUG_MESSAGE("instr: PUSH ID");

				switch (list->term.token->state) {
					case S_DOUBLE:
					{
						char buff[20];
						snprintf(buff, 19, "%g", list->term.token->val.doubleValue);

						htab_item_t *newConstant = htab_add_constant(stStackTop(tools->symtableStack), buff, KW_DOUBLE);

						iListCreateAppend(tools->iList, I_PUSHS, newConstant, NULL, NULL);
						return KW_DOUBLE;
					}

					case S_INT:
					{
						char buff[20];
						snprintf(buff, 19, "%ld", list->term.token->val.intValue);

						htab_item_t *newConstant = htab_add_constant(stStackTop(tools->symtableStack), buff, KW_INTEGER);

						iListCreateAppend(tools->iList, I_PUSHS, newConstant, NULL, NULL);
						return KW_INTEGER;
					}

					case S_STRING:
					{
						htab_item_t *newConstant = htab_add_constant(stStackTop(tools->symtableStack), list->term.token->val.stringValue, KW_STRING);

						iListCreateAppend(tools->iList, I_PUSHS, newConstant, NULL, NULL);
						return KW_STRING;
					}

					case S_IDENTIFIER_HT:
					{
						iListCreateAppend(tools->iList, I_PUSHS, list->term.token->val.identifierHT, NULL, NULL);
						return list->term.token->val.identifierHT->type;
					}
					default:
						break;
				}

			case R_MINUS:
				DEBUG_MESSAGE("applying rule R_MINUS");

				comp = checkCompatibility(list, list->next->next);

				switch (comp) {
					case COM_INT:
						DEBUG_MESSAGE("instr: STACK SUB");
						iListCreateAppend(tools->iList, I_SUBS, NULL, NULL, NULL);

						return KW_INTEGER;

					case COM_INT_DOUBLE:
						DEBUG_MESSAGE("instr: POP TO VAR");
						iListCreateAppend(tools->iList, I_POPS, tools->tmps.var_float, NULL, NULL);

						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA DOUBLE");
						iListCreateAppend(tools->iList, I_INT2FLOATS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: PUSH VAR");
						iListCreateAppend(tools->iList, I_PUSHS, tools->tmps.var_float, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK SUB");
						iListCreateAppend(tools->iList, I_SUBS, NULL, NULL, NULL);

						return KW_DOUBLE;

					case COM_DOUBLE_INT:
						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA DOUBLE");
						iListCreateAppend(tools->iList, I_INT2FLOATS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK SUB");
						iListCreateAppend(tools->iList, I_SUBS, NULL, NULL, NULL);

						return KW_DOUBLE;

					case COM_DOUBLE:
						DEBUG_MESSAGE("instr: STACK SUB");
						iListCreateAppend(tools->iList, I_SUBS, NULL, NULL, NULL);

						return KW_DOUBLE;

					case COM_STRING:
					case COM_NONE:
					default:
						errno = E_SEM_COMPA;
						DEBUG_MESSAGE("CHYBA");
						return NOT_FOUND;
				}

			case R_PLUS:
				DEBUG_MESSAGE("applying rule R_PLUS");

				comp = checkCompatibility(list, list->next->next);

				switch (comp) {
					case COM_INT:
						DEBUG_MESSAGE("instr: STACK ADD");
						iListCreateAppend(tools->iList, I_ADDS, NULL, NULL, NULL);

						return KW_INTEGER;

					case COM_INT_DOUBLE:
						DEBUG_MESSAGE("instr: POP TO VAR");
						iListCreateAppend(tools->iList, I_POPS, tools->tmps.var_float, NULL, NULL);

						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA DOUBLE");
						iListCreateAppend(tools->iList, I_INT2FLOATS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: PUSH VAR");
						iListCreateAppend(tools->iList, I_PUSHS, tools->tmps.var_float, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK ADD");
						iListCreateAppend(tools->iList, I_ADDS, NULL, NULL, NULL);

						return KW_DOUBLE;

					case COM_DOUBLE_INT:
						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA DOUBLE");
						iListCreateAppend(tools->iList, I_INT2FLOATS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK ADD");
						iListCreateAppend(tools->iList, I_ADDS, NULL, NULL, NULL);

						return KW_DOUBLE;

					case COM_DOUBLE:
						DEBUG_MESSAGE("instr: STACK ADD");
						iListCreateAppend(tools->iList, I_ADDS, NULL, NULL, NULL);

						return KW_DOUBLE;

					case COM_STRING:
						DEBUG_MESSAGE("instr: POP TO VAR1");
						iListCreateAppend(tools->iList, I_POPS, tools->tmps.var_string1, NULL, NULL);

						DEBUG_MESSAGE("instr: POP TO VAR2");
						iListCreateAppend(tools->iList, I_POPS, tools->tmps.var_string2, NULL, NULL);

						DEBUG_MESSAGE("instr: CONCAT VAR1 VAR2 ULOZ DO VAR3");
						iListCreateAppend(tools->iList, I_CONCAT, tools->tmps.var_string3, tools->tmps.var_string2, tools->tmps.var_string1);

						DEBUG_MESSAGE("instr: PUSH VAR3");
						iListCreateAppend(tools->iList, I_PUSHS, tools->tmps.var_string3, NULL, NULL);

						return KW_STRING;

					case COM_NONE:
					default:
						errno = E_SEM_COMPA;
						DEBUG_MESSAGE("CHYBA");
						return NOT_FOUND;
				}

			case R_MUL:
				DEBUG_MESSAGE("applying rule R_MUL");

				comp = checkCompatibility(list, list->next->next);

				switch (comp) {
					case COM_INT:
						DEBUG_MESSAGE("instr: STACK MUL");
						iListCreateAppend(tools->iList, I_MULS, NULL, NULL, NULL);

						return KW_INTEGER;

					case COM_INT_DOUBLE:
						DEBUG_MESSAGE("instr: POP TO VAR");
						iListCreateAppend(tools->iList, I_POPS, tools->tmps.var_float, NULL, NULL);

						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA DOUBLE");
						iListCreateAppend(tools->iList, I_INT2FLOATS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: PUSH VAR");
						iListCreateAppend(tools->iList, I_PUSHS, tools->tmps.var_float, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK MUL");
						iListCreateAppend(tools->iList, I_MULS, NULL, NULL, NULL);

						return KW_DOUBLE;

					case COM_DOUBLE_INT:
						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA DOUBLE");
						iListCreateAppend(tools->iList, I_INT2FLOATS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK MUL");
						iListCreateAppend(tools->iList, I_MULS, NULL, NULL, NULL);

						return KW_DOUBLE;

					case COM_DOUBLE:
						DEBUG_MESSAGE("instr: STACK MUL");
						iListCreateAppend(tools->iList, I_MULS, NULL, NULL, NULL);

						return KW_DOUBLE;

					case COM_STRING:
					case COM_NONE:
					default:
						errno = E_SEM_COMPA;
						DEBUG_MESSAGE("CHYBA");
						return NOT_FOUND;
				}

			default:
				return NOT_FOUND;
		}

	}
	else {
		switch (rule.value) {
			case R_RE_G:
				DEBUG_MESSAGE("applying rule R_RE_G");

				comp = checkCompatibility(list, list->next->next);

				switch (comp) {
					case COM_INT:
						DEBUG_MESSAGE("instr: STACK GREATER");
						iListCreateAppend(tools->iList, I_GTS, NULL, NULL, NULL);

						return KW_BOOLEAN;

					case COM_INT_DOUBLE:
						DEBUG_MESSAGE("instr: POP TO VAR");
						iListCreateAppend(tools->iList, I_POPS, tools->tmps.var_float, NULL, NULL);

						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA DOUBLE");
						iListCreateAppend(tools->iList, I_INT2FLOATS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: PUSH VAR");
						iListCreateAppend(tools->iList, I_PUSHS, tools->tmps.var_float, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK GREATER");
						iListCreateAppend(tools->iList, I_GTS, NULL, NULL, NULL);

						return KW_BOOLEAN;

					case COM_DOUBLE_INT:
						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA DOUBLE");
						iListCreateAppend(tools->iList, I_INT2FLOATS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK GREATER");
						iListCreateAppend(tools->iList, I_GTS, NULL, NULL, NULL);

						return KW_BOOLEAN;

					case COM_DOUBLE:
						DEBUG_MESSAGE("instr: STACK GREATER");
						iListCreateAppend(tools->iList, I_GTS, NULL, NULL, NULL);

						return KW_BOOLEAN;

					case COM_STRING:
						DEBUG_MESSAGE("instr: STACK GREATER");
						iListCreateAppend(tools->iList, I_GTS, NULL, NULL, NULL);

						return KW_BOOLEAN;

					case COM_NONE:
					default:
						errno = E_SEM_COMPA;
						DEBUG_MESSAGE("CHYBA");
				}
				break;

			case R_RE_L:
				DEBUG_MESSAGE("applying rule R_RE_L");

				comp = checkCompatibility(list, list->next->next);

				switch (comp) {
					case COM_INT:
						DEBUG_MESSAGE("instr: STACK LESS");
						iListCreateAppend(tools->iList, I_LTS, NULL, NULL, NULL);

						return KW_BOOLEAN;

					case COM_INT_DOUBLE:
						DEBUG_MESSAGE("instr: POP TO VAR");
						iListCreateAppend(tools->iList, I_POPS, tools->tmps.var_float, NULL, NULL);

						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA DOUBLE");
						iListCreateAppend(tools->iList, I_INT2FLOATS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: PUSH VAR");
						iListCreateAppend(tools->iList, I_PUSHS, tools->tmps.var_float, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK LESS");
						iListCreateAppend(tools->iList, I_LTS, NULL, NULL, NULL);

						return KW_BOOLEAN;

					case COM_DOUBLE_INT:
						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA DOUBLE");
						iListCreateAppend(tools->iList, I_INT2FLOATS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK LESS");
						iListCreateAppend(tools->iList, I_LTS, NULL, NULL, NULL);

						return KW_BOOLEAN;

					case COM_DOUBLE:
						DEBUG_MESSAGE("instr: STACK LESS");
						iListCreateAppend(tools->iList, I_LTS, NULL, NULL, NULL);

						return KW_BOOLEAN;

					case COM_STRING:
						DEBUG_MESSAGE("instr: STACK LESS");
						iListCreateAppend(tools->iList, I_LTS, NULL, NULL, NULL);

						return KW_BOOLEAN;

					case COM_NONE:
					default:
						errno = E_SEM_COMPA;
						DEBUG_MESSAGE("CHYBA");
				}
				break;

			case R_RE_E:
				DEBUG_MESSAGE("applying rule R_RE_E");

				comp = checkCompatibility(list, list->next->next);

				switch (comp) {
					case COM_INT:
						DEBUG_MESSAGE("instr: STACK EQUAL");
						iListCreateAppend(tools->iList, I_EQS, NULL, NULL, NULL);

						return KW_BOOLEAN;

					case COM_INT_DOUBLE:
						DEBUG_MESSAGE("instr: POP TO VAR");
						iListCreateAppend(tools->iList, I_POPS, tools->tmps.var_float, NULL, NULL);

						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA DOUBLE");
						iListCreateAppend(tools->iList, I_INT2FLOATS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: PUSH VAR");
						iListCreateAppend(tools->iList, I_PUSHS, tools->tmps.var_float, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK EQUAL");
						iListCreateAppend(tools->iList, I_EQS, NULL, NULL, NULL);

						return KW_BOOLEAN;

					case COM_DOUBLE_INT:
						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA DOUBLE");
						iListCreateAppend(tools->iList, I_INT2FLOATS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK EQUAL");
						iListCreateAppend(tools->iList, I_EQS, NULL, NULL, NULL);

						return KW_BOOLEAN;

					case COM_DOUBLE:
						DEBUG_MESSAGE("instr: STACK EQUAL");
						iListCreateAppend(tools->iList, I_EQS, NULL, NULL, NULL);

						return KW_BOOLEAN;

					case COM_STRING:
						DEBUG_MESSAGE("instr: STACK EQUAL");
						iListCreateAppend(tools->iList, I_EQS, NULL, NULL, NULL);

						return KW_BOOLEAN;

					case COM_NONE:
					default:
						errno = E_SEM_COMPA;
						DEBUG_MESSAGE("CHYBA");
				}
				break;

			case R_RE_GE:
				DEBUG_MESSAGE("applying rule R_RE_GE");

				comp = checkCompatibility(list, list->next->next);

				switch (comp) {
					case COM_INT:
						DEBUG_MESSAGE("instr: STACK LESS");
						iListCreateAppend(tools->iList, I_LTS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK NOT");
						iListCreateAppend(tools->iList, I_NOTS, NULL, NULL, NULL);

						return KW_BOOLEAN;

					case COM_INT_DOUBLE:
						DEBUG_MESSAGE("instr: POP TO VAR");
						iListCreateAppend(tools->iList, I_POPS, tools->tmps.var_float, NULL, NULL);

						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA DOUBLE");
						iListCreateAppend(tools->iList, I_INT2FLOATS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: PUSH VAR");
						iListCreateAppend(tools->iList, I_PUSHS, tools->tmps.var_float, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK LESS");
						iListCreateAppend(tools->iList, I_LTS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK NOT");
						iListCreateAppend(tools->iList, I_NOTS, NULL, NULL, NULL);

						return KW_BOOLEAN;

					case COM_DOUBLE_INT:
						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA DOUBLE");
						iListCreateAppend(tools->iList, I_INT2FLOATS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK LESS");
						iListCreateAppend(tools->iList, I_LTS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK NOT");
						iListCreateAppend(tools->iList, I_NOTS, NULL, NULL, NULL);

						return KW_BOOLEAN;

					case COM_DOUBLE:
						DEBUG_MESSAGE("instr: STACK LESS");
						iListCreateAppend(tools->iList, I_LTS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK NOT");
						iListCreateAppend(tools->iList, I_NOTS, NULL, NULL, NULL);

						return KW_BOOLEAN;

					case COM_STRING:
						DEBUG_MESSAGE("instr: STACK LESS");
						iListCreateAppend(tools->iList, I_LTS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK NOT");
						iListCreateAppend(tools->iList, I_NOTS, NULL, NULL, NULL);

						return KW_BOOLEAN;

					case COM_NONE:
					default:
						errno = E_SEM_COMPA;
						DEBUG_MESSAGE("CHYBA");
				}
				break;

			case R_RE_LE:
				DEBUG_MESSAGE("applying rule R_RE_LE");

				comp = checkCompatibility(list, list->next->next);

				switch (comp) {
					case COM_INT:
						DEBUG_MESSAGE("instr: STACK GREATER");
						iListCreateAppend(tools->iList, I_GTS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK NOT");
						iListCreateAppend(tools->iList, I_NOTS, NULL, NULL, NULL);

						return KW_BOOLEAN;

					case COM_INT_DOUBLE:
						DEBUG_MESSAGE("instr: POP TO VAR");
						iListCreateAppend(tools->iList, I_POPS, tools->tmps.var_float, NULL, NULL);

						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA DOUBLE");
						iListCreateAppend(tools->iList, I_INT2FLOATS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: PUSH VAR");
						iListCreateAppend(tools->iList, I_PUSHS, tools->tmps.var_float, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK GREATER");
						iListCreateAppend(tools->iList, I_GTS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK NOT");
						iListCreateAppend(tools->iList, I_NOTS, NULL, NULL, NULL);

						return KW_BOOLEAN;

					case COM_DOUBLE_INT:
						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA DOUBLE");
						iListCreateAppend(tools->iList, I_INT2FLOATS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK GREATER");
						iListCreateAppend(tools->iList, I_GTS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK NOT");
						iListCreateAppend(tools->iList, I_NOTS, NULL, NULL, NULL);

						return KW_BOOLEAN;

					case COM_DOUBLE:
						DEBUG_MESSAGE("instr: STACK GREATER");
						iListCreateAppend(tools->iList, I_GTS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK NOT");
						iListCreateAppend(tools->iList, I_NOTS, NULL, NULL, NULL);

						return KW_BOOLEAN;

					case COM_STRING:
						DEBUG_MESSAGE("instr: STACK GREATER");
						iListCreateAppend(tools->iList, I_GTS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK NOT");
						iListCreateAppend(tools->iList, I_NOTS, NULL, NULL, NULL);

						return KW_BOOLEAN;

					case COM_NONE:
					default:
						DEBUG_MESSAGE("ERROR");
						errno = E_SEM_COMPA;
				}
				break;

			case R_RE_NE:
				DEBUG_MESSAGE("applying rule R_RE_NE");

				comp = checkCompatibility(list, list->next->next);

				switch (comp) {
					case COM_INT:
						DEBUG_MESSAGE("instr: STACK EQUAL");
						iListCreateAppend(tools->iList, I_EQS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK NOT");
						iListCreateAppend(tools->iList, I_NOTS, NULL, NULL, NULL);

						return KW_BOOLEAN;

					case COM_INT_DOUBLE:
						DEBUG_MESSAGE("instr: POP TO VAR");
						iListCreateAppend(tools->iList, I_POPS, tools->tmps.var_float, NULL, NULL);

						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA DOUBLE");
						iListCreateAppend(tools->iList, I_INT2FLOATS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: PUSH VAR");
						iListCreateAppend(tools->iList, I_PUSHS, tools->tmps.var_float, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK EQUAL");
						iListCreateAppend(tools->iList, I_EQS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK NOT");
						iListCreateAppend(tools->iList, I_NOTS, NULL, NULL, NULL);

						return KW_BOOLEAN;

					case COM_DOUBLE_INT:
						DEBUG_MESSAGE("instr: ZAOKRUHLI STACK NA DOUBLE");
						iListCreateAppend(tools->iList, I_INT2FLOATS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK EQUAL");
						iListCreateAppend(tools->iList, I_EQS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK NOT");
						iListCreateAppend(tools->iList, I_NOTS, NULL, NULL, NULL);

						return KW_BOOLEAN;

					case COM_DOUBLE:
						DEBUG_MESSAGE("instr: STACK EQUAL");
						iListCreateAppend(tools->iList, I_EQS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK NOT");
						iListCreateAppend(tools->iList, I_NOTS, NULL, NULL, NULL);

						return KW_BOOLEAN;

					case COM_STRING:
						DEBUG_MESSAGE("instr: STACK EQUAL");
						iListCreateAppend(tools->iList, I_EQS, NULL, NULL, NULL);

						DEBUG_MESSAGE("instr: STACK NOT");
						iListCreateAppend(tools->iList, I_NOTS, NULL, NULL, NULL);

						return KW_BOOLEAN;

					case COM_NONE:
					default:
						DEBUG_MESSAGE("ERROR");
						errno = E_SEM_COMPA;
						break;
				}
				break;

			default:
				return NOT_FOUND;
		}
	}
	return NOT_FOUND;
}
