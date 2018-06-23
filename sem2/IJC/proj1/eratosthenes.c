/*
 * Soubor:      eratosthenes.c
 * Projekt:     IJC-DU1, priklad a)
 * Autor:       Vladimir Dusek, xdusek27
 * Skola:       VUT FIT 1BIT
 * Datum:       25/3/2017
 * Prekladac:   gcc 5.4.0
 * Popis:       algoritmus Eratosthenovo sito pro vypocet prvocisel
 *              do zadane horni meze
 */


#include <math.h>
#include "bit_array.h"
#include "eratosthenes.h"


/*
 * Vypocet prvocisel do zadane meze pomoci algoritmu Eratosthenova sita
 */
void eratosthenes(bit_array_t array_name)
{
	BA_SET_BIT_(array_name, 0, 1);
	ba_set_bit(array_name, 1, 1);

	unsigned long limit = sqrt(ba_size(array_name));
	unsigned long i, j, tmp;

    for (i = 2; i < limit; i++)
    {
        tmp = ba_get_bit(array_name, i);
        if (tmp == 0)
        {
            for (j = i * i; j < ba_size(array_name); j += i)
                ba_set_bit(array_name, j, 1);
        }
    }
}
