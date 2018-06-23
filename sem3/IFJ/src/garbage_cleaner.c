/**
 * project: Implementace překladače imperativního jazyka IFJ17
 * authors: Martin Hošala (xhosal00), Peter Kubov (xkubov06), Tomáš Kukaň (xkukan00), Vladimír Dušek (xdusek27)
 * ------------------------------------
 * @Author	Peter Kubov (xkubov06@stud.fit.vutbr.cz)
 * @date	November, 2017
 * @brief	Garbage cleaning
 *
 * Module provides autmatic garbage cleaning at exit.
 */

#include <stdlib.h>

#include "debug_ifj.h"
#include "garbage_cleaner.h"

SemTools *IFJ_17_TOOLS = NULL;

void cleanGarbage()
{
	DEBUG_MESSAGE("cleaning parser garbage");

	if (IFJ_17_TOOLS != NULL) {
		if (IFJ_17_TOOLS->inScopeTables != NULL) {
			freeSTStack(IFJ_17_TOOLS->inScopeTables);
		}

		if (IFJ_17_TOOLS->token != NULL) {
			freeToken(IFJ_17_TOOLS->token);
		}

		if (IFJ_17_TOOLS->semanticStack != NULL) {
			freeSAStack(IFJ_17_TOOLS->semanticStack);
		}

		if (IFJ_17_TOOLS->prefixZombieTmp != NULL) {
			if (IFJ_17_TOOLS->prefixZombieTmp->name != NULL)
				free(IFJ_17_TOOLS->prefixZombieTmp->name);
			if (IFJ_17_TOOLS->prefixZombieTmp->prefix != NULL)
				free(IFJ_17_TOOLS->prefixZombieTmp->prefix);
			free(IFJ_17_TOOLS->prefixZombieTmp);
		}

		if (IFJ_17_TOOLS->iList != NULL)
			iListDestroy(IFJ_17_TOOLS->iList);
	
		free(IFJ_17_TOOLS);
		IFJ_17_TOOLS = NULL;
	}
}

int registerParserGarbage(SemTools *tools)
{
	int err = 0;
	if (tools->inScopeTables == NULL)
		err++;
	if (tools->token == NULL)
		err++;
	if (tools->iList == NULL)
		err++;
	if (tools->prefixZombieTmp == NULL)
		err++;

	IFJ_17_TOOLS = tools;
	atexit(cleanGarbage);
	
	return err;
}
