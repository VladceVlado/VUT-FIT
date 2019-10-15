/**
 * project: Implementace překladače imperativního jazyka IFJ17
 * authors: Martin Hošala (xhosal00), Peter Kubov (xkubov06), Tomáš Kukaň (xkukan00), Vladimír Dušek (xdusek27)
 * ------------------------------------
 * @Author	Peter Kubov (xkubov06@stud.fit.vutbr.cz)
 * @date	November, 2017
 * @brief	Essential structures for semantic analysis for IFJ project.
 *
 * Module provides implementation of stack used in semantic analysis.
 */

#ifndef _IFJ42_SEMANTIC_ESSENTIALS_H
#define _IFJ42_SEMANTIC_ESSENTIALS_H

#include "keywords.h"
#include "symtable.h"
#include "symtable_stack.h"
#include "precedent.h"

/**
 * @brief	Type representing value of element of semantic stack.
 * @ingroup	parser
 */
typedef struct elemval {
	enum {SA_ID_PTR, SA_PREFIX} state; /// represents state of value
	union {
		htab_item_t *prefix; /// prefix is used when prefix is required to be stored
		htab_item_t *item; /// item is required when pointer to the variable/function is required later in analysis
	} val; /// differentiation of required item to be stored
} *SAElemValPtr;

/**
 * @brief	Type representing one element of semantic stack.
 * @ingroup	parser
 */
typedef struct semElement {
	SAElemValPtr spec; // represents value of the element
	struct semElement *prev; /// pointer on previous element
} *SEMElemPtr;

/**
 * @brief	Type representing stack required by semantic analysis.
 * @ingroup	parser
 */
typedef struct semstack {
	SEMElemPtr top; /// Pointer to top element
} *SAStack;

/**
 * @brief	Allocates memory required by semantic stack.
 * @ingroup	parser
 *
 * @return	On success returns pointer to memory reserved for semantic stack.
 */
SAStack newSAStack();

/**
 * @brief	Frees memory of semantic stack and it's elements.
 * @ingroup	parser
 *
 * @param	stack	Pointer to semantic stack to be freed.
 */
void freeSAStack(SAStack stack);

/**
 * @brief	Initializes semantic stack passed to function as parameter.
 * @ingroup	parser
 *
 * @param	stack	Represents stack to be initialized.
 */
void saSinit(SAStack stack);

/**
 * @brief	Checks if stack passed as argument is empty (has 0 elements)
 * @ingroup	parser
 *
 * @param	stack	Stack to be checked.
 *
 * @return	0 if stack is not empty non-zero velue otherwise.
 */
int saSIsEmpty(SAStack stack);

/**
 * @brief	Universal function to create new element of stack.
 * @ingroup	parser
 *
 * @param	stack	Stack that is going to have one more element.
 * @param	elem	Specification of element to be pushed to stack.
 *
 * @error	When error occoures errno is set appropriately.
 */
void saSPush(SAStack stack, SAElemValPtr elem);


/**
 * @brief	Creates new element of value-type prefix and pushes it on stack.
 * @ingroup	parser
 *
 * @param	stack	Stack that is going to have one more element.
 * @param	prefix	Specification of prefix to be pushed to stack.
 *
 * @error	When error occoures errno is set appropriately.
 */
void saSPushPrefix(SAStack stack, htab_item_t *prefix);

/**
 * @brief	Creates new element representing variable/function pointer and pushes it on stack.
 * @ingroup	parser
 *
 * @param	stack	Stack that is going to have one more element.
 * @param	item	Specification of variable/function pointer to be pushed to stack.
 *
 * @error	When error occoures errno is set appropriately.
 */
void saSPushDefPtr(SAStack stack, htab_item_t *item);

/**
 * @brief	Pops stack, frees semantic stack's top.
 * @ingroup	parser
 *
 * @param	stack	Stack to be popped.
 */
void saSPop(SAStack stack);

/**
 * @brief	Exposes value of the top element the stack.
 * @ingroup	parser
 *
 * @param	stack	Specifies stack thats top value is going to be exposed..
 *
 * @return	Value of the top element of the stack, NULL when stack is empty. 
 */
SAElemValPtr saSTop(SAStack stack);

#endif
