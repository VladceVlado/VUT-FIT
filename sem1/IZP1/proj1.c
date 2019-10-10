// VUT FIT
// IZP 2016/2017
// Projekt 1 - Prace s textem
// Soubor: proj1.c
// Autor: Vladimir Dusek, xdusek27 (1BIT)
// Datum: 6. 11. 2016

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

/*
 * zjisti zda je znak cislice
 */
int cislice(int a)
{
    if (a > 47 && a < 58)
        return 1;   // je cislice, vraci 1
    else
        return 0;   // neni cislice, vraci 0
}

/*
 * vypocita cislo^exponent, funguje pouze pro exponent >= 0
 */
int mocnina(int cislo, int exponent)
{
    int vysledek = 1;

    if (exponent > 0)
    {
        for (int i = 0; i < exponent; i++)
            vysledek = vysledek * cislo;
        return vysledek;
    }
    else if (exponent == 0)
        return 1;

    else
        return -1;  // pro exponent < 0 vraci hodnotu -1
}

/*
 * textovy retezec (pole znaku) pretypuje na int
 */
int pretypovani(char pole[])
{
    int size = 0;  // deklaruji promenou "size" pro pocet znaku v retezci

    for (int i = 0; pole[i] != '\0'; i++)   // zjistim pocet znaku v retezci a ulozim do "size"
            size++;

    int poledigits[size];  // deklaruji nove pole pro cislice
    bool digit = true;  // bool pro overovani cislice/jiny znak

    for (int i = 0; i < size; i++)  // cyklus ktery zjistuje, zda jsou znaky v retezci cislice
    {
        if (cislice(pole[i]))  // funkce ktera vrati 1 v pripade ze znak je cislice
            poledigits[i] = pole[i] - 48;   // naplneni noveho pole cislicemi z puvodniho pole (jako int)
        else
            digit = false;  // v pripade ze najdu neciselny znak z cyklu vyskocim

        if (!digit)
            break;
    }
    int n = 0;  // promena pro vysledne cislo
    if (digit)  // za pripadu ze jsem nasel same cislicove znaky
    {
        for (int i = 0; i < size; i++)
            n = n + poledigits[i] * mocnina(10, size - 1 - i);   // slozeni vysledneho cisla z jednotlivych cislic a mocnin 10
        return n;   // v pripade uspesneho pretypovani vrati int
    }
    else
        return -1;   // vrati -1 jestlize byl nalezen necislicovy znak
}

/*
 * varianta programu bez argumentu, pripadne s volitelnymi argumenty -s N (skip) 
 * a/nebo -n M (number of chars)
 */
void bezarg(int n, int m)
{
    int c;   // zadany znak
    int i = 0;   // pocitadlo bajtu (16 bajtu na radek)
    int counter = -1;   // pocitadlo "serii" (16 bajtu), pred kazdou serii adresa vstupniho bajtu
    char string[16];   // pole do ktereho nacitam vstup
    bool bezm = false;   // aby program fungoval v pripade ze nebude zadan argument -n M (jako parametr bude poslana nula)
    if (m == 0)
        bezm = true;

    while ((c = getchar()) != EOF)   // dokud neni zadan EOF nacita znaky a vypisuje jejich hex hodnotu
    {
        counter++;   // promena pro pocitani adres bajtu
        if (n <= 0)   // volitelny argument skip // preskakuje prvnich n znaku
        {
            if (m > 0 || bezm)   // volitelny argument number-of-chars // vypise pouze m znaku
            {
                if (i == 0)   // pred prvnim bajtem na radku vypise jeho adresu
                {
                    for (int j = 0; j < 16; j++)   // vyplnuje pole mezerama
                        string[j] = ' ';
                    printf("%08x  ", counter);   // 16 bajtu na radku, counter znaci o kolikatej se jedna radek
                }
                string[i] = c;
                i++;
                printf("%02x ", c);   // vypise zadany znak v hex hodnote zarovnany na dve mista doplneny nulou z leva
                m--;

                if (i == 8)   // 8 cislic + "2x space" + 8 cislic
                    printf(" ");

                if (i == 16)   // 16 hex cislic na radek, pak odradkovat
                {
                    printf(" |");

                    for (int j = 0; j < 16; j++)
                    {
                        if (isprint(string[j]))   // tisknutelne znaky definova pomoci funkce isprint
                            printf("%c", string[j]);
                        else
                            printf(".");   // misto netisknutelnych znaku vypsat . (tecku)
                    }
                    printf("|\n");
                    i = 0;

                    if (c == 10)
                        putchar('\n');
                }
                else if (c == 10 || m == 0)   // vypsani posledni, neuplne serie
                {
                    for (int j = 0; j < (16 - i); j++)   // vyplni mezerama v pripade neuplne serie
                        printf("%3c", ' ');

                    if (i < 8)
                        printf(" ");
                    printf(" |");

                    for (int j = 0; j < 16; j++)   // mensi nez 16 kvuli vyplneni mezerama
                    {
                        if (isprint(string[j]))   // tisknutelne znaky definova pomoci funkce isprint
                            printf("%c", string[j]);   // vypsat tisknutelny znak
                        else
                            printf("."); // misto netisknutelnych znaku vypsat . (tecku)
                    }

                    i = 0;
                    printf("|\n");
                }
            }
        }
        else
            n--;
    }
}

