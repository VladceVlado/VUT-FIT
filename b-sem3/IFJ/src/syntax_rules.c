/**
 * project: Implementace překladače imperativního jazyka IFJ17
 * authors: Martin Hošala (xhosal00), Peter Kubov (xkubov06), Tomáš Kukaň (xkukan00), Vladimír Dušek (xdusek27)
 * ------------------------------------
 * @Author	Peter Kubov (xkubov06@stud.fit.vutbr.cz)
 * @date	November, 2017
 * @brief	Rules for syntax analysis for IFJ project.
 *
 * Module provides implementation of rules of LL grammar.
 */

#include "error_ifj.h"
#include "keywords.h"
#include "scanner.h"
#include "parser_essentials.h"
#include "syntax_rules.h"

void parseProgramToFunctionPart(TStack stack)
{
	stackPop(stack);
	stackPushNTerm(stack, NT_PROGRAM);
	stackPushNTerm(stack, NT_FUNCTIONS);
}

void parseProgramToMainPart(TStack stack)
{
	stackPop(stack);
	stackPushNTerm(stack, NT_PROGRAM);
	stackPushTerm(stack, T_SCOPE);
	stackPushSemRule(stack, popSymtable);
	stackPushNTerm(stack, NT_STATEMENTS);
	stackPushSemRule(stack, pushSymtable);
	stackPushSemRule(stack, createMain);
	stackPushTerm(stack, T_EOL);
	stackPushTerm(stack, T_SCOPE);
	stackPushSemRule(stack, checkOrMakeUniqMain);
	stackPushSemRule(stack, checkEverythingsDefined);
}

void parseEmptyLineInProgram(TStack stack)
{
	stackPop(stack);
	stackPushNTerm(stack, NT_PROGRAM);
	stackPushTerm(stack, T_EOL);
}
void parseEndOfProgram(TStack stack) {

	stackPop(stack);
}

void parseDeclareFunction(TStack stack)
{
	stackPop(stack);
	stackPushTerm(stack, T_EOL);
	stackPushNTerm(stack,NT_DATATYPE);
	stackPushSemRule(stack, giveParsedFunctionType);
	stackPushTerm(stack, T_AS);
	stackPushNTerm(stack, NT_PARAMETERS);
	stackPushTerm(stack, T_OPENBRACK);
	stackPushTerm(stack, T_IDENTIFIER);
	stackPushSemRule(stack, declareSaveFunctionToParse);
	stackPushTerm(stack, T_FUNCTION);
	stackPushTerm(stack, T_DECLARE);
}

void parseDefineFunction(TStack stack)
{
	stackPop(stack);
	stackPushNTerm(stack, NT_DEFINE);
	stackPushTerm(stack, T_FUNCTION);
}

void parseDefineNotDeclaredFunction(TStack stack)
{
	stackPop(stack);
	stackPushTerm(stack, T_EOL);
	stackPushTerm(stack, T_FUNCTION);
	stackPushSemRule(stack, popSymtable);
	stackPushSemRule(stack, generateImplicitReturn);
	stackPushNTerm(stack, NT_STATEMENTS);
	stackPushTerm(stack, T_EOL);
	stackPushNTerm(stack,NT_DATATYPE);
	stackPushSemRule(stack, giveParsedFunctionType);
	stackPushTerm(stack, T_AS);
	stackPushNTerm(stack, NT_DEF_PARAMETERS);
	stackPushTerm(stack, T_OPENBRACK);
	stackPushSemRule(stack, pushSymtable);
	stackPushTerm(stack, T_IDENTIFIER);
	stackPushSemRule(stack, createFunctionLabel);
	stackPushSemRule(stack, declareSaveFunctionToParse);
}

