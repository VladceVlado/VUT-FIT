/*
 * Soubor:      htab_find.c
 * Projekt:     IJC-DU2, priklad 2)
 * Autor:       Vladimir Dusek, xdusek27
 * Skola:       VUT FIT 1BIT
 * Datum:       3/4/2017
 * Prekladac:   gcc 5.4.0
 * Popis:       V tabulce t vyhleda zaznam odpovidajici retezci key a
 *              pokud jej nalezne, vrati ukazatel na zaznam
 *              pokud nenalezne, vrati NULL.
 */


#include "hash_table.h"


struct htab_listitem *htab_find(htab_t *t, char *key)
{
	/* tabulka nebo klic neexistuji -> funkce vrati NULL */
    if (t == NULL || key == NULL)
        return NULL;

    /* vypocitani indexu pomoci hashovaci funkce % velikost pole */
    unsigned long index = hash_function(key) % htab_bucket_count(t);

    /* kvuli vraceni/vytvoreni noveho zaznamu
     * true -> nasel, vratit
     * false -> nenasel, vytvorit a vratit */
    bool find = false;

    struct htab_listitem *tmp;
    //struct htab_listitem *tmp_previous = NULL;

    /* klasicky cyklus pro prohledani celeho seznamu */
    for (tmp = t->ptr[index]; tmp != NULL; tmp = tmp->next)
    {
        /* kdyz se klic shoduje */
        if (strcmp(key, tmp->key) == 0)
        {
            tmp->data++; // inkrementuju pocet shod
            find = true; // nasla se shoda
            break;
        }
        /* ulozeni predchazejiciho zaznamu kvuli navazani seznamu */
        //tmp_previous = tmp;
    }

    /* kdyz se nasel klic, vratim adresu zaznamu */
    if (find)
        return tmp;

    /* kdyz se klic nenasel */
    else
    	return NULL;
}