/*
 * varianta programu s argumentem -x (hexa-print)
 */
void argx()
{
    int c;
    while ((c = getchar()) != EOF)   // dokud neni zadan EOF nacita znaky a vypisuje jejich hex hodnotu
    {
        printf("%02x", c);   // vypise zadany znak v hex hodnote zarovnany na dve mista doplneny nulou z leva
        //if (c == 10 || c == '\0')
        //  printf("\n");
    }
    printf("\n");
}

/*
 * varianta programu s argumentem -S N (strings)
 */
void argS(int n)
{
    int string[n];   // pole kam se jednotlive znaky nactene ze vstupu ukladaji
    int c;   // ukladani vstupu
    int i = 0;   // index pro pocitani vstupu
    bool vetsi_nez_n = false;

    while ((c = getchar()) != EOF)
    {
        if (isprint(c))   // kdyz je znak tisknutelny ulozi se do pole
        {
            string[i] = c;
            i++;
        }
        else   // kdyz neni znak tisknutelny
        {
            if (i >= n || vetsi_nez_n)   // overi se podminka zda je vetsi/roven N
            {
                for (int j = 0; j < i; j++)   // kdyz je, pole znaku se vypise
                    printf("%c", string[j]);
                printf("\n");
                vetsi_nez_n = false;
            }
            i = 0;   // index se nastavi na nulu
        }

        if (i >= n)   // kdyz je pocet zadanych znaku vetsi nez N, tak se vypisou
        {
            for (int j = 0; j < i; j++)
                printf("%c", string[j]);
            i = 0;
            vetsi_nez_n = true;   // nenapise se ale new line, program bude dal nacitat vstup
        }
    }
}

/*
 * varianta programu s argumentem -r (reverse)
 */
int argr()
{
    bool error = false;   // v momente kdy je zadan neplatny znak, vypise napovedu, platne znaky: 0-9, a-f, A-F, isblank, \n
    bool add = false;   // zadan platny znak, prida se do pole
    int c;   // promena pro ukladani vstupu
    int i = 0;   // index
    int hex[2];   // jednotlive hexa cislice
    int n = 0;   // vysledna hodnota

    while ((c = getchar()) != EOF)
    {
        if (c > 47 && c < 58)
        {
            c = c - 48;
            add = true;
        }
        else if (c > 64 && c < 71)
        {
            c = c - 55;
            add = true;
        }
        else if (c > 96 && c < 103)
        {
            c = c - 87;
            add = true;
        }
        else if (c == 10 || isblank(c))
        {
            add = false;
            if (hex[0] == 0 && hex[1] == 0)
                i = 0;
        }
        else
        {
            printf("\n\nZadan neplatny znak\n\n\n");
            error = true;
            add = false;
        }

        if (add)
        {
            hex[i] = c;
            i++;
        }

        if (error)
        {
            return 1;
        }

        if (i == 1 && c == 10)
        {
            printf("%c", hex[0]);
            hex[0] = 0;
            hex[1] = 0;
        }

        if (i > 1)
        {
            n = 16 * hex[0] + hex[1];
            printf("%c", n);
            i = 0;
            hex[0] = 0;
            hex[1] = 0;
        }
    }
    return 0;
}

/*
 * varianta programu s jakymkoliv jinym argumentem, popripade po zadani chybneho vstupu, 
 * vypise napovedu
 */
