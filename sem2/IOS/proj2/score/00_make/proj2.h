/*
 * Soubor:      proj2.h
 * Projekt:     Projekt 2 - Sprava procesu
 * Predmet:     Operacni systemy (IOS)
 * Autor:       Vladimir Dusek, xdusek27
 * Skola:       VUT FIT 1BIT
 * Datum:       18/4/2017
 * Prekladac:   gcc 5.4.0
 * Popis:       Hlavickovy soubor pro proj2.c
 */


/* Podmineny preklad _PROJ2_H */
#ifndef _PROJ2_H
#define _PROJ2_H

/* Standartni hlavickove soubory */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h> 
#include <unistd.h> 
#include <semaphore.h> 
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <sys/stat.h>

/* Makro pro jmeno vystupniho souboru */
#define NAME "proj2.out"

/* Struktura pro ulozeni argumentu programu */
typedef struct
{
	unsigned int A;
	unsigned int C;
	unsigned int AGT;
	unsigned int CGT;
	unsigned int AWT;
	unsigned int CWT;
} config_t;

/**************************************************/
/* Testovani zadani spravnosti argumentu programu */
/**************************************************/
config_t test_args(int argc, char *argv[]);

/*****************************************************************/
/* Vrati pseudonahodne cislo (unsigned int) z intervalu <0; max> */
/*****************************************************************/
unsigned int ran_num(unsigned int max);

/**********************************************************************/
/* Nastavi zdroje - sdilenou pamet, semafory a otevre vystupni soubor */
/**********************************************************************/
void set_resources(unsigned int n, unsigned int A);

/********************************************************************/
/* Uvolni zdroje - sdilenou pamet, semafory a zavre vystupni soubor */
/********************************************************************/
void clean_resources();

/*******************************/
/* Proces reprezentujici adult */
/*******************************/
void adult(config_t config);

/*******************************/
/* Proces reprezentujici child */
/*******************************/
void child(config_t config);


/* Konec podmineneho prekladu _PROJ2_H */
#endif