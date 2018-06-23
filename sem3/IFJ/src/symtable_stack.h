/**
 * project: Implementace překladače imperativního jazyka IFJ17
 * authors: Martin Hošala (xhosal00), Peter Kubov (xkubov06), Tomáš Kukaň (xkukan00), Vladimír Dušek (xdusek27)
 * ------------------------------------
 * @author  Vladimír Dušek (xdusek27)
 * @date	November, 2017
 * @brief	Stack of symtables for syntax analysis.
 * @file    symtable_stack.h
 */


#ifndef SYM_STACK_HASH_TABLE_H
#define SYM_STACK_HASH_TABLE_H


/**
 * @brief      Type representing value of stack of symtables.
 */
typedef struct symtableStack STStack;


/**
 * @brief      Allocates memory required by stack of symtables.
 *
 * @return     On success returns pointer to the memory reserved for stack of symtables.
 */
STStack *newSTStack();


/**
 * @brief      Frees memory of the stack of symtables and its elements.
 *
 * @param      stack	Pointer to the stack of symtables to be freed.
 */
void freeSTStack(STStack *stack);


/**
 * @brief      Initializes stack of symtables passed to function as parameter.
 *
 * @param      stack	Represents stack of symtables to be initialized.
 */
void stStackInit(STStack *stack);


/**
 * @brief      Checks if stack of symtables passed as argument is empty (has 0 elements).
 *
 * @param      stack	Stack of symtables to be checked.
 *
 * @return     True if stack of symtables is empty, False otherwise.
 */
int stStackIsEmpty(STStack *stack);


/**
 * @brief      Pops stack of symtables, frees stack's top.
 *
 * @param      stack	Stack of symtables to be popped.
 */
void stStackPop(STStack *stack);


/**
 * @brief      Exposes value of the top element of the stack of symtables.
 *
 * @param      stack	Specifies stack of symtables its top value is going to be exposed.
 *
 * @return     Value of the top element of the stack of symtables, NULL when stack is empty.
 */
htab_t *stStackTop(STStack *stack);


/**
 * @brief      Push new element of the stack of symtables.
 *
 * @param      stack	Stack of symtables that is going to have one more element.
 * @param      table	The new element, which will be pushed to the stack.
 */
void stPushTable(STStack *stack, htab_t *table);


/**
 * @brief      Search for identifier in every symtables of the stack.
 *
 * @param      string	Name of identifier.
 * @param      stack	Specifies stack of symtables which should be searched.
 *
 * @return     Item of symtable if identifier was found, NULL otherwise.
 */
htab_item_t *searchID(char *string, STStack *stack);


#endif
