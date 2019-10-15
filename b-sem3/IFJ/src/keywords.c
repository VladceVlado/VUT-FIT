//
// Created by zeusko on 19/10/17.
//

#include "keywords.h"

/**
 * keyword table definition
 */
keyword_t keywords_table[KEYWORDS_TABLE_LENGTH] = {
        { "as",       KW_AS },
        { "13_asc",   KW_ASC },
        { "declare",  KW_DECLARE },
        { "dim",      KW_DIM },
        { "do",       KW_DO },
        { "double",   KW_DOUBLE },
        { "else",     KW_ELSE },
        { "end",      KW_END },
        { "13_chr",   KW_CHR },
        { "function", KW_FUNCTION },
        { "if",       KW_IF },
        { "input",    KW_INPUT },
        { "integer",  KW_INTEGER },
        { "2length",  KW_LENGTH },
        { "loop",     KW_LOOP },
        { "print",    KW_PRINT },
        { "return",   KW_RETURN },
        { "scope",    KW_SCOPE },
        { "string",   KW_STRING },
        { "2substr",  KW_SUBSTR },
        { "then",     KW_THEN },
        { "while",    KW_WHILE },
        // reserved
        { "and",      KW_AND },
        { "boolean",  KW_BOOLEAN },
        { "continue", KW_CONTINUE },
        { "elseif",   KW_ELSEIF },
        { "exit",     KW_EXIT },
        { "false",    KW_FALSE },
        { "for",      KW_FOR },
        { "next",     KW_NEXT },
        { "not",      KW_NOT },
        { "or",       KW_OR },
        { "shared",   KW_SHARED },
        { "static",   KW_STATIC },
        { "true",     KW_TRUE }
};

tKeyWord get_keyword_enum(char *in_str) {
    if (in_str == NULL)
        return (tKeyWord)-1;

    for (int i = 0; i < KEYWORDS_TABLE_LENGTH; i++)
        if (strcasecmp(in_str, keywords_table[i].str) == 0) // if equals
            return keywords_table[i].num;

    return  (tKeyWord)NOT_FOUND;
}

char* get_string_of_enum(tKeyWord in_enum)
{
    for (int i = 0; i < KEYWORDS_TABLE_LENGTH; i++)
        if (keywords_table[i].num == in_enum) // if equals
            return keywords_table[i].str;

    return NULL;
}
