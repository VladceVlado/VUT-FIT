/**
 * @Author	Peter Kubov (xkubov06@stud.fit.vutbr.cz)
 * @date	November, 2017
 * @brief	Tools used in semantic analysis.
 *
 * Module provides implementation of tools used by semantic analysis. Tools of semantic analysis are attempt of simplifying and clarifying internal structure of algorithm used in semantic analysis.
 */

#ifndef _IFJ42_PARSER_TOOLS_H
#define _IFJ42_PARSER_TOOLS_H

#include "semantic_essentials.h"
#include "precedent.h"
#include "symtable.h"

/**
 * @brief	Type representing tools used by semantic analysis.
 * @ingroup	parser
 */
typedef struct semtools {
	int mainDefined; /// specifyies main scope definition
	int inMain; /// sepcifyies if smeantic of main scope is being veryfied.
	STStack *inScopeTables; /// stack of symbolic tables used for scope extension
	int sizeOfTable; /// represents size of each symbolic table in stack of symbolic tables
	tToken *token; /// represents actual token on input
	SAStack semanticStack; /// stack used in semantic analysis
	char *errorMessage; /// place for semantic rules to define error that occured
	int errCode; /// specifies error that occured during semantic action
	htab_item_t *parsedFunction; /// represents function that is being processed during semantic analysis
	htab_item_t *parsedId; /// represents variable that is being processed during semantic analysis
	tParam *actualParam; /// represents parameter of function that is processed during semntic analysis
	FILE *f; /// input file containing parsed code
	tIList *iList; /// list where are appended instructions for output code generator
	tPrGlobalVars tmps; /// definitions of temporary varibales that are required in precedent analysis
	htab_item_t *prefixZombieTmp; /// not actually item of symtable but container of prefix that is used to distignuish variables in scope extension
	int guardAtReturn;
} SemTools;

#endif
