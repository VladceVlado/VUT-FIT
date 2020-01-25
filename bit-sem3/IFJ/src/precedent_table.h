/**
 * project: Implementace překladače imperativního jazyka IFJ17
 * authors: Martin Hošala (xhosal00), Peter Kubov (xkubov06), Tomáš Kukaň (xkukan00), Vladimír Dušek (xdusek27)
 * ------------------------------------
 * @author Martin Hošala (xhosal00)
 */

/**
 * @file precedent_table.h
 * @brief Contains precedent table which is used to decide what to do based on input token and most top precedentStack
 *        token.
 */

#ifndef IFJ17_PRECEDENT_TABLE_H
#define IFJ17_PRECEDENT_TABLE_H

/// "Rules"
typedef enum {
	J_NONE, ///< There in nothing to do for the given token combination
	J_PUSH, ///< Input token should be pushed to the precedentStack
	J_SHIFT, ///< Input token should be shifted to the precedentStack
	J_REDUCE ///< Top part of the stack should be reduced

} tJob;

/// Tokens considered terminals (valid in the expression).
typedef enum {
	T_PLUS, ///< Operators with the lowest priority: "+", "-"
	T_MUL, ///< Operators with the highest priority: "*", "/"
	T_BRC_L, ///< Left brace
	T_BRC_R, ///< Right brace
	T_FCE, ///< Function identifier
	T_ID, ///< Variable identifier
	T_DOLLAR, ///< End of input / no token on the top of the precedentStack
	T_COMMA, ///< Comma
	T_REL, ///< Relation operators: <, >, <= ...
	T_DIV_INT, /// Operator with medium priority: "\"

	T_END
} tPrecedentTableTerm;

/**
 * @brief Matrix containing space for all "rules".
 */
typedef tJob tPTable[T_END][T_END];

/**
 * @brief Fulfils the given table with the appropriate "rules".
 * @param table Table pointer
 */
void ptInit(tPTable *table);


#endif //IFJ17_PRECEDENT_TABLE_H
