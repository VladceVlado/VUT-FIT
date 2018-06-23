/*
 * Soubor:      htab_foreach.c
 * Projekt:     IJC-DU2, priklad 2)
 * Autor:       Vladimir Dusek, xdusek27
 * Skola:       VUT FIT 1BIT
 * Datum:       3/4/2017
 * Prekladac:   gcc 5.4.0
 * Popis:       Volani funkce func pro kazdy prvek.
 */


#include "hash_table.h"


void htab_foreach(htab_t *t, void (*function)(char *, unsigned long))
{
    if (t == NULL) // neplatny ukazatel na tabulku
       return;

    unsigned long i;
    unsigned long arr_size = htab_bucket_count(t);
    struct htab_listitem *tmp;

    for (i = 0; i < arr_size; i++) // iterovani celym polem
    {
        for (tmp = t->ptr[i]; tmp != NULL; tmp = tmp->next) // iterovani celym seznamem
        {
            function(tmp->key, tmp->data); // pro kazdou polozku zavolej funkci
        }
    }
}