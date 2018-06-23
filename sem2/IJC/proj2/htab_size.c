/*
 * Soubor:      htab_size.c
 * Projekt:     IJC-DU2, priklad 2)
 * Autor:       Vladimir Dusek, xdusek27
 * Skola:       VUT FIT 1BIT
 * Datum:       3/4/2017
 * Prekladac:   gcc 5.4.0
 * Popis:       Vrati pocet polozek v seznamu (pocet sloupcu tabulky v danem radku).
 */


#include "hash_table.h"


unsigned long htab_size(htab_t *t)
{
	return t->n;
}