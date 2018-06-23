/**
 * project: Implementace překladače imperativního jazyka IFJ17
 * authors: Martin Hošala (xhosal00), Peter Kubov (xkubov06), Tomáš Kukaň (xkukan00), Vladimír Dušek (xdusek27)
 * ------------------------------------
 * @author	Vladimír Dušek (xdusek27)
 * @date	November, 2017
 * @brief	Lexical analysis.
 * @file	scanner.h
 */


#ifndef _SCANNER_H
#define _SCANNER_H


#include "keywords.h"
#include "symtable.h"
#include "symtable_stack.h"


/**
 * @brief	Enumerate type FSMstate for representing states of final state machine.
 */
typedef enum {
	FSM_START,
	FSM_LINE_COMMENT,
	FSM_SUSP_BEGIN_BLOCK_COMMENT,
	FSM_BLOCK_COMMENT,
	FSM_SUSP_END_BLOCK_COMMENT,
	FSM_ERROR_UNTERM_BLOCK_COMMENT,

	FSM_LEXEME_START,
	FSM_EOF,
	FSM_PLUS,
	FSM_MINUS,
	FSM_MULTIPLICATION,
	FSM_DOUBLE_DIVISION,
	FSM_INT_DIVISION,
	FSM_BIGGER_EQUAL,
	FSM_BIGGER,
	FSM_NOT_EQUAL,
	FSM_LESS_EQUAL,
	FSM_LESS,
	FSM_EQUAL_ASSIGN,
	FSM_SEMICOLOM,
	FSM_COMMA,
	FSM_OPEN_BRACKET,
	FSM_CLOSE_BRACKET,
	FSM_EOL,

	FSM_SUSP_STRING,
	FSM_IN_STRING,
	FSM_END_STRING,
	FSM_ERROR_UNTERM_STRING,
	FSM_ERROR_UNKNOWN_LEXEME,
	FSM_ERROR_INVALID_ESCAPE_SEQ,

	FSM_IDENT_KEY_WORD,
	FSM_IDENT_KEY_WORD_END,

	FSM_NUMBER,
	FSM_NUM_DOT,
	FSM_NUM_DOT_DIGIT,
	FSM_NUM_E,
	FSM_NUM_E_PM,
	FSM_NUM_E_DIGIT,
	FSM_INT,
	FSM_ERROR_INT,
	FSM_DOUBLE,
	FSM_ERROR_DOUBLE
} FSMstate;


/**
 * @brief	Enumerate type tState for representing states of tokens.
 */
typedef enum {
	S_START,                 // 00 ... START

	/* Mathematical operators */
	S_PLUS,                  // 01 ... +
	S_MINUS,                 // 02 ... -
	S_MULTIPLICATION,        // 03 ... *
	S_DOUBLE_DIVISION,       // 04 ... /
	S_INT_DIVISION,          // 05 ... \ .

	/* Relational and assign operators */
	S_BIGGER_EQUAL,          // 06 ... >=
	S_BIGGER,                // 07 ... >
	S_NOT_EQUAL,             // 08 ... <>
	S_LESS_EQUAL,            // 09 ... <=
	S_LESS,                  // 10 ... <
	S_EQUAL_ASSIGN,          // 11 ... =

	/* Semicolom, comma, brackets */
	S_SEMICOLOM,             // 12 ... ;
	S_COMMA,                 // 13 ... ,
	S_OPEN_BRACKET,          // 14 ... (
	S_CLOSE_BRACKET,         // 15 ... )
	S_EOL,                   // 16 ... \n

	/* Identifier, (reserved) key word */
	S_IDENTIFIER_HT,         // 17 ... identifier, already in hash table
	S_IDENTIFIER,            // 18 ... identifier
	S_KEY_WORD,              // 19 ... keyWord

	/* String, int, double, bool */
	S_STRING,                // 20 ... !"string"
	S_INT,                   // 21 ... 666
	S_DOUBLE,                // 22 ... 42.666e-2
	//S_BOOL,                // Only for extension

	S_ERROR,                 // 23 ... Uknown lex, error with comm, double, string, mem alloc
	S_EOF                    // 24 ... EOF
} tState;


/**
 * @brief	Structure tToken for representing token.
 */
typedef struct {
	tState state;
	union {
		long intValue;
		double doubleValue;
		char *stringValue;
		tKeyWord keyWord;
		char *identifier;
		htab_item_t *identifierHT;
		char *errorMsg;
		// bool boolValue;
	} val;
} tToken;


/**
 * @brief      Determines if c is operator, semicolom or bracket.
 *
 * @param      c	Character for determine.
 *
 * @return     True if operator, False otherwise.
 */
bool isOper(int c);


/**
 * @brief      Allocate memory for new token.
 *
 * @return     Return allocated token.
 */
tToken *newToken(void);


/**
 * @brief      Gets token.
 *
 * @param      fr		Pointer to the input stream.
 * @param      token	Token for return.
 * @param      stack	Stack of symtables of identifiers.
 *
 * @return     The token in pointer.
 * 			   0 if everything ok
 * 			   1 if memory allocation failed
 * 			   2 if unterminated comment
 * 			   3 if unterminated string
 * 			   4 if unknown lexeme
 * 			   5 if empty decimal or exponent part of double
 */
int getToken(FILE *fr, tToken *token, STStack *stack);


/**
 * @brief      Ungets token.
 *
 * @param      token	Token for unget.
 */
void ungetToken(tToken **token);


/**
 * @brief      Free dynamically allocated memory for identifier in the token.
 *
 * @param      token	Token for free.
 */
void freeToken(tToken *token);


#endif
