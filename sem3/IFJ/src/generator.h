// generator.h

#ifndef IFJ17_GENERATOR_H
#define IFJ17_GENERATOR_H

#include "symtable.h"
#include "ilist.h"
#include "keywords.h"

/**
 * Function for generating IFJcode17 from TAC.
 * @param iList a list of TAC instructions
 */
void generate(tIList* iList);

/**
 * Helpful function for adding parameters of instructions in IFJcode17
 * @param op operand
 */
void add_param(htab_item_t *op);

/**
 * This function will return the frame of an operand.
 * @param op operand
 * @return frame ["GF", "TF"]
 */
char* getFrame(htab_item_t* op);

/**
 * Prints build-in functions of IFJ17.
 */
void printDefaultFunctions();

/**
 * Changes str to str in the right format of IFJcode17.
 * @param inStr
 * @return NULL if error, must be freed!
 */
char* strToOutStr(char *inStr);

#endif //IFJ17_GENERATOR_H
