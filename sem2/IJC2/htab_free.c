/*
 * Soubor:      htab_free.c
 * Projekt:     IJC-DU2, priklad 2)
 * Autor:       Vladimir Dusek, xdusek27
 * Skola:       VUT FIT 1BIT
 * Datum:       3/4/2017
 * Prekladac:   gcc 5.4.0
 * Popis:       Destruktor - zrusení tabulky (vola htab_clear()).
 */


#include "hash_table.h"


void htab_free(htab_t *t)
{
	htab_clear(t); // uvolni vsechny polozky ve vsech seznamech
    free(t); // uvolni pole seznamu
    t = NULL; // zahodi ukazatel
}