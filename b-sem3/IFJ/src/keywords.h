//
// Created by zeusko on 19/10/17.
//

#include <stdio.h>
#include <strings.h>
#include <stdbool.h>

#ifndef TABLES2_KEYWORDS_H
#define TABLES2_KEYWORDS_H

#define KEYWORDS_TABLE_LENGTH 35

/**
 * Enumerate type tKeyWord for representing key words.
 */
typedef enum {
    NOT_FOUND,
    KW_AS,
    KW_ASC,
    KW_DECLARE,
    KW_DIM,
    KW_DO,
    KW_DOUBLE,
    KW_ELSE,
    KW_END,
    KW_CHR,
    KW_FUNCTION,
    KW_IF,
    KW_INPUT,
    KW_INTEGER,
    KW_LENGTH,
    KW_LOOP,
    KW_PRINT,
    KW_RETURN,
    KW_SCOPE,
    KW_STRING,
    KW_SUBSTR,
    KW_THEN,
    KW_WHILE,
    KW_AND,
    KW_BOOLEAN,
    KW_CONTINUE,
    KW_ELSEIF,
    KW_EXIT,
    KW_FALSE,
    KW_FOR,
    KW_NEXT,
    KW_NOT,
    KW_OR,
    KW_SHARED,
    KW_STATIC,
    KW_TRUE
} tKeyWord;

/**
 * basic structure for keyword
 */
typedef struct {
    char *str;
    tKeyWord num;
}keyword_t;

/**
 * Keyword table for looking up keyword.
 */
extern keyword_t keywords_table[KEYWORDS_TABLE_LENGTH];

/**
 * @brief Check whether in_str is a keyword.
 * @param in_str
 * @return keyword in the case of match,
 *         -1 if in_str is not matched
 */
tKeyWord get_keyword_enum(char *in_str);

char* get_string_of_enum(tKeyWord in_enum);



#endif //TABLES2_KEYWORDS_H
