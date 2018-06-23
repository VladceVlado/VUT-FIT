/*
 * Author:      Vladimir Dusek, xdusek27
 * Project:     IAL2 - hash table, recursive/not recursive operation above binary search tree
 * University:  VUT FIT 2BIT
 * Date:        20/11/2017
 * Description: task c402
 */


/* c402.c: ********************************************************************}
{* Téma: Nerekurzivní implementace operací nad BVS 
**                                     Implementace: Petr Přikryl, prosinec 1994
**                                           Úpravy: Petr Přikryl, listopad 1997
**                                                     Petr Přikryl, květen 1998
**			  	                        Převod do jazyka C: Martin Tuček, srpen 2005
**                                         Úpravy: Bohuslav Křena, listopad 2009
**                                         Úpravy: Karel Masařík, říjen 2013
**                                         Úpravy: Radek Hranický, říjen 2014
**                                         Úpravy: Radek Hranický, listopad 2015
**                                         Úpravy: Radek Hranický, říjen 2016
**
** S využitím dynamického přidělování paměti, implementujte NEREKURZIVNĚ
** následující operace nad binárním vyhledávacím stromem (předpona BT znamená
** Binary Tree a je u identifikátorů uvedena kvůli možné kolizi s ostatními
** příklady):
**
**     BTInit .......... inicializace stromu
**     BTInsert ........ nerekurzivní vložení nového uzlu do stromu
**     BTPreorder ...... nerekurzivní průchod typu pre-order
**     BTInorder ....... nerekurzivní průchod typu in-order
**     BTPostorder ..... nerekurzivní průchod typu post-order
**     BTDisposeTree ... zruš všechny uzly stromu
**
** U všech funkcí, které využívají některý z průchodů stromem, implementujte
** pomocnou funkci pro nalezení nejlevějšího uzlu v podstromu.
**
** Přesné definice typů naleznete v souboru c402.h. Uzel stromu je typu tBTNode,
** ukazatel na něj je typu tBTNodePtr. Jeden uzel obsahuje položku int Cont,
** která současně slouží jako užitečný obsah i jako vyhledávací klíč 
** a ukazatele na levý a pravý podstrom (LPtr a RPtr).
**
** Příklad slouží zejména k procvičení nerekurzivních zápisů algoritmů
** nad stromy. Než začnete tento příklad řešit, prostudujte si důkladně
** principy převodu rekurzivních algoritmů na nerekurzivní. Programování
** je především inženýrská disciplína, kde opětné objevování Ameriky nemá
** místo. Pokud se Vám zdá, že by něco šlo zapsat optimálněji, promyslete
** si všechny detaily Vašeho řešení. Povšimněte si typického umístění akcí
** pro různé typy průchodů. Zamyslete se nad modifikací řešených algoritmů
** například pro výpočet počtu uzlů stromu, počtu listů stromu, výšky stromu
** nebo pro vytvoření zrcadlového obrazu stromu (pouze popřehazování ukazatelů
** bez vytváření nových uzlů a rušení starých).
**
** Při průchodech stromem použijte ke zpracování uzlu funkci BTWorkOut().
** Pro zjednodušení práce máte předem připraveny zásobníky pro hodnoty typu
** bool a tBTNodePtr. Pomocnou funkci BTWorkOut ani funkce pro práci
** s pomocnými zásobníky neupravujte 
** Pozor! Je třeba správně rozlišovat, kdy použít dereferenční operátor *
** (typicky při modifikaci) a kdy budeme pracovat pouze se samotným ukazatelem 
** (např. při vyhledávání). V tomto příkladu vám napoví prototypy funkcí.
** Pokud pracujeme s ukazatelem na ukazatel, použijeme dereferenci.
**/


#include "c402.h"
int solved;


/*   ---------
** Pomocná funkce, kterou budete volat při průchodech stromem pro zpracování
** uzlu určeného ukazatelem Ptr. Tuto funkci neupravujte.
**/
void BTWorkOut(tBTNodePtr Ptr)
{		
	if (Ptr == NULL) 
    	printf("Chyba: Funkce BTWorkOut byla volána s NULL argumentem!\n");
 	else 
    	printf("Výpis hodnoty daného uzlu> %d\n", Ptr->Cont);
}


/* -------------------------------------------------------------------------- */
/*
** Funkce pro zásobník hodnot typu tBTNodePtr. Tyto funkce neupravujte.
**/
void SInitP(tStackP *S)  
{
	/*   ------
	** Inicializace zásobníku.
	**/
	S->top = 0;  
}	