void parseDefineDeclaredFunction(TStack stack)
{
	stackPop(stack);
	stackPushTerm(stack, T_EOL);
	stackPushTerm(stack, T_FUNCTION);
	stackPushSemRule(stack, popSymtable);
	stackPushSemRule(stack, generateImplicitReturn);
	stackPushNTerm(stack, NT_STATEMENTS);
	stackPushTerm(stack, T_EOL);
	stackPushNTerm(stack,NT_DATATYPE);
	stackPushSemRule(stack, giveParsedFunctionType);
	stackPushTerm(stack, T_AS);
	stackPushNTerm(stack, NT_DEF_PARAMETERS);
	stackPushTerm(stack, T_OPENBRACK);
	stackPushSemRule(stack, pushSymtable);
	stackPushTerm(stack, T_FUNID);
	stackPushSemRule(stack, createFunctionLabel);
	stackPushSemRule(stack, saveFunctionToParse);
}

void parseStatementsToMoreStatements(TStack stack)
{
	stackPop(stack);
	stackPushNTerm(stack, NT_STATEMENTS);
	stackPushNTerm(stack, NT_STATEMENT);
}

void parseStatementsToEnd(TStack stack)
{
	stackPop(stack);
	stackPushTerm(stack, T_END);
}

void parseEmptyLineStatement(TStack stack)
{
	stackPop(stack);
	stackPushTerm(stack, T_EOL);
}

void parseAssignment(TStack stack)
{
	stackPop(stack);
	stackPushTerm(stack, T_EOL);
	stackPushSemRule(stack, leaveIdentifierPtr);
	stackPushSemRule(stack, generateAssignmet);
	stackPushSemRule(stack, evaluateExpression);
	stackPushTerm(stack, T_ASSIGN);
	stackPushTerm(stack, T_IDENTIFIER_HT);
	stackPushSemRule(stack, saveIdentifierToParse);
}

void parseDeclaration(TStack stack)
{
	stackPop(stack);
	stackPushSemRule(stack, leaveIdentifierPtr);
	stackPushNTerm(stack, NT_INITIALIZATION);
	stackPushSemRule(stack, generateDeclaration);	
	stackPushNTerm(stack, NT_DATATYPE);
	stackPushSemRule(stack, assignTypeToIdentifier);
	stackPushTerm(stack, T_AS);
	stackPushTerm(stack, T_IDENTIFIER);
	stackPushSemRule(stack, declareIdentifierSave);
	stackPushTerm(stack, T_DIM);
}

void parseGlobalDeclaration(TStack stack)
{
	stackPop(stack);
	stackPushNTerm(stack, NT_PROGRAM);
	stackPushSemRule(stack, leaveIdentifierPtr);
	stackPushNTerm(stack, NT_GL_INITIALIZATION);
	stackPushSemRule(stack, generateGlobalDeclaration);	
	stackPushNTerm(stack, NT_DATATYPE);
	stackPushSemRule(stack, assignTypeToIdentifier);
	stackPushTerm(stack, T_AS);
	stackPushTerm(stack, T_IDENTIFIER);
	stackPushSemRule(stack, declareIdentifierSave);
	stackPushTerm(stack, T_SHARED);
	stackPushTerm(stack, T_DIM);
}

void parseStaticDeclaration(TStack stack)
{
	stackPop(stack);
	stackPushSemRule(stack, leaveIdentifierPtr);
	stackPushSemRule(stack, finnishGuard);
	stackPushNTerm(stack, NT_GL_INITIALIZATION);
	stackPushSemRule(stack, generatePureDeclaration);	
	stackPushSemRule(stack, generateGuard);
	stackPushNTerm(stack, NT_DATATYPE);
	stackPushSemRule(stack, assignTypeToIdentifier);
	stackPushTerm(stack, T_AS);
	stackPushTerm(stack, T_IDENTIFIER);
	stackPushSemRule(stack, declareIdentifierSave);
	stackPushTerm(stack, T_STATIC);
}

void parseInitializationOfGLIdentifier(TStack stack)
{
	stackPop(stack);
	stackPushTerm(stack, T_EOL);
	stackPushNTerm(stack, NT_CONST_VAL);
	stackPushTerm(stack, T_ASSIGN);
}

void parseInitializationOfIdentifier(TStack stack)
{
	stackPop(stack);
	stackPushTerm(stack, T_EOL);
	stackPushSemRule(stack, generateAssignmet);
	stackPushSemRule(stack, evaluateExpression);
	stackPushTerm(stack, T_ASSIGN);
}

