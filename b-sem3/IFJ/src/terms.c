/**
 * project: Implementace překladače imperativního jazyka IFJ17
 * authors: Martin Hošala (xhosal00), Peter Kubov (xkubov06), Tomáš Kukaň (xkukan00), Vladimír Dušek (xdusek27)
 * ------------------------------------
 * @Author	Peter Kubov (xkubov06@stud.fit.vutbr.cz)
 * @date	November, 2017
 * @brief	Terminals and non-terminals
 *
 * Module provides implementation of non-terminals and terminals used by syntax/semantic analysis.
 */

#include "scanner.h"
#include "terms.h"

TTermVal getKeyWordTerm(tKeyWord kw)
{
	static int kwToToken[] = {NULL_TERM, T_AS, T_ASC, T_DECLARE, T_DIM, T_DO, T_DOUBLE, T_ELSE, T_END, T_CHR, T_FUNCTION, T_IF, T_INPUT, T_INTEGER, T_LENGTH, T_LOOP, T_PRINT, T_RETURN, T_SCOPE, T_STRING, T_SUBSTR, T_THEN, T_WHILE, T_AND, T_BOOLEAN, T_CONTINUE, T_ELSIF, T_EXIT, T_FALSE, T_FOR, T_NEXT, T_NOT, T_OR, T_SHARED, T_STATIC, T_TRUE};

	return kwToToken[kw <= KW_TRUE ? kw:0];
}

TTermVal getTerm(tToken *token) 
{	
	switch (token->state) {
	case S_EOL:
		return T_EOL;
	case S_EOF:
		return T_EOF;
	case S_INT:
		return T_INTVAL;
	case S_DOUBLE:
		return T_DOUBLEVAL;
	case S_STRING:
		return T_STRINGVAL;
	case S_COMMA:
		return T_COMMA;
	case S_EQUAL_ASSIGN:
		return T_ASSIGN;
	case S_SEMICOLOM:
		return T_SEMMICOL;
	case S_IDENTIFIER:
		return T_IDENTIFIER;
	case S_IDENTIFIER_HT:
		if (token->val.identifierHT->is_function)
			return T_FUNID;
		return T_IDENTIFIER_HT;
	case S_OPEN_BRACKET:

		return T_OPENBRACK;
	case S_CLOSE_BRACKET:	
		return T_CLOSEBRACK;
	case S_KEY_WORD:

		return getKeyWordTerm(token->val.keyWord);
	default:
		return NULL_TERM;
	}
}
