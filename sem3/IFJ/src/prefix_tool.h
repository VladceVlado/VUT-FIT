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

#ifndef _IFJ42_PREFIX_TOOL_H
#define _IFJ42_PREFIX_TOOL_H

/**
 * @brief Creates uniq prefix
 *
 * @return Dynamically allocated uniq prefix, NULL in case of system error.
 */
char *createUniqPrefix();

#endif
