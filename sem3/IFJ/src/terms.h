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

#ifndef _IFJ_42_TERMS_H
#define _IFJ_42_TERMS_H

#include "scanner.h"

/// Type representing nonterminals value
typedef enum {
	NULL_NTERM, /// invalid non-terminal
	NT_DEFINE, /// function definition non-terminal
	NT_DATATYPE, /// datatype expectation non-terminal
	NT_ELSE_STATEMENTS, /// non-terminal specifying that "else" part of condition is expected
	NT_FOR_STATEMENTS, /// non-terminal specifying that statements in for loop are checked (used for exit, continue keywords)
	NT_FUNCTIONS, /// non-terminal specifying function analysis is next
	NT_IF_STATEMENT, /// non-terminal specifying that selection is beying analysed (else/elsif migh be option now)
	NT_INITIALIZATION, /// non-terminal specifying that datatype initialization is next
	NT_NEXT_EXPR, /// non-terminal specifying that print has next expression to be printed
	NT_PARAMETER, // non-terminal specifying hat parameter declaration is required 
	NT_PARAMETERS, /// non-terminal specifying that function parameters are being analysed for declaration
	NT_NEXTPARAMETER, /// non-terminal specifying that next parameter is available in function parameter declaration
	NT_DEF_PARAMETER, /// non-terminal specifying that parameter definition is required
	NT_DEF_PARAMETERS, /// non-terminal specifying that function parameters are being analysed for definition
	NT_DEF_NEXTPARAMETER, /// non-terminal specifying that next parameter is available in function parameter definition
	NT_PROGRAM, /// non-terminal specifying that program as whole part is being parsed
	NT_STATEMENT, /// non-terminal specifying that statement is required as next tokens set
	NT_STATEMENTS,	/// non-terminal specifying that statements are required
	NT_WHILE_STATEMENT, /// non-terminal specifying that statements in while loop are checked (used for exit, continue keywords)
	NT_GL_INITIALIZATION, /// used for global extension
	NT_CONST_VAL, /// used for global extension
	NT_COUNT /// number of non-terminals
} TNTermVal;

/// Type representing nonterminals value
typedef enum {
	NULL_TERM, /// invalid terminal
	T_AND, /// keyword And
	T_AS, /// keyword As
	T_ASC, /// build-in function Asc
	T_ASSIGN, /// character =
	T_BOOLEAN, /// keyword Bool
	T_CHR, /// build-in function Chr
	T_CLOSEBRACK, /// character )
	T_COMMA, /// character ,
	T_CONTINUE, /// keyword Continue
	T_DECLARE, /// keyword Declare
	T_DIM, /// keyword Dim
	T_DO, /// keyword Do
	T_DOUBLE, /// keyword Double
	T_ELSE, /// keyword Else
	T_ELSIF, /// keyword Elsif
	T_END, /// keyword End
	T_EOF, /// state of end of file
	T_EOL, /// end of line character
	T_EXIT, /// keyword Exit
	T_FALSE, /// keyword False
	T_FOR, /// keyword Or
	T_FUNCTION, /// keyword Function
	T_FUNID, /// pointer to item of symbolic table that is representing function
	T_IDENTIFIER, /// string representing name of identifier
	T_IDENTIFIER_HT, /// pointer into symbol table on item that is variable
	T_IF, /// keyword If
	T_INPUT, /// keyword Input
	T_INTEGER, /// Keyword Integer
	T_INTVAL, /// constat integer value
	T_STRINGVAL,  /// constant string value
	T_DOUBLEVAL, /// constant double value
	T_LENGTH, /// build-in function length
	T_LOOP, /// keyword Loop
	T_NEXT, /// keyword Next
	T_NOT, /// keyword Not
	T_OPENBRACK, /// character (
	T_OR, /// keyword Or
	T_PRINT, /// keyword Print
	T_RETURN, /// keyword Return
	T_SCOPE, /// keyword Scope
	T_SEMMICOL, /// character ; 
	T_SHARED, /// keyword Shared
	T_STATIC, /// keyword Static
	T_STRING, /// keyword String
	T_SUBSTR, /// build-in function Substr
	T_TERM, /// terimnal representing by precedent analysis recoginzable token
	T_THEN, /// keyword Then
	T_TO, /// keyword To
	T_TRUE, /// keyword True
	T_WHILE /// keyword While
} TTermVal;


/**
 * @brief	some terminals are mapped on keyword, function convert keyword on terminal
 * @ingroup parser
 *
 * @return	value representing terminal on succes, NULL_TERM otherwise
 */
TTermVal getKeyWordTerm(tKeyWord kw);


/**
 * @brief	every terminal is mapped on token's state - function converts token on terminal
 * @ingroup parser
 *
 * @return	value representing terminal on succes, NULL_TERM otherwise
 */
TTermVal getTerm(tToken *token); 

#endif
