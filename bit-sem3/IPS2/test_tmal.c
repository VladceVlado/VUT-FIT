/*
* Author:      Vladimir Dusek, xdusek27
* Project:     IPS - proj2, thread memory allocator
* University:  VUT FIT 2BIT
* Date:        15/11/2017
* Note:        Muze se hodit vypnuti randomizace pameti:
*              $ sudo sysctl kernel.randomize_va_space=0
*/


#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include "tmal.h"


const unsigned int MAX_BLOCKS = 10000;
const size_t THREAD_HEAP = 200L*1024*1024; // 200MB


void debug_blkinfo(unsigned tid, unsigned i)
{
    struct blk_t *blk = &blks_table[tid].blks[i];
    printf("blks_table[%u].blks[%u] (@%p) = {\n", tid, i, blk);
    printf("  .ptr = %p,\n", blk->ptr);
    printf("  .size = %lu,\n", blk->size);
    printf("  .prev_idx = %i,\n", blk->prev_idx);
    printf("  .next_idx = %i,\n", blk->next_idx);
    printf("  .used = %s }\n", blk->used ? "true" : "false");
}


void debug6(const char *msg)
{
    #ifndef NDEBUG
        printf("\n\n-- %s -----------\n", msg);
        for (int i = 0; i < 6; i++)
            debug_blkinfo(2, i);
    #endif
}