void SPushP(tStackP *S, tBTNodePtr ptr)
{ 
	/*   ------
	** Vloží hodnotu na vrchol zásobníku.
	**/

    /* Při implementaci v poli může dojít k přetečení zásobníku. */
 	if (S->top==MAXSTACK) 
    	printf("Chyba: Došlo k přetečení zásobníku s ukazateli!\n");
  	else
  	{  
		S->top++;  
		S->a[S->top]=ptr;
	}
}	

tBTNodePtr STopPopP(tStackP *S)
{
	/*         --------
	** Odstraní prvek z vrcholu zásobníku a současně vrátí jeho hodnotu.
	**/
    
    /* Operace nad prázdným zásobníkem způsobí chybu. */
	if (S->top == 0) 
	{
		printf("Chyba: Došlo k podtečení zásobníku s ukazateli!\n");
		return(NULL);	
	}	
	else
		return (S->a[S->top--]);
}

bool SEmptyP(tStackP *S)
{
	/*   -------
	** Je-li zásobník prázdný, vrátí hodnotu true.
	**/

	return(S->top==0);
}	


/* -------------------------------------------------------------------------- */
/*
** Funkce pro zásobník hotnot typu bool. Tyto funkce neupravujte.
*/
void SInitB(tStackB *S)
{
	/*   ------
	** Inicializace zásobníku.
	**/

	S->top = 0;  
}	

void SPushB(tStackB *S,bool val)
{
	/*   ------
	** Vloží hodnotu na vrchol zásobníku.
	**/
	
	/* Při implementaci v poli může dojít k přetečení zásobníku. */
	if (S->top == MAXSTACK) 
		printf("Chyba: Došlo k přetečení zásobníku pro boolean!\n");
	else
	{
		S->top++;  
		S->a[S->top] = val;
	}	
}

bool STopPopB(tStackB *S)
{
	/*   --------
	** Odstraní prvek z vrcholu zásobníku a současně vrátí jeho hodnotu.
	**/
    
    /* Operace nad prázdným zásobníkem způsobí chybu. */
	if (S->top == 0)
	{
		printf("Chyba: Došlo k podtečení zásobníku pro boolean!\n");
		return(NULL);	
	}	
	else  
		return(S->a[S->top--]);
}

bool SEmptyB(tStackB *S)
{
	/*   -------
	** Je-li zásobník prázdný, vrátí hodnotu true.
	**/
  	return(S->top == 0);
}


/* -------------------------------------------------------------------------- */
/*
** Následuje jádro domácí úlohy - funkce, které máte implementovat. 
*/


/*   ------
** Provede inicializaci binárního vyhledávacího stromu.
**
** Inicializaci smí programátor volat pouze před prvním použitím binárního
** stromu, protože neuvolňuje uzly neprázdného stromu (a ani to dělat nemůže,
** protože před inicializací jsou hodnoty nedefinované, tedy libovolné).
** Ke zrušení binárního stromu slouží procedura BTDisposeTree.
**	
** Všimněte si, že zde se poprvé v hlavičce objevuje typ ukazatel na ukazatel,	
** proto je třeba při práci s RootPtr použít dereferenční operátor *.
**/
void BTInit(tBTNodePtr *RootPtr)
{
	*RootPtr = NULL;
}


