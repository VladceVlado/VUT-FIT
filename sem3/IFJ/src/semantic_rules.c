/**
 * project: Implementace překladače imperativního jazyka IFJ17
 * authors: Martin Hošala (xhosal00), Peter Kubov (xkubov06), Tomáš Kukaň (xkukan00), Vladimír Dušek (xdusek27)
 * ------------------------------------
 * @Author	Peter Kubov (xkubov06@stud.fit.vutbr.cz)
 * @date	November, 2017
 * @brief	Actions for semantic analysis for IFJ project.
 *
 * Module provides implementation of semantic actions.
 * Every action is implemented as separate function that
 * has it's own responsibility.
 */

#include <errno.h>

#include "debug_ifj.h"
#include "error_ifj.h"
#include "semantic_essentials.h"
#include "semantic_rules.h"
#include "symtable_stack.h"
#include "symtable.h"
#include "precedent.h"
#include "scanner.h"
#include "prefix_tool.h"

int isDatatype(tToken *token)
{
	if (token == NULL)
		return 0;
	if (token->state != S_KEY_WORD) 
		return 0; 

	tKeyWord kw = token->val.keyWord;
	switch (kw) {
	case KW_STRING:
	case KW_INTEGER:
	case KW_DOUBLE:
		return 1;
	default:
		return 0;
	}
}

char *assignPrefix(SemTools *tools)
{
	if (tools->prefixZombieTmp == NULL || tools->prefixZombieTmp->prefix == NULL)
		return NULL;

	char *tmp = calloc(1, sizeof(char)*strlen(tools->prefixZombieTmp->prefix) + 1);
	strcpy(tmp, tools->prefixZombieTmp->prefix);

	return tmp;
}

void doGenerate(SemTools *tools)
{
	generate(tools->iList);
	iListDestroy(tools->iList);
	tools->iList = iListInit();
	if (tools->iList == NULL) {
		tools->errCode = E_SYS;
		return;
	}
}

void pushPrefix(SemTools *tools)
{
	saSPushPrefix(tools->semanticStack, tools->prefixZombieTmp);
	if (errno != 0) {
		tools->errCode = E_SYS;
		return;
	}

	tools->prefixZombieTmp = calloc(1, sizeof(htab_item_t));
	if (tools->prefixZombieTmp == NULL) {
		tools->errCode = E_SYS;
		return;
	}

	tools->prefixZombieTmp->prefix = createUniqPrefix();
	if (tools->prefixZombieTmp->prefix == NULL) {
		tools->errCode = E_SYS;
		tools->errorMessage = "memory allocation";
		return;
	}
}

void popPrefix(SemTools *tools)
{
	if (saSIsEmpty(tools->semanticStack)) {
		tools->errCode = E_SYS;
		tools->errorMessage = "internal error";
		return;
	}

	if (saSTop(tools->semanticStack) == NULL) {
		tools->errCode = E_SYS;
		tools->errorMessage = "internal error";
		return;
	}

	if (saSTop(tools->semanticStack)->state != SA_PREFIX) {
		tools->errCode = E_SYS;
		tools->errorMessage = "internal error";
		return;
	}

	free(tools->prefixZombieTmp->prefix);
	free(tools->prefixZombieTmp);

	tools->prefixZombieTmp = saSTop(tools->semanticStack)->val.prefix;
	saSPop(tools->semanticStack);
}

void checkOrMakeUniqMain(SemTools *tools)
{
	if (tools->mainDefined) {
		tools->errorMessage = "Main scope allready defined";
		tools->errCode = E_SYN;
	} else {
		tools->mainDefined = 1;
		tools->inMain = 1;
	}
}

void pushSymtable(SemTools *tools)
{
	pushPrefix(tools);

	htab_t *table = htab_init(tools->sizeOfTable);	
	if (table == NULL) {
		tools->errorMessage = "memory allocation error";
		tools->errCode = E_SYS;
		return;
	}

	stPushTable(tools->inScopeTables, table);
	
	if (errno != 0) {
		tools->errorMessage = "memory allocation error";
		tools->errCode = E_SYS;
		return;
	}
}

void popSymtable(SemTools *tools)
{
	popPrefix(tools);
	stStackPop(tools->inScopeTables);
}

