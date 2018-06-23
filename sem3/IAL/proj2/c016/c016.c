/*
 * Author:      Vladimir Dusek, xdusek27
 * Project:     IAL2 - hash table, recursive/not recursive operation above binary search tree
 * University:  VUT FIT 2BIT
 * Date:        20/11/2017
 * Description: task c016
 */


/* c016.c: **********************************************************}
{* Téma:  Tabulka s Rozptýlenými Položkami
**                      První implementace: Petr Přikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Úpravy: Karel Masařík, říjen 2014
**                              Radek Hranický, říjen 2014
**                              Radek Hranický, listopad 2015
**                              Radek Hranický, říjen 2016
**
** Vytvořete abstraktní datový typ
** TRP (Tabulka s Rozptýlenými Položkami = Hash table)
** s explicitně řetězenými synonymy. Tabulka je implementována polem
** lineárních seznamů synonym.
**
** Implementujte následující procedury a funkce.
**
**  HTInit ....... inicializuje tabulku před prvním použitím
**  HTInsert ..... vložení prvku
**  HTSearch ..... zjištění přítomnosti prvku v tabulce
**  HTDelete ..... zrušení prvku
**  HTRead ....... přečtení hodnoty prvku
**  HTClearAll ... zrušení obsahu celé tabulky (inicializace tabulky
**                 poté, co již byla použita)
**
** Definici typů naleznete v souboru c016.h.
**
** Tabulka je reprezentována datovou strukturou typu tHTable,
** která se skládá z ukazatelů na položky, jež obsahují složky
** klíče 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na další synonymum 'ptrnext'. Při implementaci funkcí
** uvažujte maximální rozměr pole HTSIZE.
**
** U všech procedur využívejte rozptylovou funkci hashCode.  Povšimněte si
** způsobu předávání parametrů a zamyslete se nad tím, zda je možné parametry
** předávat jiným způsobem (hodnotou/odkazem) a v případě, že jsou obě
** možnosti funkčně přípustné, jaké jsou výhody či nevýhody toho či onoho
** způsobu.
**
** V příkladech jsou použity položky, kde klíčem je řetězec, ke kterému
** je přidán obsah - reálné číslo.
*/


#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;


/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíč a přidělit
** mu index v rozmezí 0..HTSize-1.  V ideálním případě by mělo dojít
** k rovnoměrnému rozptýlení těchto klíčů po celé tabulce.  V rámci
** pokusů se můžete zamyslet nad kvalitou této funkce.  (Funkce nebyla
** volena s ohledem na maximální kvalitu výsledku). }
*/
int hashCode(tKey key)
{
	int retval = 1;
	int keylen = strlen(key);
	for (int i = 0; i < keylen; i++)
		retval += key[i];
	return (retval % HTSIZE);
}


/*
** Inicializace tabulky s explicitně zřetězenými synonymy.  Tato procedura
** se volá pouze před prvním použitím tabulky.
*/
void htInit(tHTable *ptrht)
{
	/* neplatny ukazatel -> konec */
	if (ptrht == NULL)
		return;

	/* kazdy ukazatel v poli inicializujeme na NULL */
	for (int i = 0; i < HTSIZE; i++)
		(*ptrht)[i] = NULL;
}


/* TRP s explicitně zřetězenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíče key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není, 
** vrací se hodnota NULL.
**
*/
tHTItem* htSearch(tHTable *ptrht, tKey key)
{
	/* neplatny ukazatel na tabulku nebo klic */
	if (ptrht == NULL || key == NULL)
		return NULL;

	/* ziskani indexu do pole seznamu */
	int index = hashCode(key);
	tHTItem *tmp;

    /* klasicky cyklus pro prohledani celeho seznamu */
    for (tmp = (*ptrht)[index]; tmp != NULL; tmp = tmp->ptrnext)
    {
        /* kdyz se klic shoduje */
        if (strcmp(key, tmp->key) == 0)
        	return tmp;
    }

    return NULL;
}


