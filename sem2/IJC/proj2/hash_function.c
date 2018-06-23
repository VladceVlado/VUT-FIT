/*
 * Soubor:      hash_function.c
 * Projekt:     IJC-DU2, priklad 2)
 * Autor:       Vladimir Dusek, xdusek27
 * Skola:       VUT FIT 1BIT
 * Datum:       3/4/2017
 * Prekladac:   gcc 5.4.0
 * Popis:       Hashovaci funkce transformuje vyhledavaci klic,
 *              v nasem pripade slovo, na cislo (index pole).
 */


#include "hash_table.h"


/*
 * Hashovaci funkce, zadana
 */
unsigned int hash_function(const char *str) 
{
	unsigned int h = 0;
	const unsigned char *p;
	for (p = (const unsigned char*) str; *p != '\0'; p++)
		h = 65599 * h + *p;
	return h;
}