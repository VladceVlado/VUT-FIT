/**
 * project: Implementace překladače imperativního jazyka IFJ17
 * authors: Martin Hošala (xhosal00), Peter Kubov (xkubov06), Tomáš Kukaň (xkukan00), Vladimír Dušek (xdusek27)
 * ------------------------------------
 * @author  Vladimír Dušek (xdusek27)
 * @date	November, 2017
 * @brief	Lexical analysis.
 * @file    scanner.c
 */


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "scanner.h"
#include "error_ifj.h"


/**
 * Number of (reserved) key words.
 */
#define NUM_KEY_WORDS 35

/**
 * Default size of buffer.
 */
#define SIZE_OF_BUFFER 100

/**
 * Indexes to error array.
 */
#define ERROR_MALLOC 0
#define ERROR_UNTERM_COMMENT 1
#define ERROR_UNTERM_STRING 2
#define ERROR_UNKNOWN_LEXEME 3
#define ERROR_DOUBLE 4
#define ERROR_INT 5
#define ERROR_INVALID_ESCAPE_SEQ 6


/**
 * Error array.
 */
static char *errorArray[] = {
	"memory allocation failed",
	"unterminated comment",
	"unterminated string",
	"unknown lexeme",
	"invalid format of double",
	"invalid format of integer",
	"invalid escape sequence in string"
};


/**
 * Determines if c is operator, semicolom, ampersand or bracket.
 */
bool isOper(int c)
{
	if (c == '+' || c == '-' || c == '*' || c == '/' || c == '\\' ||
		c == '>' || c == '<' || c == '=' || c == ';' || c == ','  ||
		c == '(' || c == ')' || c == '\'')
		return true;
	else
		return false;
}


/**
 * Allocate memory for new token.
 */
tToken *newToken(void)
{
	return (tToken *) malloc(sizeof(tToken));
}


/**
 * Ungets token.
 */
tToken *_IFJ17_UNGET_TOKEN = NULL;
void ungetToken(tToken **token)
{
	_IFJ17_UNGET_TOKEN =*token;
}


/**
 * Gets token.
 */