int main(void)
{
    assert(blks_table == NULL);
    // priprava pro 4 vlakna
    tal_alloc_blks_table(4);
    assert(blks_table != NULL);


    // kazdemu vlaknu priradit/alokovat jeho heap
    for (int tid = 0; tid < 4; tid++)
        tal_init_blks(tid, MAX_BLOCKS, THREAD_HEAP);
    /**
     *   +----+----+----+----+----+-------------+
     *   | .................................... |
     *   +----+----+----+----+----+-------------+
     */
    debug6("After init");


    for (int tid = 0; tid < 4; tid++)
    {
        assert(blks_table[tid].nblks == MAX_BLOCKS);
        assert(blks_table[tid].heap_size == THREAD_HEAP);
        struct blk_t *first = blks_table[tid].blks;
        assert(first != NULL);
        assert(first->size == THREAD_HEAP);
        assert(!first->used);
        assert(first->prev_idx < 0 && first->next_idx < 0);
    }


    // vlakno 2 by rado alokovalo par bajtu
    void *b1 = tal_alloc(2, sizeof(size_t));
    /**
     *   +----+----+----+----+----+-------------+
     *   | b1 | ............................... |
     *   +----+----+----+----+----+-------------+
     */
    debug6("After the first alloc");
 

    assert(blks_table[2].blks[0].size == sizeof(size_t));
    assert(blks_table[2].blks[0].ptr == b1);
    assert(blks_table[2].blks[0].prev_idx < 0);
    assert(blks_table[2].blks[0].next_idx == 1);
    assert(blks_table[2].blks[0].used);
    assert(blks_table[2].blks[1].size == THREAD_HEAP - sizeof(size_t));
    assert(blks_table[2].blks[1].prev_idx == 0);
    assert(blks_table[2].blks[1].next_idx < 0);
    assert(!blks_table[2].blks[1].used);
 

    // alokujeme vice polozek
    void *a[4];
    for (int i = 0; i < 4; i++)
        a[i] = tal_alloc(2, sizeof(size_t));
    /**
     *   +----+----+----+----+----+-------------+
     *   | b1 | a0 | a1 | a2 | a3 | ........... |
     *   +----+----+----+----+----+-------------+
     */
    debug6("After the next 4 allocs");


    tal_free(2, a[0]);
    /**
     *   +----+----+----+----+----+-------------+
     *   | b1 | .. | a1 | a2 | a3 | ........... |
     *   +----+----+----+----+----+-------------+
     */
    debug6("After a[0] free");


    tal_free(2, a[2]);
    /**
     *   +----+----+----+----+----+-------------+
     *   | b1 | .. | a1 | .. | a3 | ........... |
     *   +----+----+----+----+----+-------------+
     */
    debug6("After a[2] free");


    void *c1 = tal_alloc(2, 1); // alokuj pouze 1 bajt, ale zarovnej
    /**
     *   +----+----+----+----+----+-------------+
     *   | b1 | c1 | a1 | .. | a3 | ........... |
     *   +----+----+----+----+----+-------------+
     */
    debug6("After alloc in the middle");


    assert(blks_table[2].blks[0].next_idx == 1);
    assert(blks_table[2].blks[1].prev_idx == 0);
    assert(blks_table[2].blks[1].ptr == c1);
    assert(blks_table[2].blks[1].size == sizeof(size_t));


    tal_free(2, a[1]);
    /**
     *   +----+----+----+----+----+-------------+
     *   | b1 | c1 | ....... | a3 | ........... |
     *   +----+----+----+----+----+-------------+
     */
    debug6("After free in the middle");


    unsigned b1_idx = 0;
    unsigned c1_idx = blks_table[2].blks[b1_idx].next_idx;
    unsigned blank_idx = blks_table[2].blks[c1_idx].next_idx;
    unsigned a3_idx = blks_table[2].blks[blank_idx].next_idx;
    unsigned rest_idx = blks_table[2].blks[a3_idx].next_idx;
    assert(blks_table[2].blks[b1_idx].ptr == b1);
    assert(blks_table[2].blks[c1_idx].ptr == c1);
    assert(blks_table[2].blks[blank_idx].ptr == a[1]);
    assert(blks_table[2].blks[a3_idx].ptr == a[3]);
    assert(blks_table[2].blks[rest_idx].size ==
        THREAD_HEAP - 5*sizeof(size_t));


    tal_free(2, b1);
    /**
     *   +----+----+----+----+----+-------------+
     *   | .. | c1 | ....... | a3 | ........... |
     *   +----+----+----+----+----+-------------+
     */
    debug6("After free in the beginning");


    tal_free(2, c1);
    /**
     *   +----+----+----+----+----+-------------+
     *   | ................. | a3 | ........... |
     *   +----+----+----+----+----+-------------+
     */
    debug6("After free in between");


    unsigned first_idx = 0;
    unsigned second_idx = blks_table[2].blks[first_idx].next_idx;
    unsigned third_idx = blks_table[2].blks[second_idx].next_idx;
    assert(! blks_table[2].blks[first_idx].used);
    assert(blks_table[2].blks[second_idx].used);
    assert(! blks_table[2].blks[third_idx].used);
    assert(blks_table[2].blks[first_idx].size == 4*sizeof(size_t));
    assert(blks_table[2].blks[second_idx].size == sizeof(size_t));


    /**
     * Vlastni rozsireni testu o realloc
     */
    void *c2 = tal_alloc(2, 3 * sizeof(size_t));
    /**
     *   +----+----+----+----+----+-------------+
     *   |      c2      |....| a3 | ........... |
     *   +----+----+----+----+----+-------------+
     */
    debug6("After alloc c2 for 24B");


    c2 = tal_realloc(2, c2, 4 * sizeof(size_t));
    /**
     *   +----+----+----+----+----+-------------+
     *   |        c2         | a3 | ........... |
     *   +----+----+----+----+----+-------------+
     */
    debug6("After realloc c2 to 32B");


    c2 = tal_realloc(2, c2, 2 * sizeof(size_t));
    /**
     *   +----+----+----+----+----+-------------+
     *   |    c2   |.........| a3 | ........... |
     *   +----+----+----+----+----+-------------+
     */
    debug6("After realloc c2 to 16B");


    c2 = tal_realloc(2, c2, 5 * sizeof(size_t));
    /**
     *   +----+----+----+----+----+----+----+----+----+----+-------------+
     *   |...................| a3 |           c2           |.............|
     *   +----+----+----+----+----+----+----+----+----+----+-------------+
     */
    debug6("After realloc c2 to 40B");


    c2 = tal_realloc(2, c2, 1 * sizeof(size_t));
    /**
     *   +----+----+----+----+----+----+----+----+----+----+-------------+
     *   |...................| a3 | c2 |.................................|
     *   +----+----+----+----+----+----+----+----+----+----+-------------+
     */
    debug6("After realloc c2 to 8B");


    a[3] = tal_realloc(2, a[3], 2 * sizeof(size_t));
    /**
     *   +----+----+----+----+----+----+----+----+----+----+-------------+
     *   |    a3   |..............| c2 |.................................|
     *   +----+----+----+----+----+----+----+----+----+----+-------------+
     */
    debug6("After realloc a3 to 16B");


    a[3] = tal_realloc(2, a[3], 2 * sizeof(size_t));
    /**
     *   +----+----+----+----+----+----+----+----+----+----+-------------+
     *   |    a3   |..............| c2 |.................................|
     *   +----+----+----+----+----+----+----+----+----+----+-------------+
     */
    debug6("After realloc a3 to 16B -- nothing shoud change");


    a[3] = tal_realloc(2, a[3], 3 * sizeof(size_t));
    /**
     *   +----+----+----+----+----+----+----+----+----+----+-------------+
     *   |      a3      |.........| c2 |.................................|
     *   +----+----+----+----+----+----+----+----+----+----+-------------+
     */
    debug6("After realloc a3 to 24B");


    c2 = tal_realloc(2, c2, 2 * sizeof(size_t));
    /**
     *   +----+----+----+----+----+----+----+----+----+----+-------------+
     *   |      a3      |.........|    c2   |............................|
     *   +----+----+----+----+----+----+----+----+----+----+-------------+
     */
    debug6("After realloc c2 to 16B");


    a[3] = tal_realloc(2, a[3], 6 * sizeof(size_t));
    /**
     *   +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
     *   |........................|    c2   |              a3             |.........|
     *   +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
     */
    debug6("After realloc a3 to 41B (should be 48B long)");


    return 0;
}