/*   --------
** Vloží do stromu nový uzel s hodnotou Content.
**
** Z pohledu vkládání chápejte vytvářený strom jako binární vyhledávací strom,
** kde uzly s hodnotou menší než má otec leží v levém podstromu a uzly větší
** leží vpravo. Pokud vkládaný uzel již existuje, neprovádí se nic (daná hodnota
** se ve stromu může vyskytnout nejvýše jednou). Pokud se vytváří nový uzel,
** vzniká vždy jako list stromu. Funkci implementujte nerekurzivně.
**/
void BTInsert(tBTNodePtr *RootPtr, int Content)
{
	/* pomocna promenna pro prochazi stromu */
	tBTNodePtr tmp = *RootPtr;
	/* pomocna promenna pro pamatovani pozice */
	tBTNodePtr position = NULL;

	/* projduti stromem az dolu, najduti spravneho misto pro Content */
	while (tmp != NULL)
	{
		position = tmp;
		/* Content je vetsi nez Content uzle -> jdu do prava */
		if (Content > tmp->Cont)
			tmp = tmp->RPtr;
		/* Content je mensi nez Content uzle -> jdu do prava */
		else if (Content < tmp->Cont)
			tmp = tmp->LPtr;
		/* Content se shoduje s Contentem uzle -> konec cyklu */
		else
			break;
	}

	/* strom byl prazdny, novy uzel je korenem */
	if (position == NULL)
	{
		/* vytvoreni noveho uzlu */
		*RootPtr = (tBTNodePtr) malloc(sizeof(struct tBTNode));
		if (*RootPtr == NULL)
			return;

		/* inicializace uzlu */
		(*RootPtr)->Cont = Content;
		(*RootPtr)->LPtr = NULL;
		(*RootPtr)->RPtr = NULL;
		
	}
	/* strom nebyl prazdny, uzel vlozime na spravne misto */
	else
	{
		/* novy uzel ma vetsi hodnotu nez jeho otec, bude vpravo */
		if (Content > position->Cont)
		{
			/* vytvoreni noveho uzlu */
			tmp = (tBTNodePtr) malloc(sizeof(struct tBTNode));
			if (tmp == NULL)
				return;

			/* inicializace uzlu */
			tmp->Cont = Content;
            tmp->RPtr = NULL;
            tmp->LPtr = NULL;

            /* nastaveni ukazatele */
            position->RPtr = tmp;
		}

		/* novy uzel ma mensi hodnotu nez jeho otec, bude vlevo */
		else if (Content < position->Cont)
		{
			/* vytvoreni noveho uzlu */
			tmp = (tBTNodePtr) malloc(sizeof(struct tBTNode));
			if (tmp == NULL)
				return;

			/* inicializace uzlu */
			tmp->Cont = Content;
            tmp->RPtr = NULL;
            tmp->LPtr = NULL;

            /* nastaveni ukazatele */
            position->LPtr = tmp;
		}

		/* novy uzel ma stejnou hodnotu, nedeje se nic */
		//else
		//	;
	}
}


/*                                  PREORDER                                  */

/*   -----------------
** Jde po levě větvi podstromu, dokud nenarazí na jeho nejlevější uzel.
**
** Při průchodu Preorder navštívené uzly zpracujeme voláním funkce BTWorkOut()
** a ukazatele na ně is uložíme do zásobníku.
**/
void Leftmost_Preorder(tBTNodePtr ptr, tStackP *Stack)
{
	/* neplatny zasobnik */
	if (Stack == NULL)
		return;

	/* napushuvani cele leve vetve na zasobnik */
	while (ptr != NULL)
	{
		SPushP(Stack, ptr);
		BTWorkOut(ptr);
		ptr = ptr->LPtr;
	}	
}


/*   ----------
** Průchod stromem typu preorder implementovaný nerekurzivně s využitím funkce
** Leftmost_Preorder a zásobníku ukazatelů. Zpracování jednoho uzlu stromu
** realizujte jako volání funkce BTWorkOut(). 
**/
void BTPreorder(tBTNodePtr RootPtr)
{
	/* neplatny ukazatel na uzel */
	if (RootPtr == NULL)
		return;

	/* inicializace zasobniku ukazatelu */
	tStackP stackPtrs;
	SInitP(&stackPtrs);

	/* projduti stromu a napushujeme na zasobnik celou levou vetev */
	Leftmost_Preorder(RootPtr, &stackPtrs);
	
	/* pomocna */
	tBTNodePtr tmp;

	while (SEmptyP(&stackPtrs) == FALSE)
	{
		/* z vrcholu zasobniku odebereme uzel */
		tmp = STopPopP(&stackPtrs);
		/* posuneme se doprava a opet napushujeme celou vetev na zasobnik */
		Leftmost_Preorder(tmp->RPtr, &stackPtrs);
	}
}


/*                                  INORDER                                   */ 

/*   ----------------
** Jde po levě větvi podstromu, dokud nenarazí na jeho nejlevější uzel.
**
** Při průchodu Inorder ukládáme ukazatele na všechny navštívené uzly do
** zásobníku. 
**/
void Leftmost_Inorder(tBTNodePtr ptr, tStackP *Stack)
{
	/* neplatny zasobnik */
	if (Stack == NULL)
		return;

	/* napushuvani cele leve vetve na zasobnik */
	while (ptr != NULL)
	{
		SPushP(Stack, ptr);
		ptr = ptr->LPtr;
	}	
}


