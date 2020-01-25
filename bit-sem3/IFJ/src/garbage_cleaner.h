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


#ifndef _IFJ42_GARBAGE_CLEANER_H
#define _IFJ42_GARBAGE_CLEANER_H

#include "semantic_essentials.h"
#include "parser_essentials.h"

int registerParserGarbage(SemTools *tools);

#endif
