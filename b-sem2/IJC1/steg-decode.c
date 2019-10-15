/*
 * Soubor:      steg-decode.c
 * Projekt:     IJC-DU1, priklad b)
 * Autor:       Vladimir Dusek, xdusek27
 * Skola:       VUT FIT 1BIT
 * Datum:       25/3/2017
 * Prekladac:   gcc 5.4.0
 * Popis:       testovaci program b, dekodovani zpravy
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "ppm.h"
#include "bit_array.h"
#include "eratosthenes.h"


/*
 * MAIN
 * Dekodovani tajne zpravy z obrazku jehoz nazev je prvni
 * a jediny argument programu
 */
int main(int argc, const char *argv[])
{
    /* Neplatny pocet argumentu */
    if (argc != 2)
    {
        error_msg("Chybne spusteni programu\n");
        return EXIT_FAILURE;
    }

    /* Nacteni dat ze souboru a jejich ulozeni do struktury */
    struct ppm *picture = ppm_read(argv[1]);

    /* Chyba ve funkci "ppm_read" */
	if (picture == NULL)
        error_msg("Chyba v cteni souboru %s\n", argv[1]);

    /* Deklarace bitoveho pole a vypocitani prvociselnych indexu */
    ba_create(array, LIMIT);
    eratosthenes(array);

    /* Cyklus pro dekodovani zpravy */
    short bitvalue; // hodnota aktualniho bitu
    short tmpchar = 0; // buffer pro nacteni znaku
    short counterbits = 0; // pocitadlo pro vypsani znaku
    bool null = false; // retezec konci znakem '\0'

    for (unsigned long i = 0; i < LIMIT; i++)
    {
        /* neni prvociselny index, skocim rovnou na dalsi iteraci */
        if (ba_get_bit(array, i))
            continue;

        bitvalue = picture->data[i] & 1; // ulozeni hodnoty klicoveho bitu
        tmpchar |= (bitvalue << counterbits); // nastaveni bitu v bufferu
        counterbits++; // zapamatovani kolik uz mame bitu

        /* Mame osm bitu -> muzeme vypsat znak */
        if (counterbits == CHAR_BIT)
        {
            printf("%c", tmpchar);

            /* Retezec konci '\0', mame celou zpravu */
            if (tmpchar == '\0')
            {
                printf("\n");
                null = true;
                break;
            }
            /* Vynulovani pocitadla a bufferu */
            counterbits = 0;
            tmpchar = 0;
        }
    }

    /* Zakodovana zprava nebyla ukoncena znakem '\0' */
    if (!null)
        error_msg("Zakodovana zprava neni ukoncena znakem '\\0'\n");

    /* Uvolneni pameti po strukture */
    free(picture);

	return EXIT_SUCCESS;
}
