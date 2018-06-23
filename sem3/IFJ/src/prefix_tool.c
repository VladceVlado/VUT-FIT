/**
 * project: Implementace překladače imperativního jazyka IFJ17
 * authors: Martin Hošala (xhosal00), Peter Kubov (xkubov06), Tomáš Kukaň (xkukan00), Vladimír Dušek (xdusek27)
 * ------------------------------------
 * @Author	Peter Kubov (xkubov06@stud.fit.vutbr.cz)
 * @date	November, 2017
 * @brief	Modul generating prefixes for IFJ project.
 *
 * Module generates unique prefix that is used by scope extention in output code generation.
 */

#include <stdlib.h>
#include <stdio.h>

#include"prefix_tool.h"


char *createUniqPrefix()
{
	static int prefix_cnt = 0;

	char *buff = calloc(1, sizeof(int)*8 + 5);
	if (sprintf(buff, "IFJ42_%d", ++prefix_cnt) < 0)
		return NULL;

	return buff;
}
