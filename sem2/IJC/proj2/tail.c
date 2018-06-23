/*
 * Soubor:      tail.c
 * Projekt:     IJC-DU2, priklad 1) a)
 * Autor:       Vladimir Dusek, xdusek27
 * Skola:       VUT FIT 1BIT
 * Datum:       3/4/2017
 * Prekladac:   gcc 5.4.0
 * Popis:       Program tail tak jak je znam z posixu. Vypise 'n' poslednich radku
 *              ze souboru nebo standartniho vstupu.
 * Spusteni:    ./tail [-n x] [filename] 
 */


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>


/* Maximalni pocet znaku na radek, vcetne '\0' a '\n' */
#define MAX_CHARS 1024


/*
 * Vrati hodnotu 'number' parametru argumentu "-n number"
 * V pripade neplatneho tvaru 'number' vraci -1
 */
long argN(char *p_n)
{
	char *p_error;
	long n = strtoul(p_n, &p_error, 10);

	if (!(n >= 0 && *p_error == '\0'))
		return -1;

	return n;
}


/*
 * Testuje zadani argumentu
 * Vrati ukazatel na "filename" zadaneho jako argument
 * Kdyz "filename" neni zadan vraci NULL
 * V pripade chyby vypise chybove hlaseni a ukonci program
 */
char *test_arg(int argc, char *argv[], long *n)
{
	if (argc < 1 || argc > 4)
	{
		fprintf(stderr, "Chyba! Nespravne spusteni programu, chybny pocet argumentu.\n");
		exit(1);
	}

	switch (argc)
	{
		case 1:
			*n = 10;
		break;

		case 2:
			if (strcmp(argv[1], "-n") == 0)
			{
				fprintf(stderr, "Chyba! Nespravne spusteni programu, -n 'num' mimo povolenou mez.\n");
				exit(3);
			}
			*n = 10;
			return argv[1];
		break;

		case 3:
			if (strcmp(argv[1], "-n") != 0)
			{
				fprintf(stderr, "Chyba! Nespravne spusteni programu, chybne zadan argument -n.\n");
				exit(2);
			}
			if ((*n = argN(argv[2])) < 0)
			{
				fprintf(stderr, "Chyba! Nespravne spusteni programu, -n 'num' mimo povolenou mez.\n");
				exit(3);
			}
		break;

		case 4:
			if (strcmp(argv[1], "-n") != 0)
			{
				fprintf(stderr, "Chyba! Nespravne spusteni programu, chybne zadan argument -n.\n");
				exit(2);
			}
			if ((*n = argN(argv[2])) < 0)
			{
				fprintf(stderr, "Chyba! Nespravne spusteni programu, -n 'num' mimo povolenou mez.\n");
				exit(3);
			}
			return argv[3];
		break;
	}

	return NULL;
}


/*
 * Otevre soubor "filename" pro cteni
 * Vrati ukazatel na zacatek souboru, typu 'FILE *'
 * Kdyz se "filename" nepovede otevrit, vypise chybove hlaseni a ukonci program
 */
FILE *open_file(char *filename)
{
	FILE *fr;

	if ((fr = fopen(filename, "r")) == NULL) 
	{
		fprintf(stderr, "Chyba! Vstupni soubor %s se nepodarilo otevrit.\n", filename);
		exit(4);
	}

	return fr;
}


/*
 * Spocita radky v souboru a vrati jejich pocet
 * Pokud pocet znaku na radku je vetsi nez "MAX_CHARS"
 * vypise chybove hlaseni a ukonci program
*/
long lines_count(FILE *fr)
{	
	if (fseek(fr, 0, SEEK_SET) != 0)
	{
		fprintf(stderr, "Chyba! Neporadilo se presunout na zacatek souboru.\n");
		exit(7);
	}

	long line_counter = 0;
	int c;

	while ((c = getc(fr)) != EOF)
	{
		if (c == '\n')
			line_counter++;
	}

	return line_counter;
}


