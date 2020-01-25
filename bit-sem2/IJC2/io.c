/*
 * Soubor:      io.c
 * Projekt:     IJC-DU2, priklad 2)
 * Autor:       Vladimir Dusek, xdusek27
 * Skola:       VUT FIT 1BIT
 * Datum:       3/4/2017
 * Prekladac:   gcc 5.4.0
 * Popis:       Funkce pro program wordcount.
 */


#include "io.h"


int get_word(char *s, int max, FILE *f)
{
	if (f == NULL)
        return EOF;

    int c, i = 0;

    /* preskoceni veskerych bilych znaku pred slovem */
    while (isspace(c = getc(f)))
        ;
    ungetc(c, f);

    /* nacteni slova znak po znaku */
    while (true)
    {
        c = getc(f);
    	/* pocet znaku slova presahl povolene maximum */
        if (i >= max)
        {
            print_war = true;
            s[i] = '\0';
            while (true)
            {
                c = getc(f);
                if (isspace(c))
                    return i;
                if (c == EOF)
                    return EOF;
            }
        }
        /* znak je bily znak -> je nacteno cele slovo */
        else if (isspace(c))
        {
            s[i] = '\0';
            return i;
        }
        /* znak je EOF -> jsme na konci souboru */
        else if (c == EOF)
        {
            s[i] = '\0';
            return EOF;
        }
        /* znak je bezny znak -> je ulozen do bufferu */
        else
            s[i] = c;
        i++;
    }

    return EOF;
}