char *expectedDatatypeErr(tKeyWord kw)
{
	switch (kw) {
	case KW_STRING:
		return "function definition expected datatype string";
	case KW_INTEGER:
		return "function definition expected datatype integer";
	case KW_DOUBLE:
		return "function definition expected datatype double";
	default:
		return "unexpected datatype";
	}
}

void giveParsedFunctionType(SemTools *tools)
{
	if (tools->parsedFunction == NULL) {
		tools->errCode = E_SYS;
		return;
	} 

	if (!isDatatype(tools->token)) {
		tools->errCode = E_SYN;
		tools->errorMessage = "expected datatype";
		return;
	}

	if (tools->parsedFunction->type == 0) {
		tools->parsedFunction->type = tools->token->val.keyWord;
	} else if (tools->parsedFunction->type != tools->token->val.keyWord) {
		tools->errCode = E_SEM_DEF;
		tools->errorMessage = expectedDatatypeErr(tools->parsedFunction->type);
		return;
	}
}

void saveFunctionToParse(SemTools *tools)
{
	if (tools->token->state != S_IDENTIFIER_HT) {
		tools->errorMessage = "Expected function identifier";
		tools->errCode = E_SYN;
		return;
	}
	
	if (!tools->token->val.identifierHT->is_function || tools->token->val.identifierHT->defined) {
		tools->errCode = E_SEM_DEF;
		tools->errorMessage = "Duplicated definition";
		return;
	}

	tools->actualParam = tools->token->val.identifierHT->parameters;
	tools->parsedFunction = tools->token->val.identifierHT;
}

void declareSaveFunctionToParse(SemTools *tools)
{

	if (tools->token->state == S_IDENTIFIER_HT) {
		tools->errCode = E_SEM_DEF;
		tools->errorMessage = "attempt to redeclare identifier";
		return;
	}

	if (tools->token->state != S_IDENTIFIER) {
		tools->errCode = E_SYN;
		tools->errorMessage = "expected identifier";
		return;
	}

	htab_item_t *item = htab_add(stStackTop(tools->inScopeTables), tools->token->val.identifier);
	if (item == NULL) {
		tools->errCode = E_SYS;
		tools->errorMessage = "memory allocation failed";
		return;
	}

	item->is_function = 1;
	tools->actualParam = NULL;
	tools->parsedFunction = item;
}

void hackIdentifier(SemTools *tools)
{
	if (tools->token->state != S_IDENTIFIER && tools->token->state != S_IDENTIFIER_HT) {
		tools->errCode = E_SYN;
		tools->errorMessage = "expected identifier";
		return;
	}

	if (tools->token->state == S_IDENTIFIER_HT) {
		if (tools->token->val.identifierHT->is_function) {
			tools->errCode = E_SEM_DEF;
			tools->errorMessage = "duplicate definition";
			return;
		}

		if (htab_find(stStackTop(tools->inScopeTables), tools->token->val.identifierHT->name) != NULL) {
			tools->errCode = E_SEM_DEF;
			tools->errorMessage = "redefinition of function parameter";
		}

		tools->token->state = S_IDENTIFIER;
		char *htItemName = tools->token->val.identifierHT->name;
		tools->token->val.identifier = malloc((strlen(htItemName) + 1) * sizeof(char));
		if (tools->token == NULL) {
			tools->errCode = E_SYS;
			return;
		}
		strcpy(tools->token->val.identifier, htItemName);
	}
}

void saveIdentifierToParse(SemTools *tools)
{
	if (tools->token->state != S_IDENTIFIER_HT) {
		tools->errCode = E_SYN;
		tools->errorMessage = "expected identifier";
		return;
	}

	if (tools->token->val.identifierHT->is_function) {
		tools->errCode = E_SYN;
		tools->errorMessage = "expected identifier";
		return;
	}


	tools->parsedId = tools->token->val.identifierHT;
}

void saveFunctionAsIdentifierToParse(SemTools *tools)
{
	tools->parsedId = tools->token->val.identifierHT;
}

