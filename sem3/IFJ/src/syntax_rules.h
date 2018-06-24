/**
 * project: Implementace překladače imperativního jazyka IFJ17
 * authors: Martin Hošala (xhosal00), Peter Kubov (xkubov06), Tomáš Kukaň (xkukan00), Vladimír Dušek (xdusek27)
 * ------------------------------------
 * @Author	Peter Kubov (xkubov06@stud.fit.vutbr.cz)
 * @date	November, 2017
 * @brief	Rules for syntax analysis for IFJ project.
 *
 * Module provides implementation of rules of LL grammar.
 */

#ifndef _IFJ42_SYNTAX_RULES_
#define _IFJ42_SYNTAX_RULES_

#include "parser_essentials.h"

/**
 * @brief	Type representing rule of LL grammar.
 * @ingroup	parser
 *
 * Every rule is created as independent function which takes
 * Syntax stack as argument and changes stack as required for
 * correct syntax analysation.
 */
typedef void (*TRuleFunction)(TStack);

/**
 * @brief Returns rule to be applied based on non-terminal on top
 * of the syntax stack and token that is at input.
 * @ingroup	parser
 *
 * @param	nt	Non-terminal on top of the syntax stack.
 * @param	tokenState	Represents state of the token that specifies type of the token at input.
 *
 * @return	On success returns syntax rule NULL otherwise.
 */
TRuleFunction getRule(TNTermVal nt, TTermVal tokenState);

#endif