/*   ---------
** Průchod stromem typu inorder implementovaný nerekurzivně s využitím funkce
** Leftmost_Inorder a zásobníku ukazatelů. Zpracování jednoho uzlu stromu
** realizujte jako volání funkce BTWorkOut(). 
**/
void BTInorder(tBTNodePtr RootPtr)
{
	/* neplatny ukazatel na uzel */
	if (RootPtr == NULL)
		return;

	/* inicializace zasobniku ukazatelu */
	tStackP stackPtrs;
	SInitP(&stackPtrs);

	/* projduti stromu a napushujeme na zasobnik celou levou vetev */
	Leftmost_Inorder(RootPtr, &stackPtrs);
	
	/* pomocna */
	tBTNodePtr tmp;

	while (SEmptyP(&stackPtrs) == FALSE)
	{
		/* z vrcholu zasobniku odebereme uzel */
		tmp = STopPopP(&stackPtrs);
		/* vypsani uzle */
		BTWorkOut(tmp);
		/* posuneme se doprava a opet napushujeme celou vetev na zasobnik */
		Leftmost_Inorder(tmp->RPtr, &stackPtrs);
	}

	/* Inorder je to same jako preorder, pouze je prehozeny vypis */
}


/*                                 POSTORDER                                  */ 

/*           --------
** Jde po levě větvi podstromu, dokud nenarazí na jeho nejlevější uzel.
**
** Při průchodu Postorder ukládáme ukazatele na navštívené uzly do zásobníku
** a současně do zásobníku bool hodnot ukládáme informaci, zda byl uzel
** navštíven poprvé a že se tedy ještě nemá zpracovávat. 
**/
void Leftmost_Postorder(tBTNodePtr ptr, tStackP *StackP, tStackB *StackB)
{
	/* neplatny zasobnik */
	if (StackP == NULL || StackB == NULL)
		return;

	/* napushuvani cele leve vetve na zasobnik */
	while (ptr != NULL)
	{
		SPushP(StackP, ptr);
		SPushB(StackB, TRUE);
		ptr = ptr->LPtr;
	}	
}


/*           -----------
** Průchod stromem typu postorder implementovaný nerekurzivně s využitím funkce
** Leftmost_Postorder, zásobníku ukazatelů a zásobníku hodnot typu bool.
** Zpracování jednoho uzlu stromu realizujte jako volání funkce BTWorkOut(). 
**/
void BTPostorder(tBTNodePtr RootPtr)
{
	/* neplatny ukazatel na uzel */
	if (RootPtr == NULL)
		return;

	/* inicializace zasobniku */
	tStackP stackPtrs;
	tStackB stackBools;
	SInitP(&stackPtrs);
	SInitB(&stackBools);

	/* napushovani na zasobnik cele leve vetve stromu */
	Leftmost_Postorder(RootPtr, &stackPtrs, &stackBools);

	int left = FALSE;
	tBTNodePtr tmp;

	/* dokud neni zasobnik prazdny */
	while (SEmptyP(&stackPtrs) == FALSE)
	{
		/* ulozim uzel z vrcholu zasobniku a popnu */
		tmp = STopPopP(&stackPtrs);
		SPushP(&stackPtrs, tmp);

		/* ulozim bool hodnotu z vrcholu zasobniku a popnu */
		left = STopPopB(&stackBools);

		/* je z leva, pujdu doprava */
		if (left == TRUE)
		{
			SPushB(&stackBools, FALSE);
			Leftmost_Postorder(tmp->RPtr, &stackPtrs, &stackBools);
		}
		/* je z prava, odstrani otce */
		else
		{
			STopPopP(&stackPtrs);
			BTWorkOut(tmp);
		}
	}
}


/*   -------------
** Zruší všechny uzly stromu a korektně uvolní jimi zabranou paměť.
**
** Funkci implementujte nerekurzivně s využitím zásobníku ukazatelů.
**/
void BTDisposeTree (tBTNodePtr *RootPtr)
{
	/* neplatny ukazatel na uzel */
	if (*RootPtr == NULL)
		return;

	/* inicializace zasobniku */
	tStackP stackPtrs;
	SInitP (&stackPtrs);

	while (*RootPtr != NULL)
	{
		/* neni pravy uzel */
		if ((*RootPtr)->RPtr != NULL)
			SPushP(&stackPtrs, (*RootPtr)->RPtr);

		/* neni levy uzel */
		if ((*RootPtr)->LPtr != NULL)
			SPushP(&stackPtrs, (*RootPtr)->LPtr);

		free(*RootPtr);

		/* zasobnik neni prazdny, ulozim hodnotu a pop */
		if (!SEmptyP(&stackPtrs))
			*RootPtr = STopPopP(&stackPtrs);
		else
			*RootPtr = NULL;
	}
}

/* konec c402.c */
