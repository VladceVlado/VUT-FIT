/*
 * Soubor:      htab_lookup_add.c
 * Projekt:     IJC-DU2, priklad 2)
 * Autor:       Vladimir Dusek, xdusek27
 * Skola:       VUT FIT 1BIT
 * Datum:       3/4/2017
 * Prekladac:   gcc 5.4.0
 * Popis:       V tabulce t vyhleda zaznam odpovidajici retezci key a
 *              pokud jej nalezne, vrati ukazatel na zaznam
 *              pokud nenalezne, automaticky prida zaznam a vrati ukazatel.
 */


#include "hash_table.h"


struct htab_listitem *htab_lookup_add(htab_t *t, char *key)
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
    struct htab_listitem *tmp_previous = NULL;

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
        tmp_previous = tmp;
    }

    /* kdyz se nasel klic, vratim adresu zaznamu */
    if (find)
        return tmp;

    /* kdyz se klic nenasel */
    else
    {
        /* alokace pameti pro novy zaznam */
        struct htab_listitem *new_item = (struct htab_listitem *) malloc(sizeof(struct htab_listitem));

        /* alokace pro zaznam se nezdarila -> funkce vrati NULL */
        if (new_item == NULL)
            return NULL;

        /* alokace pameti pro klic v zaznamu (pocet znaku klice + 1 pro '\0') */
        new_item->key = (char *) malloc((strlen(key) + 1) * sizeof(char));

        /* alokace pro klic se nezdarila -> funkce vrati NULL */
        if (new_item->key == NULL)
        {
            free(new_item); // je nutne uvolnit pamet po novem zaznamu
            return NULL;
        }

        strcpy(new_item->key, key); // nastaveni klice v zaznamu
        new_item->data = 1; // nastaveni poctu dat v zaznamu
        new_item->next = NULL; // posledni zaznam v seznamu ukazuje na NULL

        /* radek tabulky (seznam) je prazdny -> new_item bude prvni zaznam */
        if (tmp_previous == NULL)
            t->ptr[index] = new_item;

        /* v radku tabulky (seznamu) uz jsou nejake zaznamy, new_item navazu na konec */
        else
            tmp_previous->next = new_item;

        /* inkrementuje se pocet zaznamu v tabulce */
        t->n++;

        /* vratim adresu noveho zaznamu */
        return new_item;
	}
}