/*
 * Vytiskne "n" poslednich radku ze souboru "fr", ktery ma celkem
 * "file_lines" radku
*/
void print_n_lines(FILE *fr, long file_lines, long n)
{
	if (fseek(fr, 0, SEEK_SET) != 0)
	{
		fprintf(stderr, "Chyba! Neporadilo se presunout na zacatek souboru.\n");
		exit(7);
	}

	int c;
	int chars_on_line = 0;
	bool print_error = false;

	while ((c = getc(fr)) != EOF)
	{
		chars_on_line++;

		if (file_lines <= n)
		{
			if (chars_on_line > MAX_CHARS)
			{
				print_error = true;
				
				if (c == '\n')
				{
					putchar('\n');
					file_lines--;
					chars_on_line = 0;
				}
				continue;
			}
			putchar(c);
		}

		if (c == '\n')
		{
			file_lines--;
			chars_on_line = 0;
		}
	}

	if (print_error)
		fprintf(stderr, "Varovani! Vyskytl se radek delsi nez %d znaku a byl zkracen.\n", MAX_CHARS);
}


/*
 * Zavre soubor "fp" jmena "filename" 
 * Kdyz se nepovede zavrit, vypise chybove hlaseni a ukonci program
 */
void close_file(FILE *fp, char *filename)
{
	if (fclose(fp) == EOF)
	{
		fprintf(stderr, "Chyba! Vstupni soubor %s se nepodarilo zavrit.\n", filename);
		exit(5);
	}
}

/*
 * Cte vstup ze stdin do EOF
 * Vytiskne poslednich n radku na stdout
*/
void read_stdin(long n)
{
	int c, i, j, k, help;

	bool print_error = false;

	char **p_s = (char **) malloc(n * sizeof(char *));
	
	if (p_s == NULL)
	{
		fprintf(stderr, "Chyba! Nepodarilo se alokovat potrebnou pamet.\n");
		exit(8);
	}

	for (i = 0; i < n; i++)
	{
		p_s[i] = (char *) calloc(MAX_CHARS + 1, sizeof(char));
		
		if (p_s[i] == NULL)
		{
			fprintf(stderr, "Chyba! Nepodarilo se alokovat potrebnou pamet.\n");
			
			for (i = 0; i < n; i++)
				free(p_s[i]);
			free(p_s);
			
			exit(8);
		}
	}
	
	for (i = 0, j = 0; (c = getchar()) != EOF; )
	{
		if (j < MAX_CHARS)
			p_s[i][j] = (char) c;
		else
		{
			print_error = true;

			if (c != '\n')
				continue;
			else
				p_s[i][j] = '\n';
		}

		if (c == '\n')
		{
			p_s[i][j+1] = '\0';
			i++;
			j = 0;
		}
		else
			j++;

		help = c;
		if ((c = getchar()) == EOF)
		{
			if (help != '\n')
				p_s[i][j] = '\0';
		 	break;
		}
		else
		{
			ungetc(c, stdin);

			if (i > n - 1)
			{
				for (k = 0; k < n - 1; k++)
					strcpy(p_s[k], p_s[k+1]);
				i = n - 1;
			}
		}
	}

	for (i = 0; i < n; i++)
		printf("%s", p_s[i]);

	if (print_error)
		fprintf(stderr, "Varovani! Vyskytl se radek delsi nez %d znaku a byl zkracen.\n", MAX_CHARS);

	for (i = 0; i < n; i++)
		free(p_s[i]);
	free(p_s);
}


/*
 * main
 */
int main(int argc, char *argv[])
{
	long n; // kolik se ma vypsat poslednich radku
	char *filename = test_arg(argc, argv, &n);

	if (n < 1)
		return 0;

	if (filename != NULL)
	{
		FILE *fr = open_file(filename);

		long file_lines = lines_count(fr);

		print_n_lines(fr, file_lines, n);

		close_file(fr, filename);
	}
	else
	{
		read_stdin(n);
	}

	return 0;
}
