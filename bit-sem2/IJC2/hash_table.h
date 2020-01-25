/*
 * Soubor:      hash_table.h
 * Projekt:     IJC-DU2, priklad 2)
 * Autor:       Vladimir Dusek, xdusek27
 * Skola:       VUT FIT 1BIT
 * Datum:       3/4/2017
 * Prekladac:   gcc 5.4.0
 * Popis:       Hlavickovy soubor pro vsechny funkce hashovaci tabulky.
 */


/* Podmineny preklad _HASH_TABLE_H */
#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>


/* Polozka v tabulce [key,data,next] */
struct htab_listitem 
{
    char *key; // ukazatel na dymanicky alokovany retezec
    unsigned long data; // pocet vyskytu retezce
    struct htab_listitem *next; // ukazatel na dalsi zaznam
} ;


/* Hashovaci tabulka */
typedef struct
{
    unsigned long arr_size; // velikost pole ukazatelu na struct htab_listitem
    unsigned long n;  // aktualni pocet polozek [key,data,next]
    struct htab_listitem *ptr[]; // pole ukazatelu na struct htab_listitem
} htab_t;


/* Funkcni prototypy */
unsigned int hash_function(const char *str);

htab_t *htab_init(unsigned long size);

htab_t *htab_move(unsigned long newsize, htab_t *t2);

unsigned long htab_size(htab_t *t);

unsigned long htab_bucket_count(htab_t *t);

struct htab_listitem *htab_lookup_add(htab_t *t, char *key);

struct htab_listitem *htab_find(htab_t *t, char *key);

void htab_foreach(htab_t *t, void (*function)(char *, unsigned long));

bool htab_remove(htab_t *t, char *key);

void htab_clear(htab_t *t);

void htab_free(htab_t *t);


/* Konec podmineneho prekladu _HASH_TABLE_H */
#endif