// VUT FIT
// IZP 2016/2017
// Projekt 2 - Iteracni vypocty
// Soubor: proj2.c
// Autor: Vladimir Dusek, xdusek27 (1BIT)
// Datum: 27. 11. 2016

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/*
 * --log X N // prirozeny logaritmus s argumentem x v n iteracich // tayloruv polynom
 */
double taylor_log(double x, unsigned int n)   
{
	double result = 0.0;   // pro ukladani mezivypoctu a zaroven pro finalni vysledek
	long double citatel;
	long double jmenovatel;

	if (x == INFINITY)   // logaritmus z nekonecna je nekonecno
	{
		result = INFINITY;
	}
	else if (x == 0)   // logaritmus z nuly je -nekonecno
	{	
		result = -INFINITY;
	}
	else if (x > 0 && x < 1)   // pro x z intervalu (0 , 1) se pouzije prvni vzorec
	{
		x = 1 - x;   // uprava x do pozadovane podoby
		citatel = 1;
		jmenovatel = 0;

		for(; n > 0; n--)   // vypocet probiha podle pravidelneho schematu rady a provede se prave n vypoctu
		{
			citatel = citatel * x;
			jmenovatel = jmenovatel + 1;
			result = result - citatel/jmenovatel;
		}
	}
	else if (x >= 1)   // pro x z intervalu <1 , inf) se pouzije druhy vzorec
	{
		double citatel0 = (x-1) / x;   // puvodni citatel kterym se nasobi kazdy dalsi citatel v cyklu
		citatel = 1;
		jmenovatel = 0;

		for(; n > 0; n--)   // vypocet probiha podle pravidelneho schematu rady a provede se prave n vypoctu
		{
			citatel = citatel * citatel0;
			jmenovatel = jmenovatel + 1;
			result = result + citatel/jmenovatel;
		}
	}
	else   // logaritmus ze zapornyho cisla nelze vypocitat (nan)
	{
		result = NAN;
	}

	return result;
}

/*
 * --log X N // prirozeny logaritmus s argumentem x v n iteracich // zretezeny zlomek
 */
double cfrac_log(double x, unsigned int n)
{
	double result = 0.0;   // pro ukladani mezivypoctu a zaroven pro finalni vysledek
	long double citatel;
	long double jmenovatel;

	if (x == INFINITY)   // logaritmus z nekonecna je nekonecno
	{
		result = INFINITY;
	}
	else if (x == 0)   // logaritmus z nuly je -nekonecno
	{	
		result = -INFINITY;
	}
	else if (x > 0)   // logaritmus z kladneho cisla podle zretezeneho zlomku
	{
		x = (x - 1) / (x + 1);   // uprava x do pozadovane podoby
		for(; n > 0; n--)   // vypocet probiha podle pravidelneho schematu zretezeneho zlomku (ZACINA SE OD SPODU)
		{
			jmenovatel = ((2 * n) + 1) - result;
			citatel = n * n * x * x;
			result = citatel / jmenovatel;
		}

		result = (2 * x) / ( 1 - result);   // posledni (nejvrchnejsi) iterace, nedodrzuje pravidelne schema, musi byt tedy provedena zvlast
	}
	else   // logaritmus ze zapornyho cisla nelze vypocitat (nan)
	{
		result = NAN;
	}

	return result;
}

/*
 * --pow X Y N // exponencialni funkce x^y v n iteracich // ln(y) pomoci taylor_log
 */
double taylor_pow(double x, double y, unsigned int n)   
{
	double result = 0.0;   // pro ukladani mezivypoctu a zaroven pro finalni vysledek
	long double citatel = 1.0;
	long double jmenovatel = 1.0;

	if (x < 0)
	{
		result = NAN;
	}
	else if (x == 1)
	{
		result = 1;
	}
	else if (isnan(y))
	{
		result = NAN;
	}
	else if (y == 0)
	{
		result = 1;
	}
	else if (isnan(x))
	{
		result = NAN;
	}
	else if (x == INFINITY)
	{
		if (y > 0)
			result = INFINITY;
		else
			result = 0;
	}
	else if (y == -INFINITY)
	{
		if (x > 1)
			result = 0;
		else
			result = INFINITY;
	}
	else if (y == INFINITY)
	{
		if (x > 1)
			result = INFINITY;
		else
			result = 0;
	}
	else if (x == 0 && y > 0)
	{
		result = 0;
	}
	else
	{	
		double citatel0 = y * taylor_log(x, n);   // puvodni citatel kterym se nasobi kazdy dalsi citatel v cyklu

		for(int i = 1; n > 0; i++, n--)   // vypocet probiha podle pravidelneho schematu rady a provede se prave n vypoctu
		{
			citatel = citatel * citatel0;
			jmenovatel = jmenovatel * i;
			result = result + (citatel / jmenovatel);
		}

		result = result + 1;   // na konci prictu jednicku (prvni clen rady)
	}

	return result;
}

/*
 * --pow X Y N // exponencialni funkce x^y v n iteracich // ln(y) pomoci cfrac_log
 */