void declareIdentifierSave(SemTools *tools)
{

	if (tools->token->state == S_IDENTIFIER_HT) {
		if (tools->token->val.identifierHT->is_function 
			|| htab_find(stStackTop(tools->inScopeTables), tools->token->val.identifierHT->name) != NULL) { 
			tools->errCode = E_SEM_DEF;
			tools->errorMessage = "attempt to redefine identifier";
			return;
		} else {
			tools->token->state = S_IDENTIFIER;
			char *htItemName = tools->token->val.identifierHT->name;
			tools->token->val.identifier = malloc((strlen(htItemName) + 1) * sizeof(char));
			strcpy(tools->token->val.identifier, htItemName);
		}
	}


	if (tools->token->state != S_IDENTIFIER) {
		tools->errCode = E_SYN;
		tools->errorMessage = "expected identifier after dim";
		return;
	}
	

	htab_item_t *item = htab_add(stStackTop(tools->inScopeTables), tools->token->val.identifier);


	if (item == NULL) {
		tools->errCode = E_SYS;
		tools->errorMessage = "unable to allocate memory";
		return;
	}

	item->is_global = false;
	item->is_function = false;
	item->const_value = NULL;
	item->prefix = assignPrefix(tools);

	tools->parsedId = item;
}

void assignTypeToIdentifier(SemTools *tools)
{
	if (!isDatatype(tools->token)) {
		tools->errCode = E_SYN;
		tools->errorMessage = "expected datatype";
		return;
	}

	if (tools->parsedId == NULL) {
		tools->errCode = E_SYS;
	}

	tools->parsedId->type = tools->token->val.keyWord;
}

void leaveIdentifierPtr(SemTools *tools)
{
	tools->parsedId = NULL;
}

void isInFunction(SemTools *tools)
{
	if (tools->inMain) {
		tools->errCode = E_SYN;
		tools->errorMessage = "cannot return, out of functiun";
		return;
	}
}

void generateInputRequest(SemTools *tools)
{
	if (tools->token->state == S_IDENTIFIER) {
		tools->errCode = E_SEM_DEF;
		tools->errorMessage = "undeclared identifier";
		return;
	}

	if (tools->token->state != S_IDENTIFIER_HT) {
		tools->errCode = E_SYN;
		tools->errorMessage = "expected identifier";
		return;
	}
	
	iListCreateAppend(tools->iList, I_READ, tools->token->val.identifierHT, NULL, NULL);
	doGenerate(tools);
}

/*
void declareIterator(SemTools *tools)
{
	if (tools->token->state == S_IDENTIFIER_HT) {
		if (tools->token->val.identifierHT->is_function) {
			tools->errCode = E_SEM_DEF;
			tools->errorMessage = "attempt of redefinition of identifier";
			return;
		}
		
		tools->token->state = S_IDENTIFIER;
		char *htItemName = tools->token->val.identifierHT->name;
		tools->token->val.identifier = malloc(strlen(htItemName + 1) * sizeof(char));
		strcpy(tools->token->val.identifier, htItemName);
	}

	if (tools->token->state != S_IDENTIFIER) {
		tools->errCode = E_SYN;
		tools->errorMessage = "expected identifier";
		return;
	}

	htab_item_t *item = htab_add(stStackTop(tools->inScopeTables), tools->token->val.identifier);

	if (item == NULL) {
		tools->errCode = E_SYS;
		tools->errorMessage = "unable to allocate memory";
		return;
	}

	saSPushDefPtr(tools->semanticStack, item);
	if (errno != 0) {
		tools->errCode = errno;
		tools->errorMessage = "unable to allocate memory";
	}
}
*/

void declareParam(SemTools *tools)
{
	if (!isDatatype(tools->token)) {
		tools->errCode = E_SYN;
		tools->errorMessage = "expected datatype";
		return;
	}

	if (tools->parsedFunction == NULL) {
		tools->errCode = E_SYS;
		return;
	}

	if (htab_item_add_param(tools->parsedFunction, tools->token->val.keyWord) == NULL) {
		tools->errCode = E_SYS;
		return;
	}
}