void arghelp()
{
    printf("Napoveda:\n\n"
             "IZP 2016/2017\n"
             "1. Projekt - Prace s textem\n"
             "Vladimir Dusek\n"
             "xdusek27\n"
             "\n"
             "Popis programu:\n"
             "Program formatuje bud binarni data do textove podoby, nebo textovou podobu dat prevadi do binarni podoby.\n"
             "V pripade prevodu binarnich dat na text, vystupni format obsahuje adresy vstupnich bajtu, hexadecimalni\n"
             "kodovani a textovou reprezentaci obsahu. V pripade prevodu textu do binarni podoby je na vstupu ocekavane\n"
             "hexadecimalni kodovani bajtu.\n"
             "\n"
             "Spusteni bez argumentu, s volitelnymi argumenty -s N a/nebo -n M:\n"
             "Vystupni format se sklada z posloupnosti radku, kde kazdy radek popisuje jednu serii 16 bajtu ze vstupniho\n"
             "souboru. Na zacatku radku je uvedena adresa/pozice prvniho bajtu v dane serii, jedna se o hexadecimalni cislo.\n"
             "Nasleduje vypis hexadecimalnich hodnot danych bajtu. Na konci radku je pak uvedena tisknutelna podoba daneho\n"
             "bajtu, v pripade, ze dany znak neni tisknutelny, vytiskne se znak tecka.\n"
             "Prepinac skip (-s N) definuje, na ktere adrese ma vypis zacinat, tedy kolik znaku ze vstupniho souboru se ma\n"
             "ignorovat. N je nezaporne cislo.\n"
             "Prepinac number-of-chars (-n M) definuje maximalni delku vstupnich bajtu ke zpracovani. M je kladne cislo.\n"
             "\n"
             "Spusteni s argumentem -x (hexa-print):\n"
             "Veskera vstupni data jsou prevedena do hexadecimalni podoby na jeden radek. Kazdemu vstupnimu bajtu odpovida\n"
             "dvouciferne hexadecimalni cislo z cislic 0-9a-f.\n"
             "\n"
             "Spusteni s argumentem -S N (strings):\n"
             "Program tiskne pouze takove posloupnosti v binarnim vstupu, ktere vypadaji jako textovy retezec. Kazdy\n"
             "retezec je vytisten na jeden radek. Necht retezec je nejdelsi posloupnost tisknutelnych a prazdnych znaku\n"
             "jejiz delka je vetsi nebo rovna N znaku. N je druhy argument programu a udava cele kladne cislo mensi nez 200.\n"
             "\n"
             "Spusteni s argumentem -r (reverse):\n"
             "Program ocekava na vstupu sekvenci hexadecimalnich cislic a tyto prevadi do binarniho formatu. Bile znaky\n"
             "na vstupu program ignoruje. Kazda dvojice vyjadruje hodnotu jednoho bajtu. V pripade, ze je pocet cislic\n"
             "lichy, posledni cislice vyjadruje hodnotu bajtu v rozsahu 0-15.\n\n");
}

/*
 * main, zjisteni o jakou variantu programu (argument) se jedna
 */
int main(int argc, char *argv[])
{
    bool help = false;

    if (argc == 1)   // ./proj1
    {
        int n = 0;
        int m = 0;
        bezarg(n, m);
    }

    else if (argc == 3 && argv[1][0] == '-' && argv[1][1] == 's' && argv[1][2] == '\0')   // ./proj1 -s N
    {
        int n = pretypovani(argv[2]);
        int m = 0;

        if (n >= 0)
            bezarg(n, m);
        else
            help = true;
    }

    else if (argc == 3 && argv[1][0] == '-' && argv[1][1] == 'n' && argv[1][2] == '\0')   // ./proj1 -n M
    {
        int n = 0;
        int m = pretypovani(argv[2]);

        if (m > 0)
            bezarg(n, m);
        else
            help = true;
    }

    else if (argc == 5 && argv[1][0] == '-' && argv[1][1] == 's' && argv[1][2] == '\0' &&
             argv[3][0] == '-' && argv[3][1] == 'n' && argv[3][2] == '\0')   // ./proj1 -s N -n M
    {
        int n = pretypovani(argv[2]);
        int m = pretypovani(argv[4]);

        if (m > 0 && n >= 0)
            bezarg(n, m);
        else
            help = true;
    }

    else if (argc == 5 && argv[1][0] == '-' && argv[1][1] == 'n' && argv[1][2] == '\0' &&
             argv[3][0] == '-' && argv[3][1] == 's' && argv[3][2] == '\0')   // ./proj1 -n M -s N
    {
        int n = pretypovani(argv[4]);
        int m = pretypovani(argv[2]);

        if (m > 0 && n >= 0)
            bezarg(n, m);
        else
            help = true;
    }

    else if (argc == 2 && argv[1][0] == '-' && argv[1][1] == 'x' && argv[1][2] == '\0') // ./proj1 -x
    {
        argx();
    }

    else if (argc == 3 && argv[1][0] == '-' && argv[1][1] == 'S' && argv[1][2] == '\0') // ./proj1 -S N
    {
        int n = pretypovani(argv[2]);

        if (n > 0 && n < 200)
        {
            argS(n);
        }
        else
            help = true;
    }

    else if (argc == 2 && argv[1][0] == '-' && argv[1][1] == 'r' && argv[1][2] == '\0') // ./proj1 -r
    {
        if (argr() == 1)
            help = true;
    }

    else // ./proj1 "cokoliv jineho"
        help = true;

     if (help)   // vypise kratky popis (napovedu)
     {
        arghelp();
    }

    return 0;
}
