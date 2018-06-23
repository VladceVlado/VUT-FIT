/*
 * Soubor:      htab_move.c
 * Projekt:     IJC-DU2, priklad 2)
 * Autor:       Vladimir Dusek, xdusek27
 * Skola:       VUT FIT 1BIT
 * Datum:       3/4/2017
 * Prekladac:   gcc 5.4.0
 * Popis:       Move konstruktor: vytvoreni a inicializace tabulky daty z tabulky t2,
 *              t2 nakonec zustane prazdna a alokovana;
 */


#include "hash_table.h"


htab_t *htab_move(unsigned long newsize, htab_t *t2)
{
	htab_t *new_htab = htab_init(newsize);

    if (new_htab == NULL || t2 == NULL)
        return NULL;

    unsigned long i;
    unsigned long index;
    unsigned long t2_arr_size = htab_bucket_count(t2);
    struct htab_listitem *tmp;
    struct htab_listitem *save;
    struct htab_listitem *save_first;

    /* cyklus pro proiterovani celym polem */
    for (i = 0; i < t2_arr_size; i++)
    {
        /* kdyz je seznam prazdny skoci na dalsi iteraci */
        if (t2->ptr[i] == NULL)
            continue;
        /* cyklus pro prolezeni celeho seznamu */
        for (tmp = t2->ptr[i]; tmp != NULL; )
        {
            index = hash_function(tmp->key) % htab_bucket_count(new_htab);
            save = tmp->next;

            if (new_htab->ptr[index] == NULL)
            {
                new_htab->ptr[index] = tmp;
                new_htab->ptr[index]->next = NULL;
            }
            else
            {
                save_first = new_htab->ptr[index];
                new_htab->ptr[index] = tmp;
                tmp->next = save_first;
            }

            tmp = save;
            new_htab->n++;
            t2->n--;
        }
        t2->ptr[i] = NULL; // zahozeni ukazatele
    }

    return new_htab;
}