void defineParam(SemTools *tools)
{
	if (tools->parsedFunction == NULL) {
		tools->errCode = E_SYS;
		return;
	}

	if (tools->parsedFunction->type == 0) {// it was not declared
		declareParam(tools);
		if (tools->errCode)
			return;
	} else { // it was declared, must be checked
		if (tools->actualParam == NULL) {
			tools->errCode = E_SEM_DEF;
			tools->errorMessage = "function definition does not meet it's declaration";
			return;
		} else {
			if (tools->actualParam->type != tools->token->val.keyWord) {
				tools->errCode = E_SEM_DEF;
				tools->errorMessage = "parameters in function definition do not match";
				return;
			}
			tools->actualParam = tools->actualParam->next;
		}
	}
	
	tools->parsedId->type = tools->token->val.keyWord;
	saSPushDefPtr(tools->semanticStack, tools->parsedId);

	tools->parsedId = NULL;
}

void createParameterSymbol(SemTools *tools)
{
	tools->parsedId = htab_add(stStackTop(tools->inScopeTables), tools->token->val.identifier);

	if (tools->parsedId == NULL) {
		tools->errCode = E_SYS;
		return;
	}

	tools->parsedId->prefix = assignPrefix(tools);
}

void createMain(SemTools *tools)
{
	iListCreateAppend(tools->iList, I_MAIN, NULL, NULL, NULL);
	iListCreateAppend(tools->iList, I_DEFVAR, tools->tmps.var_int, NULL, NULL);
	iListCreateAppend(tools->iList, I_DEFVAR, tools->tmps.var_float, NULL, NULL);
	iListCreateAppend(tools->iList, I_DEFVAR, tools->tmps.var_string1, NULL, NULL);
	iListCreateAppend(tools->iList, I_DEFVAR, tools->tmps.var_string2, NULL, NULL);
	iListCreateAppend(tools->iList, I_DEFVAR, tools->tmps.var_string3, NULL, NULL);	
	iListCreateAppend(tools->iList, I_CREATEFRAME, NULL, NULL, NULL);

	doGenerate(tools);

}

void createFunctionLabel(SemTools *tools)
{
	tools->parsedFunction->defined = true;

	iListCreateAppend(tools->iList, I_LABEL, tools->parsedFunction, NULL, NULL);
	doGenerate(tools);
}

void evaluateExpression(SemTools *tools)
{
	ungetToken(&(tools->token));

	tPrTools mTools;
	mTools.symtableStack = tools->inScopeTables;
	mTools.iList = tools->iList;
	mTools.file = tools->f;
	mTools.tmps = tools->tmps;
	mTools.wantedValue = tools->parsedId != NULL ? tools->parsedId->type:0;
	tToken *token = analyseExpr(&mTools);
	tools->token = token;

	if (errno != 0) {
		tools->errCode = errno;
		return;
	}

	doGenerate(tools);
}

void evaluateExpressionForIf(SemTools *tools)
{

	ungetToken(&(tools->token));

	tPrTools mTools;
	mTools.symtableStack = tools->inScopeTables;
	mTools.iList = tools->iList;
	mTools.file = tools->f;
	mTools.tmps = tools->tmps;
	mTools.wantedValue = KW_BOOLEAN;
	tToken *token = analyseExpr(&mTools);
	tools->token = token;

	if (errno != 0) {
		tools->errCode = errno;
		return;
	}

	iListCreateAppend(tools->iList, I_IF, tools->prefixZombieTmp, NULL, NULL);

	doGenerate(tools);
}

void generateEndOfElse(SemTools *tools)
{
	iListCreateAppend(tools->iList, I_ENDELSE, tools->prefixZombieTmp, NULL, NULL);

	doGenerate(tools);
	popPrefix(tools);
}

void evaluateExpressionForWhile(SemTools *tools)
{
	ungetToken(&(tools->token));
	iListCreateAppend(tools->iList, I_WHILE, tools->prefixZombieTmp, NULL, NULL);

	tPrTools mTools;
	mTools.symtableStack = tools->inScopeTables;
	mTools.iList = tools->iList;
	mTools.file = tools->f;
	mTools.tmps = tools->tmps;
	mTools.wantedValue = KW_BOOLEAN;
	tToken *token = analyseExpr(&mTools);
	tools->token = token;

	iListCreateAppend(tools->iList, I_WHILE_COND, tools->prefixZombieTmp, NULL, NULL);

	if (errno != 0) {
		tools->errCode = errno;
		return;
	}
	
	doGenerate(tools);
}

char *generatePrintVar()
{
	static long long i = 0;

	char *buff = calloc(1, sizeof(int)*16 + 5);
	if (buff == NULL) {
		return NULL;
	}

	if (sprintf(buff, "pr_%lld", i++) < 0) {
		return NULL;
	}

	return buff;
}