/* 
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vkládá do tabulky ptrht položku s klíčem key a s daty
** data.  Protože jde o vyhledávací tabulku, nemůže být prvek se stejným
** klíčem uložen v tabulce více než jedenkrát.  Pokud se vkládá prvek,
** jehož klíč se již v tabulce nachází, aktualizujte jeho datovou část.
**
** Využijte dříve vytvořenou funkci htSearch.  Při vkládání nového
** prvku do seznamu synonym použijte co nejefektivnější způsob,
** tedy proveďte.vložení prvku na začátek seznamu.
**/
void htInsert(tHTable *ptrht, tKey key, tData data)
{
	/* neplatny ukazatel na tabulku nebo klic */
	if (ptrht == NULL || key == NULL)
		return;

	/* zjistime jestli se polozka s klicem 'key' jiz v tabulce nenachazi */
	tHTItem *tmp = htSearch(ptrht, key);

	/* nachazi -> aktualizujeme data */
	if (tmp != NULL)
		tmp->data = data;

	/* nenchazi -> vytvorime novou polozku a napojime do seznamu */
	else
	{
		/* alokace pameti pro novou polozku tabulky */
		tmp = (tHTItem *) malloc(sizeof(tHTItem));
		
		/* kontrola jestli se alokace zdarila */
		if (tmp == NULL)
			return;

		/* index do pole seznamu */
		int index = hashCode(key);

		/* alokace pro klic polozky */
		tmp->key = (char *) calloc(strlen(key) + 1, sizeof(char));			

		/* kontrola jestli se alokace zdarila */
		if (tmp->key == NULL)
		{
			free(tmp);
			return;
		}

		/* ulozime klic a data */
		strcpy(tmp->key, key);
		tmp->data = data;

		/* navazeme seznam, nova polozka pujde na zacatek */
		tmp->ptrnext = (*ptrht)[index];
		(*ptrht)[index] = tmp;
	}
}


/*
** TRP s explicitně zřetězenými synonymy.
** Tato funkce zjišťuje hodnotu datové části položky zadané klíčem.
** Pokud je položka nalezena, vrací funkce ukazatel na položku
** Pokud položka nalezena nebyla, vrací se funkční hodnota NULL
**
** Využijte dříve vytvořenou funkci HTSearch.
*/
tData* htRead(tHTable *ptrht, tKey key)
{
	/* neplatny ukazatel na tabulku nebo klic */
	if (ptrht == NULL || key == NULL)
		return NULL;

	/* zjistime jestli se polozka s klicem 'key' v tabulce nenachazi */
	tHTItem *tmp = htSearch(ptrht, key);

	/* polozka nalezena, vratime jeji datovou cast */
	if (tmp != NULL)
		return &(tmp->data);

	/* polozka nenalezena, vratime NULL */
	else
		return NULL;
}


/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vyjme položku s klíčem key z tabulky
** ptrht.  Uvolněnou položku korektně zrušte.  Pokud položka s uvedeným
** klíčem neexistuje, dělejte, jako kdyby se nic nestalo (tj. nedělejte
** nic).
**
** V tomto případě NEVYUŽÍVEJTE dříve vytvořenou funkci HTSearch.
*/
void htDelete(tHTable *ptrht, tKey key)
{
	/* neplatny ukazatel na tabulku nebo klic */
	if (ptrht == NULL || key == NULL)
		return;

	/* index do pole seznamu */
	int index = hashCode(key);
	/* docasna polozka pro prochazeni seznamu, inicializujeme na zacatek */
	tHTItem *tmp = (*ptrht)[index];
	/* pro zapamatovani predchozi polozky a navazani seznamu */
	tHTItem *tmp_old = NULL;

	/* projduti celym seznamem */
	while (tmp != NULL) 
	{
		/* nasel jsem hledanou polozku, zrusim ji a navazu seznam */
		if (strcmp(tmp->key, key ) == 0)
		{
			if (tmp_old == NULL)
            	(*ptrht)[index] = tmp->ptrnext;
            else
                tmp_old->ptrnext = tmp->ptrnext;

            free(tmp->key);
            free(tmp);
			tmp = NULL;
		}
		/* nenasel jsem, pokracuji v seznamu */
		else
		{
			tmp_old = tmp;
            tmp = tmp->ptrnext;
		}
	}
}


/* TRP s explicitně zřetězenými synonymy.
** Tato procedura zruší všechny položky tabulky, korektně uvolní prostor,
** který tyto položky zabíraly, a uvede tabulku do počátečního stavu.
*/
void htClearAll(tHTable *ptrht)
{
	/* neplatny ukazatel na tabulku */
	if (ptrht == NULL)
		return;

	/* pomocne ukazatele na polozky tabulky */
	tHTItem *tmp;
	tHTItem *tmp_old;

	/* prolezeme cele pole seznamu */
	for (int i = 0; i < HTSIZE; i++)
	{
		tmp = (*ptrht)[i];
		/* prolezeme cely seznam a kazdou polozku zrusime */	
		while (tmp != NULL)
		{
			tmp_old = tmp->ptrnext;
			free(tmp->key);
			free(tmp);
			tmp = tmp_old;
		}
		/* ukazatel v poli neukazuje nikam */
		(*ptrht)[i] = NULL;
	}
}
