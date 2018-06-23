/*
 * Soubor:      io.h
 * Projekt:     IJC-DU2, priklad 2)
 * Autor:       Vladimir Dusek, xdusek27
 * Skola:       VUT FIT 1BIT
 * Datum:       3/4/2017
 * Prekladac:   gcc 5.4.0
 * Popis:       Hlavickovy soubor pro io.c.
 */


/* Podmineny preklad _IO_H */
#ifndef _IO_H
#define _IO_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>


extern bool print_war;


int get_word(char *s, int max, FILE *f);


/* Konec podmineneho prekladu _IO_H */
#endif