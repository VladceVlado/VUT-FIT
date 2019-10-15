/**
 * project: Implementace překladače imperativního jazyka IFJ17
 * authors: Martin Hošala (xhosal00), Peter Kubov (xkubov06), Tomáš Kukaň (xkukan00), Vladimír Dušek (xdusek27)
 * ------------------------------------
 * @Author	Peter Kubov (xkubov06@stud.fit.vutbr.cz)
 * @date	November, 2017
 * @brief	Syntax analysis for IFJ project.
 *
 * Modul parser provides analysis of lightweighted FreeBasic
 * programming language. Modul cooporates with other modules
 * written for this purpose and eventually generate IFJcoode17
 * on stdout during run.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "garbage_cleaner.h"
#include "parser_essentials.h"
#include "syntax_rules.h"
#include "parser_tools.h"
#include "semantic_rules.h"
#include "parser.h"
#include "scanner.h"
#include "keywords.h"
#include "error_ifj.h"
#include "debug_ifj.h"
#include "ilist.h"
#include "precedent.h"
#include "terms.h"

#define TABLE_SIZE 500
#define MEM_ALOC "memmory allocation"

char *generateGlobalPrefix()
{
	char *tmp = calloc(1, sizeof(char));
	strcpy(tmp, "");
	return tmp;
}

htab_t *createGlobalScope(SemTools *tools)
{
	htab_t *globalTable = htab_init(TABLE_SIZE);

	if (globalTable == NULL)
		return NULL;

	tPrGlobalVars vars;

	htab_item_t *tmp = NULL;

	tmp = htab_add(globalTable, "tmp_int");
	if (tmp == NULL)
		return NULL;

	tmp->type = KW_INTEGER;
	tmp->is_function = false;
	tmp->is_global = true;
	tmp->prefix = generateGlobalPrefix();
	vars.var_int = tmp;

	tmp = htab_add(globalTable, "tmp_double");
	if (tmp == NULL)
		return NULL;

	tmp->type = KW_DOUBLE;
	tmp->is_function = false;
	tmp->is_global = true;
	tmp->prefix = generateGlobalPrefix();
	vars.var_float = tmp;

	tmp = htab_add(globalTable, "tmp_string");
	if (tmp == NULL)
		return NULL;

	tmp->type = KW_STRING;
	tmp->is_function = false;
	tmp->is_global = true;
	tmp->prefix = generateGlobalPrefix();
	vars.var_string1 = tmp;

	tmp = htab_add(globalTable, "tmp_string2");
	if (tmp == NULL)
		return NULL;

	tmp->type = KW_INTEGER;
	tmp->is_function = false;
	tmp->is_global = true;
	tmp->prefix = generateGlobalPrefix();
	vars.var_string2 = tmp;

	tmp = htab_add(globalTable, "tmp_string3");
	if (tmp == NULL)
		return NULL;

	tmp->type = KW_INTEGER;
	tmp->is_function = false;
	tmp->is_global = true;
	tmp->prefix = generateGlobalPrefix();
	vars.var_string3 = tmp;

	tools->tmps = vars;

	tmp = htab_add(globalTable, "length");
	if (tmp == NULL)
		return NULL;

	tmp->type = KW_INTEGER;
	tmp->is_function = true;
	tmp->defined = true;
	htab_item_add_param(tmp, KW_STRING);

	tmp = htab_add(globalTable, "substr");
	if (tmp == NULL)
		return NULL;

	tmp->type = KW_STRING;
	tmp->is_function = true;
	tmp->defined = true;
	htab_item_add_param(tmp, KW_STRING);
	htab_item_add_param(tmp, KW_INTEGER);
	htab_item_add_param(tmp, KW_INTEGER);

	tmp = htab_add(globalTable, "asc");
	if (tmp == NULL)
		return NULL;

	tmp->type = KW_INTEGER;
	tmp->is_function = true;
	tmp->defined = true;
	htab_item_add_param(tmp, KW_STRING);
	htab_item_add_param(tmp, KW_INTEGER);

	tmp = htab_add(globalTable, "chr");
	if (tmp == NULL)
		return NULL;

	tmp->type = KW_STRING;
	tmp->is_function = true;
	tmp->defined = true;
	htab_item_add_param(tmp, KW_INTEGER);

	return globalTable;
}

SemTools *allocResources()
{
	SemTools *tools = calloc(1, sizeof(SemTools));
	if (tools == NULL)
		return NULL;

	tools->token = newToken();
	tools->semanticStack = newSAStack();
	tools->inScopeTables = newSTStack();
	tools->errCode = 0;
	tools->errorMessage = NULL;
	tools->mainDefined = 0;
	tools->f = stdin;
	tools->actualParam = NULL;
	tools->parsedId = NULL;
	tools->inMain = 0;
	tools->iList = iListInit();
	tools->sizeOfTable = TABLE_SIZE;
	tools->prefixZombieTmp = calloc(1, sizeof(htab_item_t));
	tools->prefixZombieTmp->prefix = generateGlobalPrefix();

	if (registerParserGarbage(tools))
		return NULL;

	return tools;
}

int parseCode(FILE *file)
{
	SemTools *tools = allocResources();
	if (tools == NULL)
		errorExit(E_SYS, MEM_ALOC);

	tools->f = file;

	TStack syntaxStack = newStack();
	if (syntaxStack == NULL)
		errorExit(E_SYS, MEM_ALOC);

	stStackInit(tools->inScopeTables);
	stackInit(syntaxStack);
	saSinit(tools->semanticStack);

	htab_t *globalTable = createGlobalScope(tools);
	if (globalTable == NULL)
		errorExit(E_SYS, MEM_ALOC);
	
	stPushTable(tools->inScopeTables, globalTable);

	tools->errCode = getToken(tools->f, tools->token, tools->inScopeTables);
	if (tools->errCode)
		errorExit(tools->errCode, tools->token->val.errorMsg);

	stackPushNTerm(syntaxStack, NT_PROGRAM);

	bool existsNextToken = true;
	while (existsNextToken) {
		if (stackIsEmpty(syntaxStack)) {
			if (tools->token->state == S_EOF) 
				existsNextToken = false;
			else {
				freeStack(syntaxStack);
				errorExit(E_SYN, NULL);
			}
		} else if (isTermOnTop(syntaxStack)) {
			TTermVal term = stackTopTerm(syntaxStack);
			if (term == getTerm(tools->token)) {
				stackPop(syntaxStack);
				if (tools->token->state == S_IDENTIFIER || tools->token->state == S_STRING) {
					free(tools->token->val.identifier);
				}
				tools->errCode = getToken(tools->f, tools->token, tools->inScopeTables);

				if (tools->errCode) {
					freeStack(syntaxStack);
					errorExit(tools->errCode, tools->token->val.errorMsg);
				}	

			} else {
				freeStack(syntaxStack);
				errorExit(E_SYN,"unexpected token");
			}
		} else if (isNTermOnTop(syntaxStack)) {
			TRuleFunction applyRule = getRule(stackTopNTerm(syntaxStack), getTerm(tools->token));
			if (applyRule == NULL) {
				freeStack(syntaxStack);
				errorExit(E_SYN, "incorrect syntax");
			} else {
				applyRule(syntaxStack);
			}
		} else if (isSemRuleOnTop(syntaxStack)) {
			TSemRule applyRule = stackTopSemRule(syntaxStack);
			applyRule(tools);
			if (tools->errCode != 0) {
				freeStack(syntaxStack);
				errorExit(tools->errCode, tools->errorMessage);
			}
			stackPop(syntaxStack);
		} else {
			freeStack(syntaxStack);
			errorExit(E_SYN, "magic");
		}
	} 

	if (!tools->mainDefined) {
		freeStack(syntaxStack);
		errorExit(E_SYN, "Main scope not defined");
	}

	generate(tools->iList);
	freeStack(syntaxStack);

	return 0;
}
