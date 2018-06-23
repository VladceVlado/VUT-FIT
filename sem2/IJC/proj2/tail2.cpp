/*
 * Soubor:      tail2.cpp
 * Projekt:     IJC-DU2, priklad 1) a)
 * Autor:       Vladimir Dusek, xdusek27
 * Skola:       VUT FIT 1BIT
 * Datum:       3/4/2017
 * Prekladac:   gcc 5.4.0
 * Popis:       Program tail tak jak je znam z posixu. Vypise 'n' poslednich radku
 *              ze souboru nebo standartniho vstupu.
 * Spusteni:    ./tail2 [-n x] [filename] 
 */


#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
#include <fstream>
#include <queue>



/*
 * Vrati hodnotu 'number' parametru argumentu "-n number"
 * V pripade neplatneho tvaru 'number' vraci -1
 */
long argN(char *p_n)
{
	char *p_error;
	long unsigned n = strtoul(p_n, &p_error, 10);

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
char *test_arg(int argc, char *argv[], unsigned long *n)
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


using namespace std;

/*
 * main
 */
int main(int argc, char *argv[])
{
	ios::sync_with_stdio(false);

	unsigned long n; // kolik se ma vypsat poslednich radku
	
	/* zpracovani argumentu, kdyz chyba samo ukonci */
	char *filename = test_arg(argc, argv, &n);

	if (n < 1)
		return 0;

	istream *in;
	ifstream fr;

	/* cte ze souboru */
	if (filename != NULL)
	{
		string name = filename;
		fr.open(name, ios::in);

    	if (!fr.is_open())
    	{
      		cerr << "Chyba! Soubor se nepodarilo otevrit." << endl;
      		return 1;
      	}

      	in = &fr;
    }
    /* cte ze stdin */
	else
		in = &cin;

	queue<string> fronta; // fronta
	string line; // buffer pro nacteni radku

	/* nacteni vstupu */
	while (getline(*in, line))
    {
    	fronta.push(line); // push do fronty

    	if (fronta.size() > n) // pop kdyz > n
    		fronta.pop();
    }

    unsigned long size = fronta.size();
    
    /* vypsani poslednich n radku */
    for (unsigned long i = 0; i < size; i++)
    {
    	cout << fronta.front() << endl;
    	fronta.pop();
    }

    /* kdyz se cetlo ze souboru tak ho zavre */
    if (fr.is_open())
    	fr.close();
  	
	return 0;
}
