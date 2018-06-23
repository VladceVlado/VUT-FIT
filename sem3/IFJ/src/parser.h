/**
 * project: Implementace překladače imperativního jazyka IFJ17
 * authors: Martin Hošala (xhosal00), Peter Kubov (xkubov06), Tomáš Kukaň (xkukan00), Vladimír Dušek (xdusek27)
 * ------------------------------------
 * @Author	Peter Kubov (xkubov06@stud.fit.vutbr.cz)
 * @date	November, 2017
 * @brief	Syntax analysis for IFJ project.
 *
 * Modul parser provides analysis of lightweighted FreeBasic
 * programming language. Modul cooporates with other modules
 * written for this purpose and eventually generate IFJcoode17
 * on stdout during run.
 */

#ifndef IFJ_42_PARSER_H
#define IFJ_42_PARSER_H

/**
 * @brief	Parses code passed to this function as paramater. Generates output on stdout.
 * @ingroup	parser
 *
 * @param [in]	codeFile	File which contains FreeBasic code.
 * @retval	on success return 0 positive number specifying error otherwise.
 *
 * Example Usage:
 * 	parseCode(stdin);
 */
int parseCode(FILE *file);

#endif
