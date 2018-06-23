/*
 * Soubor:      primes.c
 * Projekt:     IJC-DU1, priklad a)
 * Autor:       Vladimir Dusek, xdusek27
 * Skola:       VUT FIT 1BIT
 * Datum:       25/3/2017
 * Prekladac:   gcc 5.4.0
 * Popis:       testovaci program a, vypocet prvocisel od 0 do "LIMIT" 
 *              a vzestupne vytisknuti poslednich "PRINT"
 */


#include <stdio.h>
#include <stdlib.h>
#include "bit_array.h"
#include "eratosthenes.h"
#include "error.h"


#define LIMIT 303000000 // horni mez 303 milionu
#define PRINT 10 // tisknout poslednich 10 prvocisel


/* 
 * Tiskne poslednich "PRINT" prvocisel v intervalu <0,"LIMIT"> 
 */
void print_primes(bit_array_t array_name)
{
    unsigned long i, tmp, counter = 0;

    /* Zjisti index "PRINT". nejvetsiho prvocisla, ulozi do "i" */
    for (i = LIMIT; i > 1 && counter <= PRINT; i--)
    {
        tmp = ba_get_bit(array_name, i);
        if (tmp == 0)
            counter++;
    }
    /* Tiskne prvocisla */
    for (; i < LIMIT; i++)
    {
        tmp = ba_get_bit(array_name, i);
        if (tmp == 0)
            printf("%lu\n", i);
    }
}


/* 
 * MAIN
 * ba_create() - Vytvori bitove pole o velikosti "LIMIT"
 * eratosthenes() - Pomoci algoritmu Eratosthenova sita vypocita 
 * vsechna prvocisla az do horni hranice "LIMIT"
 * print_primes() - Poslednich "PRINT" prvocisel vytiskne 
 */
int main(void)
{
    ba_create(array_name, LIMIT);

    eratosthenes(array_name);

    print_primes(array_name);

    return EXIT_SUCCESS;
}
