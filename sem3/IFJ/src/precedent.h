/**
 * project: Implementace překladače imperativního jazyka IFJ17
 * authors: Martin Hošala (xhosal00), Peter Kubov (xkubov06), Tomáš Kukaň (xkukan00), Vladimír Dušek (xdusek27)
 * ------------------------------------
 * @author Martin Hošala (xhosal00)
 */

/**
 * @file precedent.h
 * @brief Contains the declaration of the expression-analysing module interface.
 *
 * This module serves to parse and check an expression. Function call can also be a part of an expression.
 */

#ifndef PRECEDENT_H
#define PRECEDENT_H

#include "error_ifj.h"
#include "precedent_stack.h"
#include "precedent_tools.h"
#include "symtable_stack.h"

/**
 * @brief Serves to analyse an expression.
 *
 * Reads tokens from the input file by scanner. Checks the sequence of the coming tokens checks syntactically,
 * semantically, and generates 3AK which contains steps in pseudo assembler to evaluate the expression and store the
 * result on the top of the stack. In case of an error sets the errno to the appropriate value and despite some 3AK may
 * be generated, it is not valid.
 *
 * @param tools Some tools needed to analyse an expression, see precedent_tools.h
 * @return On success the first token that follows the evaluated expression. Random input token else.
 */
tToken *analyseExpr(tPrTools *tools);

#endif //PRECEDENT_H