void evaluateExpressionForPrint(SemTools *tools)
{
	ungetToken(&(tools->token));

	tPrTools mTools;
	mTools.symtableStack = tools->inScopeTables;
	mTools.iList = tools->iList;
	mTools.file = tools->f;
	mTools.tmps = tools->tmps;
	mTools.wantedValue = 0;
	tToken *token = analyseExpr(&mTools);

	tools->token = token;

	if (errno != 0) {
		tools->errCode = errno;
		return;
	}

	char *namePr = generatePrintVar(); 
	if (namePr == NULL) {
		tools->errCode = E_SYS;
		return;
	}

	htab_item_t *itm = htab_add(stStackTop(tools->inScopeTables), namePr);
	itm->prefix = assignPrefix(tools);
	itm->is_function = false;
	itm->is_global = false;
	itm->const_value = NULL;

	iListCreateAppend(tools->iList, I_DEFVAR, itm, NULL, NULL);
	iListCreateAppend(tools->iList, I_POPS, itm, NULL, NULL);
	iListCreateAppend(tools->iList, I_WRITE,itm, NULL, NULL);

	doGenerate(tools);

	free(namePr);
}

void generateAssignmet(SemTools *tools)
{
	iListCreateAppend(tools->iList, I_POPS, tools->parsedId, NULL, NULL);

	doGenerate(tools);
}

void generateIfEnd(SemTools *tools)
{
	htab_item_t *elsePrefix = saSTop(tools->semanticStack)->val.prefix;
	iListCreateAppend(tools->iList, I_ENDIF, tools->prefixZombieTmp, elsePrefix, NULL);

	doGenerate(tools);
	popSymtable(tools);
}

void generateIterationSkip(SemTools *tools)
{
	iListCreateAppend(tools->iList, I_WHILE_CONTINUE, tools->prefixZombieTmp, NULL, NULL);

	doGenerate(tools);
}

void generateLoopExit(SemTools *tools)
{
	iListCreateAppend(tools->iList, I_WHILE_BREAK, tools->prefixZombieTmp, NULL, NULL);
	doGenerate(tools);
}

void generateReturnFunction(SemTools *tools)
{
	tools->parsedId = tools->parsedFunction;
	evaluateExpression(tools);
	if (tools->errCode != 0)
		return;
	tools->parsedId = NULL;

	iListCreateAppend(tools->iList, I_RETURN, NULL, NULL, NULL);

	doGenerate(tools);
}

void generateWhileEnd(SemTools *tools)
{
	iListCreateAppend(tools->iList, I_WHILE_END, tools->prefixZombieTmp, NULL, NULL);
	doGenerate(tools);
}

void terminateParameterDecDef(SemTools *tools)
{
	if (tools->actualParam != NULL) {
		tools->errCode = E_SEM_DEF;
		tools->errorMessage = "function definition does not meet it's declaration";
	}
	
	while (saSTop(tools->semanticStack) != NULL && saSTop(tools->semanticStack)->state == SA_ID_PTR) {
		iListCreateAppend(tools->iList, I_DEFVAR, saSTop(tools->semanticStack)->val.item, NULL, NULL);
		iListCreateAppend(tools->iList, I_POPS, saSTop(tools->semanticStack)->val.item, NULL, NULL);
		saSPop(tools->semanticStack);
	}	

	doGenerate(tools);
}

void implicitInitialization(SemTools *tools)
{
	htab_item_t *constant = htab_add_constant(stStackTop(tools->inScopeTables), tools->parsedId->type == KW_STRING ? "":"0", tools->parsedId->type);
	iListCreateAppend(tools->iList, I_MOVE, tools->parsedId, constant, NULL);

	doGenerate(tools);
}

void generateDeclaration(SemTools *tools)
{
	iListCreateAppend(tools->iList, I_DEFVAR, tools->parsedId, NULL, NULL);
}

void generateGlobalDeclaration(SemTools *tools)
{
	tools->parsedId->is_global = true;
	iListCreateAppend(tools->iList, I_DEFVAR, tools->parsedId, NULL, NULL);
}