int getToken(FILE *fr, tToken *token, STStack *stack)
{
	if (_IFJ17_UNGET_TOKEN != NULL) {
		token->state = _IFJ17_UNGET_TOKEN->state;
		token->val = _IFJ17_UNGET_TOKEN->val;
		free(_IFJ17_UNGET_TOKEN);
		_IFJ17_UNGET_TOKEN = NULL;
		return E_OK;
	}
	/* Decklaration of all necessery variables */
	int i = 0, c, cNext, size = SIZE_OF_BUFFER;
	char *buffer = (char *) calloc(size, sizeof(char));
	bool FSMcon = true;
	FSMstate s = FSM_START;
	htab_item_t *tmp;

	if (buffer == NULL) {
		token->state = S_ERROR;
		token->val.errorMsg = errorArray[ERROR_MALLOC];
		return E_SYS;
	}

	/* Loading lexeme */
	while (FSMcon) {
		c = getc(fr);
		switch (s) {

		/* Start */
		case FSM_START:
			if (isspace(c) && c != '\n')
				s = FSM_START;
			else if (c == '\'')
				s = FSM_LINE_COMMENT;
			else if (c == '/')
				s = FSM_SUSP_BEGIN_BLOCK_COMMENT;
			else {
				s = FSM_LEXEME_START;
				ungetc(c, fr);
			}
		break;

		/* Line comment */
		case FSM_LINE_COMMENT:
			if (c == '\n' || c == EOF) {
				s = FSM_LEXEME_START;
				ungetc(c, fr);
			}
			else
				s = FSM_LINE_COMMENT;
		break;

		/* Suspicion of the begin of a block comment */
		case FSM_SUSP_BEGIN_BLOCK_COMMENT:
			if (c == '\'')
				s = FSM_BLOCK_COMMENT;
			else {
				s = FSM_LEXEME_START;
				ungetc(c, fr);
				ungetc('/', fr);
			}
		break;

		/* Block comment */
		case FSM_BLOCK_COMMENT:
			if (c == '\'')
				s = FSM_SUSP_END_BLOCK_COMMENT;
			else if (c == EOF) {
				s = FSM_ERROR_UNTERM_BLOCK_COMMENT;
				ungetc(c, fr);
			}
			else
				s = FSM_BLOCK_COMMENT;
		break;

		/* Suspicion of the end of a block comment */
		case FSM_SUSP_END_BLOCK_COMMENT:
			if (c == '/')
				s = FSM_START;
			else if (c == EOF) {
				s = FSM_ERROR_UNTERM_BLOCK_COMMENT;
				ungetc(c, fr);
			}
			else
				s = FSM_BLOCK_COMMENT;
		break;

		/* Beginning of a lexeme */
		case FSM_LEXEME_START:
			if (c == EOF)
				s = FSM_EOF;
			else if (c == '+')
				s = FSM_PLUS;
			else if (c == '-')
				s = FSM_MINUS;
			else if (c == '*')
				s = FSM_MULTIPLICATION;
			else if (c == '/')
				s = FSM_DOUBLE_DIVISION;
			else if (c == '\\')
				s = FSM_INT_DIVISION;
			else if (c == '>') {
				cNext = getc(fr);
				if (cNext == '=')
					s = FSM_BIGGER_EQUAL;
				else {
					ungetc(cNext, fr);
					s = FSM_BIGGER;
				}
			}
			else if (c == '<') {
				cNext = getc(fr);
				if (cNext == '>')
					s = FSM_NOT_EQUAL;
				else if (cNext == '=')
					s = FSM_LESS_EQUAL;
				else {
					ungetc(cNext, fr);
					s = FSM_LESS;
				}
			}
			else if (c == '=')
				s = FSM_EQUAL_ASSIGN;
			else if (c == ';')
				s = FSM_SEMICOLOM;
			else if (c == ',')
				s = FSM_COMMA;
			else if (c == '(')
				s = FSM_OPEN_BRACKET;
			else if (c == ')')
				s = FSM_CLOSE_BRACKET;
			else if (c == '\n')
				s = FSM_EOL;
			else if (isalpha(c) || c == '_')
				s = FSM_IDENT_KEY_WORD;
			else if (c == '!') {
				s = FSM_SUSP_STRING;
				c = getc(fr);
			}
			else if (isdigit(c))
				s = FSM_NUMBER;
			else {
				s = FSM_ERROR_UNKNOWN_LEXEME;
				c = getc(fr);
			}
			ungetc(c, fr);
		break;

		/* TOKEN STATE = EOF */
		case FSM_EOF:
			token->state = S_EOF;
			FSMcon = false;
		break;

		/* TOKEN STATE = + */
		case FSM_PLUS:
			token->state = S_PLUS;
			FSMcon = false;
		break;

		/* TOKEN STATE = - */
		case FSM_MINUS:
			token->state = S_MINUS;
			FSMcon = false;
		break;

		/* TOKEN STATE = * */
		case FSM_MULTIPLICATION:
			token->state = S_MULTIPLICATION;
			FSMcon = false;
		break;

		/* TOKEN STATE = / */
		case FSM_DOUBLE_DIVISION:
			token->state = S_DOUBLE_DIVISION;
			FSMcon = false;
		break;

		/* TOKEN STATE = \ */
		case FSM_INT_DIVISION:
			token->state = S_INT_DIVISION;
			FSMcon = false;
		break;

		/* TOKEN STATE = >= */
		case FSM_BIGGER_EQUAL:
			token->state = S_BIGGER_EQUAL;
			FSMcon = false;
		break;

		/* TOKEN STATE = > */
		case FSM_BIGGER:
			token->state = S_BIGGER;
			FSMcon = false;
		break;

		/* TOKEN STATE = <> */
		case FSM_NOT_EQUAL:
			token->state = S_NOT_EQUAL;
			FSMcon = false;
		break;

		/* TOKEN STATE = <= */
		case FSM_LESS_EQUAL:
			token->state = S_LESS_EQUAL;
			FSMcon = false;
		break;

		/* TOKEN STATE = < */
		case FSM_LESS:
			token->state = S_LESS;
			FSMcon = false;
		break;

		/* TOKEN STATE = = */
		case FSM_EQUAL_ASSIGN:
			token->state = S_EQUAL_ASSIGN;
			FSMcon = false;
		break;

		/* TOKEN STATE = ; */
		case FSM_SEMICOLOM:
			token->state = S_SEMICOLOM;
			FSMcon = false;
		break;

		/* TOKEN STATE = , */
		case FSM_COMMA:
			token->state = S_COMMA;
			FSMcon = false;
		break;

		/* TOKEN STATE = ( */
		case FSM_OPEN_BRACKET:
			token->state = S_OPEN_BRACKET;
			FSMcon = false;
		break;

		/* TOKEN STATE = ) */
		case FSM_CLOSE_BRACKET:
			token->state = S_CLOSE_BRACKET;
			FSMcon = false;
		break;

		/* TOKEN STATE = EOL */
		case FSM_EOL:
			token->state = S_EOL;
			FSMcon = false;
		break;

		/* Loading identifier or key word to the buffer */
		case FSM_IDENT_KEY_WORD:
			if (isalnum(c) || c == '_') {
				if (i >= size) {
					size = size * 2;
					buffer = (char *) realloc(buffer, size);
					if (buffer == NULL) {
						token->state = S_ERROR;
						token->val.errorMsg = errorArray[ERROR_MALLOC];
						return E_SYS;
					}
				}
				buffer[i++] = c;
			}
			else {
				s = FSM_IDENT_KEY_WORD_END;
				ungetc(c, fr);
				if (i >= size) {
					size++;
					buffer = (char *) realloc(buffer, size);
					if (buffer == NULL) {
						token->state = S_ERROR;
						token->val.errorMsg = errorArray[ERROR_MALLOC];
						return E_SYS;
					}
				}
				buffer[i] = '\0';
			}
		break;

		/* Decide if identifier or key word and save its content to token */
		case FSM_IDENT_KEY_WORD_END:
			token->val.keyWord = get_keyword_enum(buffer);
			/* Key word */
			if (token->val.keyWord != NOT_FOUND)
				token->state = S_KEY_WORD;
			else {
				for (i = 0; buffer[i] != '\0'; i++)
	 				buffer[i] = tolower(buffer[i]);
	 			/* Try to find identifier in table of identifiers */
				if ((tmp = searchID(buffer, stack)) != NULL)
				{
					token->val.identifierHT = tmp;
					token->state = S_IDENTIFIER_HT;
				}
				/* If not found, dynamically allocate */
				else
				{
					token->state = S_IDENTIFIER;
					token->val.identifier = (char *) calloc(strlen(buffer) + 1, sizeof(char));
					if (token->val.identifier == NULL) {
						free(buffer);
						token->state = S_ERROR;
						token->val.errorMsg = errorArray[ERROR_MALLOC];
						return E_SYS;
					}
					for (i = 0; buffer[i] != '\0'; i++)
  						buffer[i] = tolower(buffer[i]);
					strcpy(token->val.identifier, buffer);
				}
			}
			ungetc(c, fr);
			FSMcon = false;
		break;

		/* Suspicion of string, exclamation was found */
		case FSM_SUSP_STRING:
			if (c == '"')
				s = FSM_IN_STRING;
			else {
				ungetc(c, fr);
				s = FSM_ERROR_UNKNOWN_LEXEME;
			}
		break;

		/* Loading chars in string */
		case FSM_IN_STRING:
			if (c == EOF) {
				s = FSM_ERROR_UNTERM_STRING;
				ungetc(c, fr);
			}
			else if (c == '"')
				s = FSM_END_STRING;
			/* Loading escape sequence */
			else if (c == '\\') {
				c = getc(fr);
				if (c == '\\' || c == '"' || c == 'n' || c == 't') {
					switch (c) {
					case '\\':
						buffer[i++] = '\\';
						break;
					case '"':
						buffer[i++] = '"';
						break;
					case 'n':
						buffer[i++] = '\n';
						break;
					case 't':
						buffer[i++] = '\t';
						break;
					default:
						break;
					}
				}
				else if (isdigit(c)) {
					cNext = (c - '0') * 100;
					c = getc(fr);
					if (isdigit(c)) {
						cNext += (c - '0') * 10;
						c = getc(fr);
						if (isdigit(c)) {
							cNext += (c - '0') * 1;
							if (cNext >= 0 && cNext <= 255)
								buffer[i++] = cNext;
							else
								s = FSM_ERROR_INVALID_ESCAPE_SEQ;
						}
						else
							s = FSM_ERROR_INVALID_ESCAPE_SEQ;
					}
					else
						s = FSM_ERROR_INVALID_ESCAPE_SEQ;
				}
				else
					s = FSM_ERROR_INVALID_ESCAPE_SEQ;
			}
			else {
				if (i >= size) {
					size = size * 2;
					buffer = (char *) realloc(buffer, size);
					if (buffer == NULL) {
						token->state = S_ERROR;
						token->val.errorMsg = errorArray[ERROR_MALLOC];
						return E_SYS;
					}
				}
				buffer[i++] = c;
			}
		break;

		/* The end of string, " was found, loading string to token */
		case FSM_END_STRING:
			if (i >= size) {
				buffer = (char *) realloc(buffer, size + 1);
				if (buffer == NULL) {
					token->state = S_ERROR;
					token->val.errorMsg = errorArray[ERROR_MALLOC];
					return E_SYS;
				}
			}
			buffer[i] = '\0';
			token->state = S_STRING;
			token->val.stringValue = (char *) calloc(strlen(buffer) + 1, sizeof(char));
			if (token->val.stringValue == NULL) {
				free(buffer);
				token->state = S_ERROR;
				token->val.errorMsg = errorArray[ERROR_MALLOC];
				return E_SYS;
			}
			strcpy(token->val.stringValue, buffer);
			ungetc(c, fr);
			FSMcon = false;
		break;

		/* Number */
		case FSM_NUMBER:
			if (i >= size) {
				size = size * 2;
				buffer = (char *) realloc(buffer, size);
				if (buffer == NULL) {
					token->state = S_ERROR;
					token->val.errorMsg = errorArray[ERROR_MALLOC];
					return E_SYS;
				}
			}
			buffer[i++] = c;
			if (tolower(c) == 'e')
				s = FSM_NUM_E;
			else if (c == '.')
				s = FSM_NUM_DOT;
			else if (isdigit(c))
				s = FSM_NUMBER;
			else if (isspace(c) || isOper(c) || c == EOF) {
				s = FSM_INT;
				buffer[--i] = '\0';
				ungetc(c, fr);
			}
			else {
				ungetc(c, fr);
				s = FSM_ERROR_INT;
			}
		break;

		/* Number and then dot */
		case FSM_NUM_DOT:
			if (isdigit(c)) {
				if (i >= size) {
					size = size * 2;
					buffer = (char *) realloc(buffer, size);
					if (buffer == NULL) {
						token->state = S_ERROR;
						token->val.errorMsg = errorArray[ERROR_MALLOC];
						return E_SYS;
					}
				}
				buffer[i++] = c;
				s = FSM_NUM_DOT_DIGIT;
			}
			else {
				ungetc(c, fr);
				s = FSM_ERROR_INT;
			}
		break;

		/* Number then dot and then digit */
		case FSM_NUM_DOT_DIGIT:
			if (isdigit(c)) {
				if (i >= size) {
					size = size * 2;
					buffer = (char *) realloc(buffer, size);
					if (buffer == NULL) {
						token->state = S_ERROR;
						token->val.errorMsg = errorArray[ERROR_MALLOC];
						return E_SYS;
					}
				}
				buffer[i++] = c;
			}
			else if (tolower(c) == 'e') {
				if (i >= size) {
					size = size * 2;
					buffer = (char *) realloc(buffer, size);
					if (buffer == NULL) {
						token->state = S_ERROR;
						token->val.errorMsg = errorArray[ERROR_MALLOC];
						return E_SYS;
					}
				}
				buffer[i++] = c;
				s = FSM_NUM_E;
			}
			else if (isspace(c) || isOper(c) || c == EOF) {
				ungetc(c, fr);
				buffer[i] = '\0';
				s = FSM_DOUBLE;
			}
			else {
				ungetc(c, fr);
				s = FSM_ERROR_DOUBLE;
			}
		break;

		/* Number and then e */
		case FSM_NUM_E:
			if (isdigit(c)) {
				if (i >= size) {
					size = size * 2;
					buffer = (char *) realloc(buffer, size);
					if (buffer == NULL) {
						token->state = S_ERROR;
						token->val.errorMsg = errorArray[ERROR_MALLOC];
						return E_SYS;
					}
				}
				buffer[i++] = c;
				s = FSM_NUM_E_DIGIT;
			}
			else if (c == '+' || c == '-') {
				if (i >= size) {
					size = size * 2;
					buffer = (char *) realloc(buffer, size);
					if (buffer == NULL) {
						token->state = S_ERROR;
						token->val.errorMsg = errorArray[ERROR_MALLOC];
						return E_SYS;
					}
				}
				buffer[i++] = c;
				s = FSM_NUM_E_PM;
			}
			else {
				ungetc(c, fr);
				s = FSM_ERROR_DOUBLE;
			}
		break;

		/* Number then e and then +/- */
		case FSM_NUM_E_PM:
			if (isdigit(c)) {
				if (i >= size) {
					size = size * 2;
					buffer = (char *) realloc(buffer, size);
					if (buffer == NULL) {
						token->state = S_ERROR;
						token->val.errorMsg = errorArray[ERROR_MALLOC];
						return E_SYS;
					}
				}
				buffer[i++] = c;
				s = FSM_NUM_E_DIGIT;
			}
			else {
				ungetc(c, fr);
				s = FSM_ERROR_DOUBLE;
			}
		break;

		/* Number then e and then digit */
		case FSM_NUM_E_DIGIT:
			if (isdigit(c)) {
				if (i >= size) {
					size = size * 2;
					buffer = (char *) realloc(buffer, size);
					if (buffer == NULL) {
						token->state = S_ERROR;
						token->val.errorMsg = errorArray[ERROR_MALLOC];
						return E_SYS;
					}
				}
				buffer[i++] = c;
			}
			else if (isspace(c) || isOper(c) || c == EOF) {
				ungetc(c, fr);
				buffer[i] = '\0';
				s = FSM_DOUBLE;
			}
			else {
				ungetc(c, fr);
				s = FSM_ERROR_DOUBLE;
			}
		break;

		/* TOKEN STATE = INT */
		case FSM_INT:
			token->state = S_INT;
			token->val.intValue = strtol(buffer, NULL, 10);
			ungetc(c, fr);
			FSMcon = false;
		break;

		/* TOKEN STATE = DOUBLE */
		case FSM_DOUBLE:
			token->state = S_DOUBLE;
			token->val.doubleValue = strtod(buffer, NULL);
			ungetc(c, fr);
			FSMcon = false;
		break;

		/* Error: Unterminated block comment */
		case FSM_ERROR_UNTERM_BLOCK_COMMENT:
			token->state = S_ERROR;
			token->val.errorMsg = errorArray[ERROR_UNTERM_COMMENT];
			//ungetc(c, fr);
			FSMcon = false;
			free(buffer);
			return E_LEX;
		break;

		/* Error: Unterminated string */
		case FSM_ERROR_UNTERM_STRING:
			token->state = S_ERROR;
			token->val.errorMsg = errorArray[ERROR_UNTERM_STRING];
			//ungetc(c, fr);
			FSMcon = false;
			free(buffer);
			return E_LEX;
		break;

		/* Error: Invalid escape sequence in string */
		case FSM_ERROR_INVALID_ESCAPE_SEQ:
			token->state = S_ERROR;
			token->val.errorMsg = errorArray[ERROR_INVALID_ESCAPE_SEQ];
			//ungetc(c, fr);
			FSMcon = false;
			free(buffer);
			return E_LEX;
		break;

		/* Error: Unknown lexeme */
		case FSM_ERROR_UNKNOWN_LEXEME:
			token->state = S_ERROR;
			token->val.errorMsg = errorArray[ERROR_UNKNOWN_LEXEME];
			ungetc(c, fr);
			FSMcon = false;
			free(buffer);
			return E_LEX;
		break;

		/* Error: Invalid format of integer */
		case FSM_ERROR_INT:
			token->state = S_ERROR;
			token->val.errorMsg = errorArray[ERROR_INT];
			ungetc(c, fr);
			FSMcon = false;
			free(buffer);
			return E_LEX;
		break;

		/* Error: Invalid format of double */
		case FSM_ERROR_DOUBLE:
			token->state = S_ERROR;
			token->val.errorMsg = errorArray[ERROR_DOUBLE];
			ungetc(c, fr);
			FSMcon = false;
			free(buffer);
			return E_LEX;
		break;
	}}

	free(buffer);
	return E_OK;
}


/**
 * Free dynamically allocated memory for identifier in the token.
 */
void freeToken(tToken *token)
{
	if (token == NULL)
		return;
	if (token->state == S_IDENTIFIER || token->state == S_STRING)
		free(token->val.identifier);
	free(token);
}