void parseNoInitialization(TStack stack)
{
	stackPop(stack);
	stackPushTerm(stack, T_EOL);
	stackPushSemRule(stack, implicitInitialization);
}

void parseFunctionCall(TStack stack)
{
	stackPop(stack);
	stackPushSemRule(stack, leaveIdentifierPtr);
	stackPushSemRule(stack, popFromStack);
	stackPushSemRule(stack, evaluateExpression);
	stackPushSemRule(stack, saveFunctionAsIdentifierToParse);
}

void parseScopeProgram(TStack stack)
{
	stackPop(stack);
	stackPushTerm(stack, T_EOL);
	stackPushTerm(stack, T_SCOPE);
	stackPushSemRule(stack, popSymtable);
	stackPushNTerm(stack, NT_STATEMENTS);
	stackPushSemRule(stack, pushSymtable);
	stackPushTerm(stack, T_EOL);
	stackPushTerm(stack, T_SCOPE);
}

void parseInput(TStack stack)
{
	stackPop(stack);
	stackPushTerm(stack, T_EOL);
	stackPushTerm(stack, T_IDENTIFIER_HT);
	stackPushSemRule(stack, generateInputRequest);
	stackPushTerm(stack, T_INPUT);
}

void parsePrintStatement(TStack stack)
{
	stackPop(stack);
	stackPushNTerm(stack, NT_NEXT_EXPR);
	stackPushTerm(stack, T_SEMMICOL);
	stackPushSemRule(stack, evaluateExpressionForPrint);
	stackPushTerm(stack, T_PRINT);
}

void parseNextPrintExpression(TStack stack)
{
	stackPop(stack);
	stackPushNTerm(stack, NT_NEXT_EXPR);
	stackPushTerm(stack, T_SEMMICOL);
	stackPushSemRule(stack, evaluateExpressionForPrint);
}

void parseEndOfPrinting(TStack stack)
{
	stackPop(stack);
	stackPushTerm(stack, T_EOL);
}

void parseWhileLoop(TStack stack)
{
	stackPop(stack);
	stackPushTerm(stack, T_EOL);
	stackPushSemRule(stack, popSymtable);
	stackPushSemRule(stack, decProtectionOfGuard);
	stackPushSemRule(stack, generateWhileEnd);
	stackPushNTerm(stack, NT_WHILE_STATEMENT);
	stackPushTerm(stack, T_EOL);
	stackPushSemRule(stack, evaluateExpressionForWhile);
	stackPushSemRule(stack, incProtectionOfGuard);
	stackPushTerm(stack, T_WHILE);
	stackPushTerm(stack, T_DO);
	stackPushSemRule(stack, pushSymtable);
}

void parseWhileStatement(TStack stack)
{
	stackPop(stack);
	stackPushNTerm(stack, NT_WHILE_STATEMENT);
	stackPushNTerm(stack, NT_STATEMENT);
}

void parseSkipIteration(TStack stack)
{
	stackPop(stack);
	stackPushNTerm(stack, NT_WHILE_STATEMENT);
	stackPushTerm(stack, T_EOL);
	stackPushSemRule(stack, generateIterationSkip);
	stackPushTerm(stack, T_CONTINUE);
}

void parseBreakLoop(TStack stack)
{
	stackPop(stack);
	stackPushNTerm(stack, NT_WHILE_STATEMENT);
	stackPushTerm(stack, T_EOL);
	stackPushSemRule(stack, generateLoopExit);
	stackPushTerm(stack, T_EXIT);
}

void parseEndOfWhileLoop(TStack stack)
{
	stackPop(stack);
	stackPushTerm(stack, T_LOOP);
}

void parseReturnFunction(TStack stack)
{
	stackPop(stack);
	stackPushSemRule(stack, generateReturnFunction);
	stackPushSemRule(stack, popGuard);
	stackPushSemRule(stack, isInFunction);
	stackPushTerm(stack, T_RETURN);
}

