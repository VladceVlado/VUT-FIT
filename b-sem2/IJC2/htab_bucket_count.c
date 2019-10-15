/*
 * Soubor:      htab_bucket_count.c
 * Projekt:     IJC-DU2, priklad 2)
 * Autor:       Vladimir Dusek, xdusek27
 * Skola:       VUT FIT 1BIT
 * Datum:       3/4/2017
 * Prekladac:   gcc 5.4.0
 * Popis:       Vrati pocet prvku pole (pocet radku tabulky).
 */


#include "hash_table.h"


unsigned long htab_bucket_count(htab_t *t)
{
    return t->arr_size;
}