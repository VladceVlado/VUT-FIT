/**
 * project: Implementace překladače imperativního jazyka IFJ17
 * authors: Martin Hošala (xhosal00), Peter Kubov (xkubov06), Tomáš Kukaň (xkukan00), Vladimír Dušek (xdusek27)
 * ------------------------------------
 * @author Martin Hošala (xhosal00)
 */

#include "precedent_table.h"

void ptInit (tPTable *table)
{
	(*table)[T_PLUS][T_PLUS]       = J_REDUCE;
	(*table)[T_PLUS][T_MUL]        = J_REDUCE;
	(*table)[T_PLUS][T_BRC_L]      = J_SHIFT;
	(*table)[T_PLUS][T_BRC_R]      = J_REDUCE;
	(*table)[T_PLUS][T_FCE]        = J_NONE;
	(*table)[T_PLUS][T_ID]         = J_REDUCE;
	(*table)[T_PLUS][T_DOLLAR]     = J_SHIFT;
	(*table)[T_PLUS][T_COMMA]      = J_SHIFT;
	(*table)[T_PLUS][T_REL]        = J_SHIFT;
	(*table)[T_PLUS][T_DIV_INT]    = J_REDUCE;

	(*table)[T_DIV_INT][T_PLUS]       = J_SHIFT;
	(*table)[T_DIV_INT][T_MUL]        = J_REDUCE;
	(*table)[T_DIV_INT][T_BRC_L]      = J_SHIFT;
	(*table)[T_DIV_INT][T_BRC_R]      = J_REDUCE;
	(*table)[T_DIV_INT][T_FCE]        = J_NONE;
	(*table)[T_DIV_INT][T_ID]         = J_REDUCE;
	(*table)[T_DIV_INT][T_DOLLAR]     = J_SHIFT;
	(*table)[T_DIV_INT][T_COMMA]      = J_SHIFT;
	(*table)[T_DIV_INT][T_REL]        = J_SHIFT;
	(*table)[T_DIV_INT][T_DIV_INT]    = J_REDUCE;

	(*table)[T_MUL][T_PLUS]       = J_SHIFT;
	(*table)[T_MUL][T_MUL]        = J_REDUCE;
	(*table)[T_MUL][T_BRC_L]      = J_SHIFT;
	(*table)[T_MUL][T_BRC_R]      = J_REDUCE;
	(*table)[T_MUL][T_FCE]        = J_NONE;
	(*table)[T_MUL][T_ID]         = J_REDUCE;
	(*table)[T_MUL][T_DOLLAR]     = J_SHIFT;
	(*table)[T_MUL][T_COMMA]      = J_SHIFT;
	(*table)[T_MUL][T_REL]        = J_SHIFT;
	(*table)[T_MUL][T_DIV_INT]    = J_SHIFT;

	(*table)[T_BRC_L][T_PLUS]       = J_SHIFT;
	(*table)[T_BRC_L][T_MUL]        = J_SHIFT;
	(*table)[T_BRC_L][T_BRC_L]      = J_SHIFT;
	(*table)[T_BRC_L][T_BRC_R]      = J_NONE;
	(*table)[T_BRC_L][T_FCE]        = J_PUSH;
	(*table)[T_BRC_L][T_ID]         = J_NONE;
	(*table)[T_BRC_L][T_DOLLAR]     = J_SHIFT;
	(*table)[T_BRC_L][T_COMMA]      = J_SHIFT;
	(*table)[T_BRC_L][T_REL]        = J_SHIFT;
	(*table)[T_BRC_L][T_DIV_INT]    = J_SHIFT;

	(*table)[T_BRC_R][T_PLUS]       = J_REDUCE;
	(*table)[T_BRC_R][T_MUL]        = J_REDUCE;
	(*table)[T_BRC_R][T_BRC_L]      = J_PUSH;
	(*table)[T_BRC_R][T_BRC_R]      = J_REDUCE;
	(*table)[T_BRC_R][T_FCE]        = J_NONE;
	(*table)[T_BRC_R][T_ID]         = J_REDUCE;
	(*table)[T_BRC_R][T_DOLLAR]     = J_NONE;
	(*table)[T_BRC_R][T_COMMA]      = J_PUSH;
	(*table)[T_BRC_R][T_REL]        = J_REDUCE;
	(*table)[T_BRC_R][T_DIV_INT]    = J_REDUCE;

	(*table)[T_ID][T_PLUS]       = J_SHIFT;
	(*table)[T_ID][T_MUL]        = J_SHIFT;
	(*table)[T_ID][T_BRC_L]      = J_SHIFT;
	(*table)[T_ID][T_BRC_R]      = J_NONE;
	(*table)[T_ID][T_FCE]        = J_NONE;
	(*table)[T_ID][T_ID]         = J_NONE;
	(*table)[T_ID][T_DOLLAR]     = J_SHIFT;
	(*table)[T_ID][T_COMMA]      = J_SHIFT;
	(*table)[T_ID][T_REL]        = J_SHIFT;
	(*table)[T_ID][T_DIV_INT]    = J_SHIFT;

	(*table)[T_FCE][T_PLUS]       = J_SHIFT;
	(*table)[T_FCE][T_MUL]        = J_SHIFT;
	(*table)[T_FCE][T_BRC_L]      = J_SHIFT;
	(*table)[T_FCE][T_BRC_R]      = J_NONE;
	(*table)[T_FCE][T_FCE]        = J_NONE;
	(*table)[T_FCE][T_ID]         = J_NONE;
	(*table)[T_FCE][T_DOLLAR]     = J_SHIFT;
	(*table)[T_FCE][T_COMMA]      = J_SHIFT;
	(*table)[T_FCE][T_REL]        = J_SHIFT;
	(*table)[T_FCE][T_DIV_INT]    = J_SHIFT;

	(*table)[T_COMMA][T_PLUS]       = J_REDUCE;
	(*table)[T_COMMA][T_MUL]        = J_REDUCE;
	(*table)[T_COMMA][T_BRC_L]      = J_PUSH;
	(*table)[T_COMMA][T_BRC_R]      = J_REDUCE;
	(*table)[T_COMMA][T_FCE]        = J_NONE;
	(*table)[T_COMMA][T_ID]         = J_REDUCE;
	(*table)[T_COMMA][T_DOLLAR]     = J_NONE;
	(*table)[T_COMMA][T_COMMA]      = J_PUSH;
	(*table)[T_COMMA][T_REL]        = J_REDUCE;
	(*table)[T_COMMA][T_DIV_INT]    = J_REDUCE;

	(*table)[T_DOLLAR][T_PLUS]       = J_REDUCE;
	(*table)[T_DOLLAR][T_MUL]        = J_REDUCE;
	(*table)[T_DOLLAR][T_BRC_L]      = J_NONE;
	(*table)[T_DOLLAR][T_BRC_R]      = J_REDUCE;
	(*table)[T_DOLLAR][T_FCE]        = J_NONE;
	(*table)[T_DOLLAR][T_ID]         = J_REDUCE;
	(*table)[T_DOLLAR][T_DOLLAR]     = J_NONE;
	(*table)[T_DOLLAR][T_COMMA]      = J_NONE;
	(*table)[T_DOLLAR][T_REL]        = J_REDUCE;
	(*table)[T_DOLLAR][T_DIV_INT]    = J_REDUCE;

	(*table)[T_REL][T_PLUS]       = J_REDUCE;
	(*table)[T_REL][T_MUL]        = J_REDUCE;
	(*table)[T_REL][T_BRC_L]      = J_SHIFT;
	(*table)[T_REL][T_BRC_R]      = J_REDUCE;
	(*table)[T_REL][T_FCE]        = J_NONE;
	(*table)[T_REL][T_ID]         = J_REDUCE;
	(*table)[T_REL][T_DOLLAR]     = J_SHIFT;
	(*table)[T_REL][T_COMMA]      = J_SHIFT;
	(*table)[T_REL][T_REL]        = J_REDUCE;
	(*table)[T_REL][T_DIV_INT]    = J_REDUCE;

}
