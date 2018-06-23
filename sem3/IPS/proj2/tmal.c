/*
* Author:      Vladimir Dusek, xdusek27
* Project:     IPS - proj2, thread memory allocator
* University:  VUT FIT 2BIT
* Date:        15/11/2017
* Note:        Muze se hodit vypnuti randomizace pameti:
*              $ sudo sysctl kernel.randomize_va_space=0
*/


#include "tmal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Globalni promenna, ukazatel na pool array */
struct blk_pool_t *blks_table; 


/**
 * Allocate sparse table of blocks for several threads.
 * @param  nthreads     number of threads/items in the table
 * @return              pointer to the first block pool, NULL = failed
 */
struct blk_pool_t *tal_alloc_blks_table(unsigned nthreads)
{
	/* Alokace pameti pro pool array, jeden pool pro jedno vlakno */
	blks_table = (struct blk_pool_t *) malloc(nthreads * sizeof(struct blk_pool_t));

	/* Vrati ukazatel na pool array */
	return blks_table;
}


/**
 * Allocates and initialize pool of blocks.
 * @param  tid      thread index.
 * @param  nblks    capacity in number of blocks in the pool.
 * @param  theap    heap capacity for a given thread.
 * @return          pointer to the first block in a pool.
 */
struct blk_t *tal_init_blks(unsigned tid, unsigned nblks, size_t theap)
{
	/* Inicializace poolu */
	blks_table[tid].blks = (struct blk_t *) malloc(nblks * sizeof(struct blk_t));
	blks_table[tid].nblks = nblks;
	blks_table[tid].heap_size = theap;

	/* Inicializace celeho block array */
	for (unsigned i = 0; i < nblks; i++)
		blk_ctor(&blks_table[tid].blks[i]);

	/* Prideleni pozadovaneho heapu prvnimu bloku */
	blks_table[tid].blks[0].ptr = malloc(theap);
	blks_table[tid].blks[0].size = theap;
	blks_table[tid].blks[0].used = false;

	/* Vrati ukazatel na block array pro dane vlakno */
	return blks_table[tid].blks;
}


/**
 * Block metadata constructor (alone, not used block).
 * @param blk pointer to block metadata.
 */
void blk_ctor(struct blk_t *blk)
{
	/* Inicializace bloku */
	blk->ptr = NULL;
	blk->size = 0;
	blk->prev_idx = -1;
	blk->next_idx = -1;
	blk->used = false;
}


/**
 * Allocate memory for a given thread. Note that allocated memory will be
 * aligned to sizeof(size_t) bytes.
 * @param  tid  thread index (in the blocks table)
 * @param  size requested allocated size
 * @return      pointer to allocated space, NULL = failed
 */
void *tal_alloc(unsigned tid, size_t size)
{
	unsigned i;

	/* Projde vsechny bloky poolu a prvni ktery bude vyhovovat (nebude vyuzivan a
	   bude dostatecne velky) se pouzije */
	for (i = 0; i < blks_table[tid].nblks; i++)
	{
		/* Vyhovuje */
		if (blks_table[tid].blks[i].size >= size && blks_table[tid].blks[i].used == false)
		{
			/* Zarovnani velikosti pozadovane pameti na nasobek 8 bytu */
			size = size % 8 != 0 ? size + 8 - size % 8 : size;

			/* Zbyde nejaka volna pamet, blok je potreba rozdelit */
			if (blks_table[tid].blks[i].size - size != 0)
				tal_blk_split(tid, i, size);

			/* Blok se zaplni presne cely, neni treba ho rozdelovat */
			else
				blks_table[tid].blks[i].used = true;

			/* Vratim ukazatel na uzivatelem alokovanou pamet */
			return blks_table[tid].blks[i].ptr;
		}
	}

	/* Nenasel se vyhovujici blok */
	return NULL;
}


/**
 * Free memory for a given thread.
 * @param tid   thread index
 * @param ptr   pointer to memory allocated by tal_alloc or tal_realloc.
 *              NULL = do nothing.
 */
void tal_free(unsigned tid, void *ptr)
{
	unsigned i;

	/* Najde blok ktery se ma uvolnit */
	for (i = 0; blks_table[tid].blks[i].ptr != ptr; i++)
		;

	int left_idx = blks_table[tid].blks[i].prev_idx;
	int curr_idx = (int) i;
	int right_idx = blks_table[tid].blks[i].next_idx;

	/* Nalevo i napravo je volny blok -> sloucim vsechny */
	if (left_idx != -1 && !(blks_table[tid].blks[left_idx].used) &&
		right_idx != -1 && !(blks_table[tid].blks[right_idx].used))
	{
		tal_blk_merge(tid, left_idx, curr_idx);
		tal_blk_merge(tid, left_idx, right_idx);
	}

	/* Pouze nalevo je volny blok -> sloucim soucasny a levy */
	else if (left_idx != -1 && !(blks_table[tid].blks[left_idx].used))
		tal_blk_merge(tid, left_idx, curr_idx);

	/* Pouze napravo je volny blok -> sloucim soucasny a pravy */
	else if (right_idx != -1 && !(blks_table[tid].blks[right_idx].used))
		tal_blk_merge(tid, curr_idx, right_idx);

	blks_table[tid].blks[curr_idx].used = false;
}


