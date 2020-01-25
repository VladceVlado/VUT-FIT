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

#ifndef _IFJ42_SEMANTIC_RULES_
#define _IFJ42_SEMANTIC_RULES_

#include "semantic_essentials.h"
#include "parser_tools.h"

/**
 * @brief	Type representing action of semantic analysis.
 * @ingroup	parser
 *
 * Every action is created as independent function which takes
 * tools required for semantic analysis as argument.
 */
typedef void (*TSemRule)(SemTools *);

/**
 * @brief	Creates symbol table an instersts it on top of the symbol table stack.
 *
 * Function acts as smenatic action that's role is to create new symbol table
 * for new level of variables definitions.
 *
 * @param	tools	set of tools used for semantic snalysis.
 */
void pushSymtable(SemTools *tools);

/**
 * @brief	Removes symbol table from top of the symbol table stack.
 *
 * Function acts as smenatic action that's role is to remove symbol table
 * from top to return to previous level of variables definitions.
 *
 * @param	tools	set of tools used for semantic snalysis.
 */
void popSymtable(SemTools *tools);

/// Checks if program has one, uniq scope.
void checkOrMakeUniqMain(SemTools *tools);
/// Creates awareness of main scope.
void createMain(SemTools *tools);

/// Declares function in symbol table.
void generateDeclaration(SemTools *tools);
/// Generates 3AK of generation label of function.
void createFunctionLabel(SemTools *tools);
/// Gives parsed function saved in tools datatype of actual parsed token.
void giveParsedFunctionType(SemTools *tools);
/// Saves previously declared function in token to tools.
void saveFunctionToParse(SemTools *tools);
/// Checks if parsed function has datatype of actual parsed token.
void checkFunctionDatatype(SemTools *tools);
/// Declare function in token saved as string and saves pointer to symbolic table in tools.
void declareSaveFunctionToParse(SemTools *tools);

/// Calls precedent module to analyse expression.
void evaluateExpression(SemTools *tools);
/// Declares variable and saves it tempoorary into tools.
void declareIdentifierSave(SemTools *tools);
/// Assigns type to vvariable saved in tools.
void assignTypeToIdentifier(SemTools *tools);

/// After evaluating expression by precedent module, result is on stack.
void assignEvaluatedExpression(SemTools *tools);
/// After finishing parsing of variable leave identifier from tools.
void leaveIdentifierPtr(SemTools *tools);
/// When assignment is parsed, identifier is required to be saved from token for later use.
void saveIdentifierToParse(SemTools *tools);

/// Calls precedent module and prints result from stack.
void evaluateExpressionForPrint(SemTools *tools);
/// Calls precedent module and generates while condition.
void evaluateExpressionForWhile(SemTools *tools);

/// For loop extension (continue)
void generateIterationSkip(SemTools *tools);
/// For loop extension (exit)
void generateLoopExit(SemTools *tools);

/// While ends when 'loop' keyword is on input
void generateWhileEnd(SemTools *tools);

/// Checks if return is possible
void isInFunction(SemTools *tools);
/// Generates instruction RETURN
void generateReturnFunction(SemTools *tools);

/// Calls precedent analysis module and makes addditional steps
void evaluateExpressionForIf(SemTools *tools);
/// If-part of condition  ends with keyword 'end'
void generateIfEnd(SemTools *tools);
/// Starts else-parst of condition
void generateElse(SemTools *tools);
/// Else-part of condition ends with keyword 'end'
void generateEndOfElse(SemTools *tools);

/// Generates instructions for input
void generateInputRequest(SemTools *tools);

/// For loops extension
void declareIterator(SemTools *tools);
/// For loops extension
void generateForLoop(SemTools *tools);
/// For loops extension
void generateEndFor(SemTools *tools);

/// When parametrs of functions are finished analysation then parameters must be generated
void terminateParameterDecDef(SemTools *tools);

/// Parameter declaration
void declareParam(SemTools *tools);
/// Parameter definition
void defineParam(SemTools *tools);

/// Generate instructions to assign top of stack to identifier
void generateAssignmet(SemTools *tools);

/**
 * To prevent LL-grammar having too many rules this semantic action analyses token
 * and modifies it to suit parameters definition. This means when token of type
 * pointer to symbol table comes it is analysed if it is not function or previously
 * declared parameter and when token is suitable it's state is changed to 
 * identifier string and then this token is declared.
 */
void hackIdentifier(SemTools *tools);
/// Inserts parameter to symbol table 
void createParameterSymbol(SemTools *tools);
/// After function call result is on top of stack but it is not being assigned nowhere so it must be popped
void popFromStack(SemTools *tools);

/**
 * To unify process of evaluation expression this semantic action pretend
 * function to be identifier so evaluate expression smeantic action can take
 * it's datatype and give it to precedent analysis module as expected return
 * type.
 */
void saveFunctionAsIdentifierToParse(SemTools *tools);

/// When function has no return implicit return must be provided 
void generateImplicitReturn(SemTools *tools);
/// When attemt to redefine function/variable this semantic action takes place.
void shoutErrorRedefiniton(SemTools *tools);
/// When attempt to assign to identifier which was not defined this semantic action takes place.
void shoutErrorDefinition(SemTools *tools);

/// Pushes prefix to semantic stack.
void pushPrefix(SemTools *tools);
/// Pops prefix from semantic stack.
void popPrefix(SemTools *tools);

/// Generates protextion of condition block on generated code level.
void generateIfProtection(SemTools *tools);

/**
 * Generates protection of guard of definitions on generated code level.
 * Protection is generated when returning from function, so that stack doues not contain guards.
 */
void protectGuard(SemTools *tools);

/// Increment guard protection
void incProtectionOfGuard(SemTools *tools);
/// Decrement guard protection
void decProtectionOfGuard(SemTools *tools);

/// Pops guard of variables definition.
void popGuard(SemTools *tools);
/// Generates declaration on global scale.
void generateGlobalDeclaration(SemTools *tools);
/// Generates pure declaration for static variables.
void generatePureDeclaration(SemTools *tools);

/// Generates start of guard code.
void generateGuard(SemTools *tools);
/// Generates end of guard code.
void finnishGuard(SemTools *tools);

/// Implicit initialization of variables.
void implicitInitialization(SemTools *tools);

/// Assignes constant when initializing shared/static variable.
void assignConstant(SemTools *tools);
/// Semantic action that takes place when constant is expected, but not provided.
void shoutExpectedConstant(SemTools *tools);

/// Searches for declared but undefined function in global symbol table.
void checkEverythingsDefined(SemTools *tools);

#endif
