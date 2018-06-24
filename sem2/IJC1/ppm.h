/*
 * Soubor:      ppm.h
 * Projekt:     IJC-DU1, priklad b)
 * Autor:       Vladimir Dusek, xdusek27
 * Skola:       VUT FIT 1BIT
 * Datum:       25/3/2017
 * Prekladac:   gcc 5.4.0
 * Popis:       hlavickovy soubor pro ppm.c
 */


/* Podmineny preklad _PPM_H */
#ifndef _PPM_H
#define _PPM_H


/* 
 * Maximalni velikost obrazku, omezana kvuli staticke 
 * deklaraci bitoveho pole pro program "steg-decode"
 */
#define LIMIT (1000 * 1000 * 3)


/* Struktura pro data souboru "*.ppm" */
struct ppm {
		unsigned xsize;
    	unsigned ysize;
    	char data[];    // RGB bajty, celkem 3*xsize*ysize
	};


/*
 * Nacte date ze souboru "file_name" a ulozi je do dynamicky 
 * alokovane struktury "*picture"
 */
struct ppm *ppm_read(const char *filename);


/*
 * Zapise data ze struktury "*p" do souboru "file_name"
 */
int ppm_write(struct ppm *p, const char *filename);


/* Konec podmineneho prekladu _PPM_H */
#endif
