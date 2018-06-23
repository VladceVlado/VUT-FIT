//
// Created by zeusko on 13/10/17.

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "keywords.h"

#ifndef TABLES2_HASH_TABLE_H
#define TABLES2_HASH_TABLE_H


/**
 * Struct for parameter.
 */
typedef struct param {
    tKeyWord type;
    struct param *next;
} tParam;

/**
 *  Hash table item.
 */
typedef struct htab_item {
    /// symbol data
    char *name;   // also a key
    char *const_value;
    char *prefix;
    bool is_global;
    bool defined;
    bool used;
    tKeyWord type;
    bool is_function;
    unsigned params_num;
    struct param *parameters; // list of function parameters
    /// pointer to next record
    struct htab_item *next;
} htab_item_t;

/**
 *  Hash table structure.
 */
typedef struct {
    unsigned long arr_size;
    unsigned long n; // number of records

    struct htab_item *list_arr[];
} htab_t;

/*** FUNCTIONS ***/


/**
 * Adds constant to table.
 * @param table
 * @param string_const
 * @return same as htab_add
 */
htab_item_t *htab_add_constant(htab_t *table,char *string_const, tKeyWord type);

/**
 *
 * @param item Item, on which param will be added.
 * @param type Type of argument
 * @return tParam * on the parameter, NULL in case of error
 */
tParam *htab_item_add_param(struct htab_item *item, tKeyWord type);

/**
 * Deletes list of params.
 * @param head of the list
 */
void htab_delete_param_list(tParam *head);

/**
 * Basic hash function.
 * @param str
 * @param table_size
 * @return index
 */
unsigned long hash_djb2(char *str, long table_size);


/**
 * Constructor for hash table.
 * @param size
 * @return NULL in case of allocation error
 *         pointer at the table otherwise
 */
htab_t *htab_init(long unsigned size);

/**
 * Adds item to the table, to the end of list on according line.
 * @param t table
 * @param key name of the variable
 * @return pointer at the item
 *         NULL in case of error (mem allocation, some of the params are NULL)
 */
htab_item_t *htab_add(htab_t *t, char *key);

/**
 * Finds an item with key 'key'
 * @param t table
 * @param key
 * @return 'htab_item_t *' if exists
 *          NULL if not found
 */
htab_item_t *htab_find(htab_t *t, char *key);

/**
 *
 * @param t table
 * @param key name of the variable
 * @return
 */
int htab_remove(htab_t *t, char *key);

/**
 * Checks whether all functions are defined.
 * @param t input table
 * @return true : all functions are defined
 *         false : elsewhere
 */
bool htab_check_functions(htab_t *t);

/**
 * Frees all items of hash table.
 */
void htab_clear(htab_t *t);


/**
 * Destructor for hash table.
 */
void htab_destroy(htab_t *t);

/************************************* DEBUG FUNCTIONS *********************************************/

void print_hashtable(htab_t *t);
void print_params(tParam *head);
void print_param(tParam *param);



#endif //TABLES2_HASH_TABLE_H
