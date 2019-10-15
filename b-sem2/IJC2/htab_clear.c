/*
 * Soubor:      htab_clear.c
 * Projekt:     IJC-DU2, priklad 2)
 * Autor:       Vladimir Dusek, xdusek27
 * Skola:       VUT FIT 1BIT
 * Datum:       3/4/2017
 * Prekladac:   gcc 5.4.0
 * Popis:       Zruseni vsech polozek, tabulka zustane prazdna.
 */


#include "hash_table.h"


/*
 * 
 */
void htab_clear(htab_t *t)
{
	if (t == NULL) // tabulka je prazdna -> konec
        return;

    unsigned long i;
    unsigned long arr_size = htab_bucket_count(t);
    struct htab_listitem *tmp;

    /* cyklus pro proiterovani celym polem */
    for (i = 0; i < arr_size; i++)
    {
        /* kdyz je seznam prazdny skoci na dalsi iteraci */
        if (t->ptr[i] == NULL)
            continue;

        /* cyklus pro prolezeni celeho seznamu */
        for (tmp = t->ptr[i]; tmp != NULL; tmp = tmp->next)
        {
            t->n--; // dekrementuje se pocet zaznamu v tabulce
            free(tmp->key); // uvolneni pameti po klici
            free(tmp); // uvolneni pameti po polozce
        }
        t->ptr[i] = NULL; // zahozeni ukazatele
    }
}


