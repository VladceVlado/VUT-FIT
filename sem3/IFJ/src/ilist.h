//
// Created by zeusko on 19/11/17.
//

#ifndef GENERATOR_ILIST_H
#define GENERATOR_ILIST_H

#include <stdio.h>
#include "keywords.h"
#include "symtable.h"

typedef enum {
    // Frame handling, functions calling
    // save some value { I_MOVE, where_to, what, NULL, NULL }
    I_MOVE,
    I_CREATEFRAME,
    I_PUSHFRAME,
    I_POPFRAME,
	I_PURE_DEFVAR,
    I_DEFVAR,
    I_CALL,
    I_RETURN,
    I_IF_ELSE_PROTECTION,
    I_GUARD_PROTECTION,
	I_POP_GUARD,
	I_GUARD,
	I_GUARD_END,

    // stack operations
    I_PUSHS,
    I_POPS,
    I_CLEARS,

    // stack arithmetics
    

    // arithmetic, relation, boolean and conversion instructions
    // { I_ARITHMETIC, where_to, what, what}
    I_ADD,
    I_SUB,
    I_MUL,
    I_DIV,
    I_LT,
    I_GT,
    I_EQ,
    I_AND,
    I_OR,
    // { I_NOT, where_to, what, NULL}
    I_NOT,
    // STACK - arithmetic, relation, boolean and conversion instructions
    // { I_ARITHMETIC, NULL, NULL, NULL}
    I_ADDS,
    I_SUBS,
    I_MULS,
    I_DIVS,
    I_LTS,
    I_GTS,
    I_EQS,
    I_ANDS,
    I_ORS,
    // { I_NOTS, NULL, NULL, NULL}
    I_NOTS,
    
    I_INT2FLOAT,
    I_FLOAT2INT,
    I_FLOAT2R2EINT,
    I_FLOAT2R2OINT,
    I_FLOAT2CHAR,
    I_STRI2INT,

    I_INT2FLOATS,
    I_FLOAT2INTS,
    I_FLOAT2R2EINTS,
    I_FLOAT2R2OINTS,
    I_FLOAT2CHARS,
    I_STRI2INTS,
    
    // input, output
    I_READ,
    I_WRITE,

    // string handling
    I_CONCAT,
    I_STRLEN,
    I_GETCHAR,
    I_SETCHAR,

    // type handling
    I_TYPE,

    // program flow control
    I_LABEL,
    I_JUMP,
    I_JUMPIFEQ,
    I_JUMPIFNEQ,

    // debug instructions
    I_BREAK,
    I_DPRINT,

    // others
    I_IF,
    I_ENDELSE,
    I_ENDIF,
    I_WHILE,
    I_WHILE_COND,
    I_WHILE_END,
    I_WHILE_BREAK,
    I_WHILE_CONTINUE,
    I_FOR,
    I_FOR_END,
    I_FOR_BREAK,
    I_FOR_CONTINUE,
    I_MAIN

} tOperator;


typedef struct {
    tOperator oper;
    htab_item_t *op1;
    htab_item_t *op2;
    htab_item_t *op3;
} tInstruction;

typedef struct iListItem{
    tInstruction instr;
    struct iListItem *next;
} tIListItem;

typedef struct iList{
    tIListItem *head;
    tIListItem *tail;
} tIList;

/**
 * Allocates, initializes and returns tIList struct.
 * @return
 */
tIList *iListInit();

/**
 * Appends to the end of the list.
 * @param list
 * @param instr
 * @return -1 on error, 0 otherwise
 */
int iListAppend(tIList *list, tInstruction instr);

/**
 * Creates Instruction struct from params and appends it at the end of the list.
 * @param list
 * @param op1
 * @param op2
 * @param op3
 * @return -1 on error, 0 otherwise
 */
int iListCreateAppend(tIList *list, tOperator oper, htab_item_t *op1, htab_item_t *op2, htab_item_t *op3);

/**
 * Frees all items of list and destroys it.
 * @return -1 on error, 0 otherwise
 */
void iListDestroy(tIList *list);

#endif //GENERATOR_ILIST_H
