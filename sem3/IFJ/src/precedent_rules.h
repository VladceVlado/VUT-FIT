/**
 * project: Implementace překladače imperativního jazyka IFJ17
 * authors: Martin Hošala (xhosal00), Peter Kubov (xkubov06), Tomáš Kukaň (xkukan00), Vladimír Dušek (xdusek27)
 * ------------------------------------
 * @author Martin Hošala (xhosal00)
 */

/**
 * @file precedent_rules.h
 * @brief Contains declaration of the precedent analysis syntactic rules.
 *
 * Also contains functions to "use" this rules, semantically check the correctness of the expression and to generate
 * appropriate 3AK.
 *
 */

#include <stdbool.h>

#include "precedent_tools.h"
#include "precedent_stack.h"

#ifndef IFJ17_PRECEDENT_RULES_H
#define IFJ17_PRECEDENT_RULES_H

/**
 * @brief Types of the compatibility of the 2 noTerminals.
 */
typedef enum {
	COM_NONE, ///< No implicit conversion exists
	COM_DOUBLE, ///< Both noTerminals are real numbers
	COM_DOUBLE_INT, ///< First noTerminal is a real number, second an integer number, implicit conversion exists.
	COM_INT_DOUBLE, ///< First noTerminal is an integer number, second a real number, implicit conversion exists.
	COM_INT, ///< Both noTerminals are integer numbers
	COM_STRING ///< Both noTerminals are strings

} tCompatibility;

/**
 * @brief The rule parts values.
 */
typedef enum {
	RP_NO_TERM = 1,
	RP_ID = 2,
	RP_PLUS = 3,
	RP_MINUS = 4,
	RP_MUL = 5,
	RP_DIV_INT = 6,
	RP_DIV_REAL = 7,
	RP_L_BRACKET = 8,
	RP_R_BRACKET = 9,
	RP_FCE = 10,

	RP_RE_G = 2,
	RP_RE_L = 3,
	RP_RE_E = 4,
	RP_RE_GE = 5,
	RP_RE_LE = 6,
	RP_RE_NE = 7

} tRulePartValue;

/**
 * @brief The rule codes.
 */
typedef enum {
	R_BRACKETS = 819, ///< E -> (E)
	R_ID = 2, ///< E -> variable
	R_PLUS = 131, ///< E -> E + E
	R_MINUS = 141, ///< E -> E - E
	R_MUL = 151,  ///< E -> E * E
	R_DIV_INT = 161, ///< E -> E \ E
	R_DIV_REAL = 171, ///< E -> E / E
	R_FCE = 10,  ///< E -> function(...)

	R_RE_G = 121, ///< E -> E > E
	R_RE_L = 131, ///< E -> E < E
	R_RE_E = 141, ///< E -> E = E
	R_RE_GE = 151, ///< E -> E >= E
	R_RE_LE = 161, ///< E -> E <= E
	R_RE_NE = 171 ///< E -> E <> E

} tRuleValue;

/**
 * @brief The syntactic rules.
 *
 * Every rule consist of the value, which is sequence of the rule parts, and the information if a relation operator
 * was is part of the value.
 */
typedef struct {
	tRuleValue value;
	bool isRelation;

} tRule;

/**
 * @brief Serves to check correctness and apply syntactic and semantic rules.
 *
 * Checks if the given rule exists. If so, semantically checks the given list to see if the application of the rule will
 * be correct. If so, generates appropriate 3AK.
 *
 * @param rule The given rule code
 * @param tools Given precedentTools
 * @param list The given list
 * @return Type of result after the rule application
 */
tKeyWord applyRule(tRule rule, tPrTools *tools, tPrecElement *list);

/**
 * @brief Serves to generate 3AK to cast element on the top of the stack.
 *
 * This functions is used when an expression contains function call and the input token is no longer part of actual
 * function parameter being parsed.
 *
 * Checks the possibility to cast from actual type to wanted type. Wanted type is given by the given tools, in the
 * inFceStack most top element.
 *
 * If possible, generates 3AK to make this cast happen.
 *
 * @param noTerm The given noTerminal.
 * @param tools Given precedentTools
 * @return True if the cast is possible
 */
bool castTopGen(tPrecElement *noTerm, tPrTools *tools);

/**
 * @brief Serves to generate 3AK to cast element on the top of the stack.
 *
 * This function is used, when the expression has been successfully syntactically and semantically checked to check if
 * the evaluated expression type is compatible with the wanted type.
 *
 * Checks the possibility to cast from actual type to wanted type. Wanted type is given by the given tools and actual
 * type is the type of the most top noTerminal in the precedent stack, also given by the given tools.
 *
 * If possible, generates 3AK to make this cast happen.
 *
 * @param tools Given precedentTools.
 * @return True if the cast is possible.
 */
bool finalCastGen(tPrTools *tools);

#endif //IFJ17_PRECEDENT_RULES_H
