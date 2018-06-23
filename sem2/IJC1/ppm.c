/*
 * Soubor:      ppm.c
 * Projekt:     IJC-DU1, priklad b)
 * Autor:       Vladimir Dusek, xdusek27
 * Skola:       VUT FIT 1BIT
 * Datum:       25/3/2017
 * Prekladac:   gcc 5.4.0
 * Popis:       cteni a ukladani dat z *.ppm a jejich zapisovani
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ppm.h"
#include "error.h"


/*
 * Nacte date ze souboru "file_name" a ulozi je do dynamicky 
 * alokovane struktury "*picture"
 */
struct ppm *ppm_read(const char *file_name)
{
	FILE *fr = fopen(file_name, "r");

	/* Soubor se nepodari otevrit -> error a vrati NULL */
	if (fr == NULL)
	{
		error_msg("Soubor %s se nepodarilo otevrit\n", file_name);
		return NULL;
	}

	unsigned int x, y, datasize, color;

    /* Povinna hlavicka souboru - "P6"<ws>+<x><ws>+<y><ws>+"255"<ws><data><EOF> */
	if (fscanf(fr, "P6 %u %u %u ", &x, &y, &color) != 3)
	{
        warning_msg("Spatny format souboru %s\n", file_name);
        if (fclose(fr) == EOF)
            warning_msg("Soubor %s se nepodarilo zavrit\n", file_name);
		return NULL;
	}
    
    /* Pocet barev musi byt 255 */
    if (color != 255)
    {
        warning_msg("Neplatny pocet barev obrazku %s\n", file_name);
        if (fclose(fr) == EOF)
            warning_msg("Soubor %s se nepodarilo zavrit\n", file_name);
        return NULL;
    }

    /* Preskoceni libovolneho poctu bilych znaku */
    while(isspace(getc(fr)))
        ;

    /* Vraceni cteci hlavy na zacatek binarnich dat (-1 char) */
    if (fseek(fr, -sizeof(char), SEEK_CUR) != 0)
    {
        warning_msg("Spatny format souboru %s\n", file_name);
        if (fclose(fr) == EOF)
            warning_msg("Soubor %s se nepodarilo zavrit\n", file_name);
        return NULL;
    }

	datasize = 3 * x * y;

    /* Kontrola velikosti souboru, kvuli staticke deklaraci bit_array_t */
	if (datasize > LIMIT)
	{
        warning_msg("Soubor %s je prilis velky\n", file_name);
        if (fclose(fr) == EOF)
            warning_msg("Soubor %s se nepodarilo zavrit\n", file_name);
        return NULL;
    }

    /* Dynamicka alokace struktury pro data */
	struct ppm *picture = (struct ppm *) malloc(sizeof(struct ppm) + sizeof(char) * datasize);
	if (picture == NULL)
	{
        warning_msg("Nepodarilo se alokovat pamet\n");
        if (fclose(fr) == EOF)
            warning_msg("Soubor %s se nepodarilo zavrit\n", file_name);
		return NULL;
	}

	picture->xsize = x;
	picture->ysize = y;

    /* Nacteni dat z obrazku do struktury*/
    if (fread(picture->data, sizeof(char), datasize, fr) != datasize)
    {
        warning_msg("Chyba pri nacitani dat ze souboru\n");
        if (fclose(fr) == EOF)
            warning_msg("Soubor %s se nepodarilo zavrit\n", file_name);
        free(picture);
		return NULL;
    }

	/* Soubor se nepodari zavrit -> warning */
	if (fclose(fr) == EOF)
		warning_msg("Soubor %s se nepodarilo zavrit\n", file_name);

	return picture;
}


/*
 * Zapise data ze struktury "*p" do souboru "file_name"
 */
int ppm_write(struct ppm *p, const char *file_name)
{
	FILE *fw = fopen(file_name, "w");
    
    /* Soubor se nepodari otevrit -> error a vrati zaporne cislo */
    if (fw == NULL) 
    {
        error_msg("Soubor %s se nepodarilo otevrit\n", file_name);
		return -1;
    }
    
    /* Zapsani hlavicky souboru */
    if (fprintf(fw, "P6\n%u %u\n255\n", p->xsize, p->ysize) < 0) 
    {
        error_msg("Nepodarilo se zapsat hlavicku souboru %s\n", file_name);
        if (fclose(fw) == EOF)
    		warning_msg("Soubor %s se nepodarilo zavrit\n", file_name);
        return -1;
    }

    unsigned int datasize = 3 * p->xsize * p->ysize;

    /* Zapsani binarnich dat obrazku */
    if (fwrite(p->data, sizeof(char), datasize, fw) != datasize) 
    {
        error_msg("Nepodarilo se zapsat binarni data souboru %s\n", file_name);
        if (fclose(fw) == EOF)
    		warning_msg("Soubor %s se nepodarilo zavrit\n", file_name);
        return -1;
    }

    /* Soubor se nepodari zavrit -> warning */
    if (fclose(fw) == EOF)
        warning_msg("Soubor %s se nepodarilo zavrit\n", file_name);

    return 0;
}