void generatePureDeclaration(SemTools *tools)
{
	iListCreateAppend(tools->iList, I_PURE_DEFVAR, tools->parsedId, NULL, NULL);
	doGenerate(tools);
}

void generateGuard(SemTools *tools)
{
	iListCreateAppend(tools->iList, I_GUARD, tools->parsedId, NULL, NULL);
	doGenerate(tools);
}

void finnishGuard(SemTools *tools)
{
	iListCreateAppend(tools->iList, I_GUARD_END, tools->parsedId, NULL, NULL);
	doGenerate(tools);;
}

void popFromStack(SemTools *tools)
{
	htab_item_t *tmp;
	if (tools->parsedId->type == KW_INTEGER)
		tmp = tools->tmps.var_int;
	else if (tools->parsedId->type == KW_STRING)
		tmp = tools->tmps.var_string1;
	else if (tools->parsedId->type == KW_DOUBLE)
		tmp = tools->tmps.var_float;
	else {
		tools->errCode = E_SYS;
		return;
	}

	iListCreateAppend(tools->iList, I_POPS, tmp, NULL, NULL);
}

void generateImplicitReturn(SemTools *tools)
{
	if (tools->parsedFunction == NULL) {
		tools->errCode = E_SYS;
		return;
	}

	htab_item_t *constant = htab_add_constant(stStackTop(tools->inScopeTables), tools->parsedFunction->type == KW_STRING ? "":"0", tools->parsedFunction->type);
	if (constant == NULL) {
		tools->errCode = E_SYS;
		return;
	}

	iListCreateAppend(tools->iList, I_PUSHS, constant, NULL, NULL);
	iListCreateAppend(tools->iList, I_RETURN, NULL, NULL, NULL);

	doGenerate(tools);
}

void shoutErrorRedefiniton(SemTools *tools)
{
	tools->errCode = E_SEM_DEF;
	tools->errorMessage = "Attempt to redefine function";
}

void shoutErrorDefinition(SemTools *tools)
{
	tools->errCode = E_SEM_DEF;
	tools->errorMessage = "Not defined variable";
}

void generateIfProtection(SemTools *tools)
{
	iListCreateAppend(tools->iList, I_IF_ELSE_PROTECTION, tools->prefixZombieTmp, NULL, NULL);

	doGenerate(tools);
}

void protectGuard(SemTools *tools)
{
	iListCreateAppend(tools->iList, I_GUARD_PROTECTION, NULL, NULL, NULL);
}

void incProtectionOfGuard(SemTools *tools)
{
	tools->guardAtReturn++;
}

void decProtectionOfGuard(SemTools *tools)
{
	tools->guardAtReturn--;
}

void popGuard(SemTools *tools)
{
	for (int i = 0; i < tools->guardAtReturn; i++)
		iListCreateAppend(tools->iList, I_POP_GUARD, NULL, NULL, NULL);
}

void assignConstant(SemTools *tools)
{
	htab_item_t *constant = NULL;
	char buff[sizeof(long int)*10+1] = {0};

	switch (tools->parsedId->type) {
	case KW_STRING:
		constant = htab_add_constant(stStackTop(tools->inScopeTables), tools->token->val.stringValue, tools->parsedId->type);
		break;
	case KW_INTEGER:
		snprintf(buff, sizeof(long int)*10+1, "%ld", tools->token->val.intValue);
		constant = htab_add_constant(stStackTop(tools->inScopeTables), buff,tools->parsedId->type);
		break;
	case KW_DOUBLE:
		snprintf(buff, sizeof(long int)*10+1, "%g", tools->token->val.doubleValue);
		constant = htab_add_constant(stStackTop(tools->inScopeTables), buff,tools->parsedId->type);
		break;
	default:
		tools->errCode = E_SYS;
		tools->errorMessage = "unexpected state of program";
		return;
	}
	
	iListCreateAppend(tools->iList, I_MOVE, tools->parsedId, constant, NULL);
}

void shoutExpectedConstant(SemTools *tools)
{
	tools->errCode = E_SYN;
	tools->errorMessage = "expected constant value";
}

void checkEverythingsDefined(SemTools *tools)
{		
	if (!htab_check_functions(stStackTop(tools->inScopeTables))) {
		tools->errCode = E_SEM_DEF;
		tools->errorMessage = "declared function(s) miss definition";
	}
}
