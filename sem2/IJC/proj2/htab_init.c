/*
 * Soubor:      htab_init.c
 * Projekt:     IJC-DU2, priklad 2)
 * Autor:       Vladimir Dusek, xdusek27
 * Skola:       VUT FIT 1BIT
 * Datum:       3/4/2017
 * Prekladac:   gcc 5.4.0
 * Popis:       Konstruktor - vytvoreni a inicializace tabulky.
 */


#include "hash_table.h"


htab_t *htab_init(unsigned long size)
{
	// alokuje pamet pro hashovaci tabulku (htab_t) a pole ukazatelu velikosti "size"
	htab_t *htab = (htab_t *) malloc(sizeof(htab_t) + size * sizeof(struct htab_listitem *));

	if (htab == NULL) // nepodarilo se alokovat pamet - vrati NULL
        return NULL;

    htab->arr_size = size; // nastavi velikost tabulky na "size"
    htab->n = 0; // nastavi pocet zaznamu na 0

    for (unsigned long i = 0; i < size; i++) // nastavi vsechny ukazatele v poli na NULL
        htab->ptr[i] = NULL;

    return htab; // vrati adresu zacatku alokovane pameti
}