/**
 * Merge two blocks.
 * @param tid       thread index
 * @param left_idx  index of the left block
 * @param right_idx index of the right block
 */
void tal_blk_merge(unsigned tid, int left_idx, int right_idx)
{
	/* Ulozeni novych informaci o levem bloku (slouceny) */
	blks_table[tid].blks[left_idx].size += blks_table[tid].blks[right_idx].size;
	blks_table[tid].blks[left_idx].next_idx = blks_table[tid].blks[right_idx].next_idx;
	blks_table[tid].blks[blks_table[tid].blks[right_idx].next_idx].prev_idx = left_idx;

	/* Inicializace praveho bloku vychozimi hodnotami */
	blk_ctor(&(blks_table[tid].blks[right_idx]));
}


/**
 * Splits one block into two.
 * @param tid       thread index
 * @param blk_idx   index of the block to be split
 * @param req_size  requested size of the block
 * @return          index of a new block created as remainder.
 */
int tal_blk_split(unsigned tid, int blk_idx, size_t req_size)
{
	unsigned i;

	/* Projde vsechny bloky a najde prvni ktery je volny (ptr == NULL), 
	   ten pouzije na zapamatovani noveho volneho mista. */
	for (i = 0; i < blks_table[tid].nblks; i++)
	{
		/* Volny blok nalezen */
		if (blks_table[tid].blks[i].ptr == NULL)
		{
			/* Ulozeni informaci o nove volnem bloku */
			blks_table[tid].blks[i].ptr = blks_table[tid].blks[blk_idx].ptr + req_size;
			blks_table[tid].blks[i].size = blks_table[tid].blks[blk_idx].size - req_size;
			blks_table[tid].blks[i].used = false;
			blks_table[tid].blks[i].prev_idx = blk_idx;
			blks_table[tid].blks[i].next_idx = blks_table[tid].blks[blk_idx].next_idx;

			/* Nastaveni zpetneho pointeru za blokem ktery se rozdelu */
			blks_table[tid].blks[blks_table[tid].blks[blk_idx].next_idx].prev_idx = i;

			/* Ulozeni informaci o nove vyuzivanem bloku */
			blks_table[tid].blks[blk_idx].next_idx = i;
			blks_table[tid].blks[blk_idx].size = req_size;
			blks_table[tid].blks[blk_idx].used = true;
			break;
		}
	}

	return i;
}


/**
 * Realloc memory for a given thread.
 * @param tid   thread index
 * @param ptr   pointer to allocated memory, NULL = allocate a new memory.
 * @param size  a new requested size (may be smaller than already allocated),
 *              0 = equivalent to free the allocated memory.
 * @return      pointer to reallocated space, NULL = failed.
 */
void *tal_realloc(unsigned tid, void *ptr, size_t size)
{
	unsigned i, j;

	/* Nova pozadovana velikost je nula, zavolam free */
	if (size == 0)
	{
		tal_free(tid, ptr);
		return NULL;
	}

	/* Zarovnani velikosti pozadovane pameti na nasobek 8 bytu */
	size = size % 8 != 0 ? size + 8 - size % 8 : size;

	/* Najde blok ktery se ma realokovat */
	for (i = 0; blks_table[tid].blks[i].ptr != ptr; i++)
		;
	
	/* Blok se bude zmensovat */
	if (size < blks_table[tid].blks[i].size)
	{
		j = tal_blk_split(tid, i, size);
		tal_free(2, blks_table[tid].blks[j].ptr);
		return blks_table[tid].blks[i].ptr;
	}

	/* Blok se bude zvetsovat */
	else if (size > blks_table[tid].blks[i].size)
	{
		int beyond_idx = blks_table[tid].blks[i].next_idx;

		/* Blok za realokovanym je volny a dostatecne velky, staci je mergenout */
		if (!(blks_table[tid].blks[beyond_idx].used) &&
			size <= blks_table[tid].blks[i].size + 
			blks_table[tid].blks[beyond_idx].size)
		{
			/* Sloucim bloky */
			tal_blk_merge(tid, (int) i, beyond_idx);

			/* Kdyz sloucene bloky jsou moc velike, je potreba je rozdelit */
			size_t size_merge_blk = blks_table[tid].blks[i].size + blks_table[tid].blks[beyond_idx].size;
			if (size != size_merge_blk)
				tal_blk_split(tid, i, size);

			return blks_table[tid].blks[i].ptr;
		}
		/* Blok za realokovanym neni volny nebo dostatecne velky, bude potreba najit
		   jiny, dostatecne velky a data tam nakopirovat */
		else
		{
			/* Alokuje novou pamet */
			void *dst = tal_alloc(tid, size);

			/* Nenasel se dostatecne velky blok -> vrati se NULL */
			if (dst == NULL)
				return NULL;

			/* Presune do ni stary obsah */
			memcpy(dst, blks_table[tid].blks[i].ptr, size);

			/* Starou pamet uvolni */
			tal_free(tid, blks_table[tid].blks[i].ptr);

			/* Vrati novou pamet */
			return dst;
		}
	}

	/* Blok ma byt stejne velky, neni treba realokovat */
	else
		return ptr;

	return NULL;
}
