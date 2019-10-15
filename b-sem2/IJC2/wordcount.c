/*
 * Soubor:      wordcount.c
 * Projekt:     IJC-DU2, priklad 2)
 * Autor:       Vladimir Dusek, xdusek27
 * Skola:       VUT FIT 1BIT
 * Datum:       3/4/2017
 * Prekladac:   gcc 5.4.0
 * Popis:       Program wordcount.
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "hash_table.h"
#include "io.h"


#define MAX_LENGHT 127
#define TAB_SIZE 65536


bool print_war = false;


/* vytiskne klic a data kazde polozky tabulky */
void print_htab(char *key, unsigned long data)
{
    printf("%s \t %ld\n", key, data);
}


int main(void)
{
    /* vytvori tabulku */
    htab_t *htab = htab_init(TAB_SIZE);

    if (htab == NULL)
    {
        fprintf(stderr, "Chyba! Nepodarilo se alokovat pamet pro tabulku.\n");
        return EXIT_FAILURE;
    }

    char buffer[MAX_LENGHT + 1] = {0}; // deklarace bufferu
    int ret;

    /* cte vstup do EOF */
    do
    {
    	ret = get_word(buffer, MAX_LENGHT, stdin);

        /* v bufferu neni zadny "poradny" znak -> dalsi iterace nebo konec */
        if (strlen(buffer) < 1)
        {
            if (ret == EOF)
                break;
            continue;
        }
        /* prida slovo v bufferu do tabulky */
        if (htab_lookup_add(htab, buffer) == NULL)
        {
            fprintf(stderr, "Chyba! Nepodarilo se alokovat pamet pro polozku v tabulce.\n");
            htab_free(htab);
            return EXIT_FAILURE;
        }
        /* get_word vratilo EOF -> konec cyklu */
        if (ret == EOF)
            break;
    } while (ret != EOF);

    putchar('\n'); // odradkovani
    htab_foreach(htab, print_htab); // vypsani klicu a dat kazde polozky v tabulce
    htab_free(htab); // uvolneni tabulky

    /* vypsani varovani v pripade zkraceni slov/a */
    if (print_war)
        fprintf(stderr, "Varovani! Nektera slova byla prilis dlouha a musela byt zkracena na %d znaku\n", MAX_LENGHT);

    return EXIT_SUCCESS;
}