void parseCondition(TStack stack)
{
	stackPop(stack);
	stackPushTerm(stack, T_EOL);
	stackPushTerm(stack, T_IF);
	stackPushSemRule(stack, decProtectionOfGuard);
	stackPushNTerm(stack, NT_IF_STATEMENT);
	stackPushTerm(stack, T_EOL);
	stackPushTerm(stack, T_THEN);
	stackPushSemRule(stack, evaluateExpressionForIf);
	stackPushSemRule(stack, generateIfProtection);
	stackPushSemRule(stack, pushSymtable);
	stackPushSemRule(stack, incProtectionOfGuard);
	stackPushSemRule(stack, protectGuard);
	stackPushTerm(stack, T_IF);
	stackPushSemRule(stack, pushPrefix);
}

void parseIfStatement(TStack stack)
{
	stackPop(stack);
	stackPushNTerm(stack, NT_IF_STATEMENT);
	stackPushNTerm(stack, NT_STATEMENT);
}

void parseElsifCondition(TStack stack)
{
	stackPop(stack);
	stackPushNTerm(stack, NT_IF_STATEMENT);
	stackPushTerm(stack, T_EOL);
	stackPushTerm(stack, T_THEN);
	stackPushSemRule(stack, evaluateExpressionForIf);
	stackPushTerm(stack, T_ELSIF);
	stackPushSemRule(stack, generateIfProtection);
	stackPushSemRule(stack, pushSymtable);
	stackPushSemRule(stack, generateIfEnd);
}

void parseElseOfCondition(TStack stack)
{
	stackPop(stack);
	stackPushNTerm(stack, NT_ELSE_STATEMENTS);
	stackPushTerm(stack, T_EOL);
	stackPushTerm(stack, T_ELSE);
	stackPushSemRule(stack, pushSymtable);
	stackPushSemRule(stack, generateIfEnd);
}

void parseEndOfElseOfCondition(TStack stack)
{
	stackPop(stack);
	stackPushSemRule(stack, generateEndOfElse);
	stackPushSemRule(stack, popSymtable);
	stackPushTerm(stack, T_END);
}

void parseElseStatement(TStack stack)
{
	stackPop(stack);
	stackPushNTerm(stack, NT_ELSE_STATEMENTS);
	stackPushNTerm(stack, NT_STATEMENT);
}

void parseEndOfCondition(TStack stack)
{
	stackPop(stack);
	stackPushTerm(stack, T_END);
	stackPushSemRule(stack, generateEndOfElse);
	stackPushSemRule(stack, generateIfEnd);
}
/*
void parseForLoop(TStack stack)
{


	stackPop(stack);
	stackPushSemRule(stack, popSymtable);
	stackPushSemRule(stack, generateEndFor);
	stackPushNTerm(stack, NT_FOR_STATEMENTS);
	stackPushTerm(stack, T_INTVAL);
	stackPushTerm(stack, T_TO);
	stackPushTerm(stack, T_IDENTIFIER);
	stackPushSemRule(stack, generateForLoop);
	stackPushSemRule(stack, declareIterator);
	stackPushSemRule(stack, pushSymtable);
	stackPushTerm(stack, T_FOR);
}

void parseForStatement(TStack stack)
{


	stackPop(stack);
	stackPushNTerm(stack, NT_FOR_STATEMENTS);
	stackPushNTerm(stack, NT_STATEMENT);
}

void parseNextInFor(TStack stack)
{


	stackPop(stack);
	stackPushTerm(stack, T_EOL);
	stackPushTerm(stack, T_IDENTIFIER_HT);
	stackPushTerm(stack, T_NEXT);
}

void parseSkipIterationFor(TStack stack)
{


	stackPop(stack);;
	stackPushTerm(stack, T_EOL);;
	stackPushTerm(stack, T_CONTINUE);
//	stackPushSemRule(stack, generateIterationSkip);
}

void parseBreakLoopFor(TStack stack)
{


	stackPop(stack);
	stackPushTerm(stack, T_EOL);
	stackPushTerm(stack, T_EXIT);
//	stackPushSemRule(stack, generateLoopExit);
}
*/