double taylorcf_pow(double x, double y, unsigned int n)   
{
	double result = 0.0;   // pro ukladani mezivypoctu a zaroven pro finalni vysledek
	long double citatel = 1.0;
	long double jmenovatel = 1.0;

	if (x < 0)
	{
		result = NAN;
	}
	else if (x == 1)
	{
		result = 1;
	}
	else if (isnan(y))
	{
		result = NAN;
	}
	else if (y == 0)
	{
		result = 1;
	}
	else if (isnan(x))
	{
		result = NAN;
	}
	else if (x == INFINITY)
	{
		if (y > 0)
			result = INFINITY;
		else
			result = 0;
	}
	else if (y == -INFINITY)
	{
		if (x > 1)
			result = 0;
		else
			result = INFINITY;
	}
	else if (y == INFINITY)
	{
		if (x > 1)
			result = INFINITY;
		else
			result = 0;
	}
	else if (x == 0 && y > 0)
	{
		result = 0;
	}
	else
	{	
		double citatel0 = y * cfrac_log(x, n);   // puvodni citatel kterym se nasobi kazdy dalsi citatel v cyklu

		for(int i = 1; n > 0; i++, n--)   // vypocet probiha podle pravidelneho schematu rady a provede se prave n vypoctu
		{
			citatel = citatel * citatel0;
			jmenovatel = jmenovatel * i;
			result = result + (citatel / jmenovatel);
		}

		result = result + 1;   // na konci prictu jednicku (prvni clen rady)
	}

	return result;
}

/*
 * zpracovani argumentu N
 */
long argN(char *p_n, bool *error)   
{
	char *p_error;
	long n = strtoul(p_n, &p_error, 10);   // string to unsigned long // n = vysledna hodnota

	if (n > 0 && *p_error == '\0')   // kdyz je n zadane spravne a pretypovani probehlo uspesne
		return n;
	else   // jinak neplatny znak
		*error = true;   // nastavi error v mainu na true

	return 1;
}

/*
 * zpracovani argumentu X
 */
double argX(char *p_x, bool *error)   
{
	char *p_error;
	double x = strtod(p_x, &p_error);   // string to double // x = vysledna hodnota

	if (*p_error == '\0')   // kdyz je x zadane spravne a pretypovani probehlo uspesne
		return x;
	else   // jinak neplatny znak
		*error = true;   // nastavi error v mainu na true

	return 1;
}

/*
 * zpracovani argumentu Y
 */
double argY(char *p_y, bool *error)
{
	char *p_error;
	double y = strtod(p_y, &p_error);   // pretypovani (string to double)

	if (*p_error == '\0')   // kdyz pretypovani y probehlo uspesne
		return y;
	else   // jinak neplatny znak
		*error = true;   // error v mainu ze nastavi na true

	return 1;   // vrati hodnotu y
}

/*
 * main
 */
int main (int argc, char *argv[])
{
	double x, y;   // argument X, Y
	long n;   // argument N
	bool error = false;   // chybove hlaseni
	double result1;   // vysledek pro taylor_log, ...
	double result2;   // vysledek pro cfrac_log, ...

	// --log X N // prirozeny logaritmus z x v n iteracich
	// X je realne cislo vetsi nez nula-09
	// N je prirozene cislo (bez nuly)
 	if ((argc == 4) && (strcmp(argv[1], "--log") == 0))
 	{
 		x = argX(argv[2], &error);    // pretypovani argumentu x (string to double)
 		n = argN(argv[3], &error);    // pretypovani argumentu n (string to long)

		if (!error)
		{
			result1 = taylor_log(x, n);   // vypocet vysledku pomoci taylorova polynomu
			result2 = cfrac_log(x, n);   // vypocet vysledku pomoci zretezeneho zlomku

			printf("       log(%g) = %.12g\n", x, log(x));   // vypsani vysledku
			printf(" cfrac_log(%g) = %.12g\n", x, result2);
			printf("taylor_log(%g) = %.12g\n", x, result1);
		}
 	}

 	// --pow X Y N; x^y v n iteracich
 	// X je realne cislo vetsi nez nula a ruzne od jedne
 	// Y je realne cislo
 	// N je prirozene cislo (bez nuly)
	else if ((argc == 5) && (strcmp(argv[1], "--pow") == 0))
	{
		n = argN(argv[4], &error);
 		y = argY(argv[3], &error);
		x = argX(argv[2], &error);

		if (!error)
		{
			result1 = taylor_pow(x, y, n);
			result2 = taylorcf_pow(x, y, n);

			printf("         pow(%g,%g) = %.12g\n", x, y, pow(x, y));   // vypsani vysledku
			printf("  taylor_pow(%g,%g) = %.12g\n", x, y, result1);
			printf("taylorcf_pow(%g,%g) = %.12g\n", x, y, result2);
		}
	}

	// jakykoliv jiny argument
	else
	{
		error = true;
	}

	if (error)
	{
		fprintf(stderr, "Chyba! Neocekavane spusteni programu.\n");
		return 1;
	}
	else
	{
 		return 0;
	}
}
