/*
 * Soubor:      htab_remove.c
 * Projekt:     IJC-DU2, priklad 2)
 * Autor:       Vladimir Dusek, xdusek27
 * Skola:       VUT FIT 1BIT
 * Datum:       3/4/2017
 * Prekladac:   gcc 5.4.0
 * Popis:       Vyhledani a zruseni zadane polozky, 
 *              vraci b == false pokud neexistuje.
 */


#include "hash_table.h"


bool htab_remove(htab_t *t, char *key)
{
	/* tabulka nebo klic neexistuji -> funkce vrati hned false */
	if (t == NULL || key == NULL)
        return false;

    /* vypocitani indexu pomoci hashovaci funkce % velikost pole */
    unsigned int index = hash_function(key) % htab_bucket_count(t);

    struct htab_listitem *tmp;
    struct htab_listitem *tmp_previous = NULL;

    /* klasicky cyklus pro prohledani celeho seznamu */
    for (tmp = t->ptr[index]; tmp != NULL; tmp = tmp->next)
    {
    	/* kdyz se klic shoduje */
        if (strcmp(key, tmp->key) == 0)
        {
            t->n--; // dekrementuje se pocet zaznamu v tabulce
            
            free(tmp->key); // uvolneni pameti po klici

            /* kdyz je tmp prvni polozka seznamu, musim navazat ukazatel z tabulky */
            if (tmp_previous == NULL)
                t->ptr[index] = tmp->next;
            /* jinak predchazejici polozka ukazuje na nasledujici */
            else
                tmp_previous->next = tmp->next;

            free(tmp); // uvolneni soucasne polozky iterace

            return true; // zadana polozka byla nalezena a smazana -> return true
        }
        /* ulozeni predchazejiciho zaznamu kvuli navazani seznamu */
        tmp_previous = tmp;
    }

    return false; // zadana polozka nebyla nalezena -> return false
}