void parseNoParametersToFunction(TStack stack)
{
	stackPop(stack);
	stackPushTerm(stack, T_CLOSEBRACK);
	stackPushSemRule(stack, terminateParameterDecDef);
}

void parseFirstParameterToFunction(TStack stack)
{
	stackPop(stack);
	stackPushNTerm(stack, NT_NEXTPARAMETER);
	stackPushNTerm(stack, NT_PARAMETER);
}

void parseNoParametersToDefinition(TStack stack)
{
	stackPop(stack);
	stackPushTerm(stack, T_CLOSEBRACK);
	stackPushSemRule(stack, terminateParameterDecDef);
}

void parseFirstParameterToDefinition(TStack stack)
{
	stackPop(stack);
	stackPushNTerm(stack, NT_DEF_NEXTPARAMETER);
	stackPushNTerm(stack, NT_DEF_PARAMETER);
}

void parseDeclarationOfParameterToFunction(TStack stack)
{
	stackPop(stack);
	stackPushNTerm(stack, NT_DATATYPE);
	stackPushSemRule(stack, declareParam);
	stackPushTerm(stack, T_AS);
	stackPushTerm(stack, T_IDENTIFIER);
	stackPushSemRule(stack, hackIdentifier);
}

void parseDeclarationOfNextParameterToFunction(TStack stack)
{
	stackPop(stack);
	stackPushNTerm(stack, NT_NEXTPARAMETER);
	stackPushNTerm(stack, NT_PARAMETER);
	stackPushTerm(stack, T_COMMA);
}

void parseEndOfParamsDeclaration(TStack stack)
{
	stackPop(stack);
	stackPushTerm(stack, T_CLOSEBRACK);
}

void parseDefinitionOfParameterToFunction(TStack stack)
{
	stackPop(stack);
	stackPushNTerm(stack, NT_DATATYPE);
	stackPushSemRule(stack, defineParam);
	stackPushTerm(stack, T_AS);
	stackPushTerm(stack, T_IDENTIFIER);
	stackPushSemRule(stack, createParameterSymbol);
	stackPushSemRule(stack, hackIdentifier);
}

void parseDefinitionOfNextParameterToFunction(TStack stack)
{
	stackPop(stack);
	stackPushNTerm(stack, NT_DEF_NEXTPARAMETER);
	stackPushNTerm(stack, NT_DEF_PARAMETER);
	stackPushTerm(stack, T_COMMA);
}

void parseEndOfParamsDefinition(TStack stack)
{
	stackPop(stack);
	stackPushTerm(stack, T_CLOSEBRACK);
	stackPushSemRule(stack, terminateParameterDecDef);
	
}

void parseIntegerType(TStack stack)
{
	stackPop(stack);
	stackPushTerm(stack, T_INTEGER);
}

void parseStringType(TStack stack)
{
	stackPop(stack);
	stackPushTerm(stack, T_STRING);
}

void parseFloatType(TStack stack)
{
	stackPop(stack);
	stackPushTerm(stack, T_DOUBLE);
}

void parseErrorRedefinitionParatmeter(TStack stack)
{
	stackPop(stack);
	stackPushSemRule(stack, shoutErrorRedefiniton);
}

void parseErrorUndefinedVariable(TStack stack)
{
	stackPop(stack);
	stackPushSemRule(stack, shoutErrorDefinition);
}

void parseStringValue(TStack stack)
{
	stackPop(stack);
	stackPushTerm(stack, T_STRINGVAL);
	stackPushSemRule(stack, assignConstant);
}

void parseIntegerValue(TStack stack)
{
	stackPop(stack);
	stackPushTerm(stack, T_INTVAL);
	stackPushSemRule(stack, assignConstant);
}

void parseDoubleValue(TStack stack)
{
	stackPop(stack);
	stackPushTerm(stack, T_DOUBLEVAL);
	stackPushSemRule(stack, assignConstant);
}

void parseErrorExpectedConstant(TStack stack)
{
	stackPop(stack);
	stackPushSemRule(stack, shoutExpectedConstant);
}

