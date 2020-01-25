/*
 * Soubor:      error.c
 * Projekt:     IJC-DU1, priklad a), b)
 * Autor:       Vladimir Dusek, xdusek27
 * Skola:       VUT FIT 1BIT
 * Datum:       25/3/2017
 * Prekladac:   gcc 5.4.0
 * Popis:       vypis chybovych a varovnych zprav
 */


#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "error.h"


/*
 * Chybove hlaseni, napr. osetreni meze poli pro priklad a)
 */
void error_msg(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "CHYBA: ");
    vfprintf(stderr, fmt, ap);
    exit(1);
}

/*
 * Varovne hlaseni, hlavne pro priklad b)
 */
void warning_msg(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "CHYBA: ");
    vfprintf(stderr, fmt, ap);
}