int isTerm(TTermVal tokenState)
{
	switch(tokenState) {
	case T_IDENTIFIER:
	case T_IDENTIFIER_HT:
	case T_FUNID:
	case T_INTVAL:
	case T_STRINGVAL:
	case T_DOUBLEVAL:
		return 1;
	default:
		return 0;
	}
}

TRuleFunction getRule(TNTermVal nt, TTermVal tokenState) 
{
	switch (nt) {
	case NT_PROGRAM:
		switch (tokenState) {
		case T_DECLARE:
		case T_FUNCTION:
			return parseProgramToFunctionPart;
		case T_SCOPE:
			return parseProgramToMainPart;
		case T_EOL:
			return parseEmptyLineInProgram;
		case T_EOF:
			return parseEndOfProgram;
		case T_DIM:
			return parseGlobalDeclaration;
		default:
			return NULL;
		}
	
	case NT_FUNCTIONS:
		switch (tokenState) {
		case T_DECLARE:
			return parseDeclareFunction;
		case T_FUNCTION:
			return parseDefineFunction;
		default:
			return NULL;
		}

	case NT_DEFINE:
		switch (tokenState) {
		case T_IDENTIFIER:
			return parseDefineNotDeclaredFunction;
		case T_FUNID:
			return parseDefineDeclaredFunction;
		default:
			return NULL;
		}

	case NT_STATEMENTS:
		switch (tokenState) {
		case T_DIM:
		case T_IDENTIFIER_HT:
		case T_FUNID:
		case T_SCOPE:
		case T_INPUT:
		case T_PRINT:
		case T_DO:
		case T_EOL:
		case T_RETURN:
		case T_FOR:
		case T_IF:
		case T_IDENTIFIER:
		case T_STATIC:
			return parseStatementsToMoreStatements;

		case T_END:
			return parseStatementsToEnd;
		default:
			return NULL;			
		}

	case NT_STATEMENT:
		switch (tokenState) {
		case T_EOL:
			return parseEmptyLineStatement;
		case T_DIM:
			return parseDeclaration;
		case T_IDENTIFIER_HT:
			return parseAssignment;
		case T_FUNID:
			return parseFunctionCall;
		case T_SCOPE:
			return parseScopeProgram;
		case T_INPUT:
			return parseInput;
		case T_PRINT:
			return parsePrintStatement;
		case T_DO:
			return parseWhileLoop;
		case T_RETURN:
			return parseReturnFunction;
		case T_IF:
			return parseCondition;
		case T_IDENTIFIER:
			return parseErrorUndefinedVariable;
		case T_STATIC:
			return parseStaticDeclaration;
//		case T_FOR:
//			return parseForLoop;
		default:
			return NULL;
		}

	case NT_NEXT_EXPR:
		if (isTerm(tokenState)) {
			return parseNextPrintExpression;
		} else if (tokenState == T_EOL) {
			return parseEndOfPrinting;
		} else {
			return NULL;
		}

	case NT_WHILE_STATEMENT:
		switch (tokenState) {
		case T_DIM:
		case T_IDENTIFIER_HT:
		case T_FUNID:
		case T_SCOPE:
		case T_INPUT:
		case T_PRINT:
		case T_DO:
		case T_EOL:
		case T_RETURN:
		case T_FOR:
		case T_IF:
		case T_IDENTIFIER:
		case T_STATIC:
			return parseWhileStatement;
		case T_CONTINUE:
			return parseSkipIteration;
		case T_EXIT:
			return parseBreakLoop;
		case T_LOOP:
			return parseEndOfWhileLoop;
		default:
			return NULL;
		}

	case NT_IF_STATEMENT:
		switch (tokenState) {
		case T_DIM:
		case T_IDENTIFIER_HT:
		case T_FUNID:
		case T_SCOPE:
		case T_INPUT:
		case T_PRINT:
		case T_DO:
		case T_EOL:
		case T_RETURN:
		case T_FOR:
		case T_IF:
		case T_IDENTIFIER:
		case T_STATIC:
			return parseIfStatement;
		case T_END:
			return parseEndOfCondition;
		case T_ELSE:
			return parseElseOfCondition;
		case T_ELSIF:
			return parseElsifCondition;
		default:
			return NULL;
		}

	case NT_ELSE_STATEMENTS:
		switch (tokenState) {
		case T_DIM:
		case T_IDENTIFIER_HT:
		case T_FUNID:
		case T_SCOPE:
		case T_INPUT:
		case T_PRINT:
		case T_DO:
		case T_EOL:
		case T_RETURN:
		case T_FOR:
		case T_IF:
		case T_IDENTIFIER:
		case T_STATIC:
			return parseElseStatement;
		case T_END:
			return parseEndOfElseOfCondition;
		default:
			return NULL;
		}

	case NT_INITIALIZATION:
		switch (tokenState) {
		case T_ASSIGN:
			return parseInitializationOfIdentifier;
		case T_EOL:
			return parseNoInitialization;
		default:
			return NULL;
		}

	case NT_GL_INITIALIZATION:
		switch (tokenState) {
		case T_ASSIGN:
			return parseInitializationOfGLIdentifier;
		case T_EOL:
			return parseNoInitialization;
		default:
			return NULL;
		}

	case NT_PARAMETERS:
		switch (tokenState) {
		case T_CLOSEBRACK:
			return parseNoParametersToFunction;
		case T_IDENTIFIER:
		case T_IDENTIFIER_HT:
			return parseFirstParameterToFunction;
		case T_FUNID:
			return parseErrorRedefinitionParatmeter;
		default:
			return NULL;
		}

	case NT_DEF_PARAMETERS:
		switch (tokenState) {
		case T_CLOSEBRACK:
			return parseNoParametersToDefinition;
		case T_IDENTIFIER:
		case T_IDENTIFIER_HT:
			return parseFirstParameterToDefinition;
		case T_FUNID:
			return parseErrorRedefinitionParatmeter;
		default:
			return NULL;
		}

	case NT_PARAMETER:
		switch (tokenState) {
		case T_IDENTIFIER:
		case T_IDENTIFIER_HT:
			return parseDeclarationOfParameterToFunction;
		default:
			return NULL;
		}

	case NT_NEXTPARAMETER:
		switch (tokenState) {
		case T_COMMA:
			return parseDeclarationOfNextParameterToFunction;
		case T_CLOSEBRACK:
			return parseEndOfParamsDeclaration;
		default:
			return NULL;
		}

	case NT_DEF_PARAMETER:
		switch (tokenState) {
		case T_IDENTIFIER:
		case T_IDENTIFIER_HT:
			return parseDefinitionOfParameterToFunction;
		default:
			return NULL;
		}

	case NT_DEF_NEXTPARAMETER:
		switch (tokenState) {
		case T_COMMA:
			return parseDefinitionOfNextParameterToFunction;
		case T_CLOSEBRACK:
			return parseEndOfParamsDefinition;
		default:
			return NULL;
		}

	case NT_DATATYPE:
		switch (tokenState) {
		case T_INTEGER:
			return parseIntegerType;
		case T_DOUBLE:
			return parseFloatType;
		case T_STRING:
			return parseStringType;
		default:
			return NULL;
		}

	case NT_CONST_VAL:
		switch (tokenState) {
		case T_STRINGVAL:
			return parseStringValue;
		case T_INTVAL:
			return parseIntegerValue;
		case T_DOUBLEVAL:
			return parseDoubleValue;
		default:
			return parseErrorExpectedConstant;
		}

/*	case NT_FOR_STATEMENTS:
		switch (tokenState) {
		case T_DIM:
		case T_IDENTIFIER:
		case T_FUNID:
		case T_SCOPE:
		case T_INPUT:
		case T_PRINT:
		case T_DO:
		case T_EOL:
		case T_RETURN:
		case T_FOR:
			return parseForStatement;
		case T_CONTINUE:
			return parseSkipIterationFor;
		case T_EXIT:
			return parseBreakLoopFor;
		case T_NEXT:
			return parseNextInFor;
		default:
			return NULL;
		}*/
	default:
		return NULL;
	}
}
