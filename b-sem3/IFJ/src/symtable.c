//
// Created by zeusko on 13/10/17.
//
#include "symtable.h"
#include "error_ifj.h"


htab_item_t *htab_add_constant(htab_t* table, char *string_const, tKeyWord type)
{
    static unsigned const_num = 0;

    char *value = (char *) calloc(strlen(string_const) + 1, sizeof(char));
    if (value == NULL)
        errorExit(E_SYS, "mem alloc\n");

    strcpy(value, string_const);

    char *name = (char *) calloc(20, sizeof(char));
    if (name == NULL)
        errorExit(E_SYS, "mem alloc\n");

    snprintf(name, 20, "0%u_const", const_num++);

    htab_item_t *ret = htab_add(table, name);
    if (ret != NULL) {

        ret->const_value = value;
        ret->type = type;
    }
    else
    {
        free (value);
    }

    free (name);

    return ret;

}

tParam *htab_item_add_param(struct htab_item *item, tKeyWord type)
{
    tParam *new_item = (tParam *) calloc(1, sizeof(tParam));
    if (new_item == NULL)
        return NULL;

    new_item->type = type;
    new_item->next = NULL;

    if (item->parameters == NULL) {
        item->parameters = new_item;
    }
    else {
        tParam *tmp = item->parameters;
		while (tmp->next != NULL) {
			tmp = tmp->next;
		}
		tmp->next = new_item;
    }

	item->params_num++;
    return new_item;
}

void htab_delete_param_list(tParam *head)
{
    if (head == NULL)
        return;

    tParam *tmp;
    while (head)
    {
        tmp = head;
        head = head->next;
        free (tmp);
    }
}

unsigned long hash_djb2(char *str, long table_size)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
    hash = ((hash << 5) + hash) + c;

    return hash % table_size;
}

htab_t *htab_init(long unsigned size)
{
    htab_t *table = (htab_t *) malloc(sizeof(htab_t) + size * sizeof(struct htab_listitem *));
    if (table == NULL)
        return NULL;

    // initializing values
    table->arr_size = size;
    table->n = 0;
    // set pointers to NULL
    memset(table->list_arr, 0, size * sizeof(struct htab_listitem *));

    return table;
}

htab_item_t *htab_add(htab_t *t, char *key)
{
    if (t == NULL || key == NULL)
        return NULL;

    unsigned long index = hash_djb2(key, t->arr_size);
    htab_item_t *tmp, *new_item, *last_tmp;
    tmp = t->list_arr[index]; // tmp = head
    last_tmp = tmp;

    bool first_item = false;
    if (tmp == NULL)
        first_item = true;

    while (tmp != NULL) {
        /// item already existed, return NULL
        if (strcmp(key, tmp->name) == 0)
            return NULL;

        last_tmp = tmp;
        tmp = tmp->next;
    }
    /// create new item
    // calloc is used instead of malloc to initialize the values
    new_item = (htab_item_t *) calloc(sizeof(htab_item_t), 1);
    if (new_item == NULL)
        return NULL;

    new_item->name = (char *) malloc(sizeof(char) * (strlen(key) + 1));
    if (new_item->name == NULL){
        free(new_item);
        return NULL;
    }

    strcpy(new_item->name, key);
    new_item->next = NULL;
	new_item->const_value = NULL;

    if (first_item) // if head -> NULL
        t->list_arr[index] = new_item;
    else
        last_tmp->next = new_item;


    return new_item;
}

htab_item_t *htab_find(htab_t *t, char *key)
{
    if (t == NULL || key == NULL)
        return NULL;

    unsigned long index = hash_djb2(key, t->arr_size);
    htab_item_t *tmp;
    tmp = t->list_arr[index]; // tmp = head

    while (tmp != NULL) {
        /// item already existed, return pointer
        if (strcmp(key, tmp->name) == 0)
            return tmp;

        tmp = tmp->next;
    }

    // not found
    return NULL;
}

bool htab_check_functions(htab_t *t)
{
    if (t == NULL)
        return false;

    for (unsigned int i = 0; i < t->arr_size; i++) {
        htab_item_t *tmp = t->list_arr[i];

        while (tmp != NULL) {
            if (tmp->is_function && tmp->defined == false)
                return false;

            tmp = tmp->next;
        }
    }

    return true;
}

int htab_remove(htab_t *t, char *key)
{
    if (t == NULL || key == NULL)
        return -1;

    unsigned long index = hash_djb2(key, t->arr_size);

    htab_item_t *tmp = t->list_arr[index];
    htab_item_t *last_tmp = NULL;

    while (tmp != NULL) {
        if (strcmp(key, tmp->name) == 0) {
            break;
        }
        else {
            last_tmp = tmp;
            tmp = tmp->next;
        }
    }
    if (tmp == NULL) {
        return 1;
    }
    else {
        if (last_tmp == NULL) { // first in row
            t->list_arr[index] = tmp->next;
            htab_delete_param_list(tmp->parameters);
            if (tmp->const_value != NULL)
                free(tmp->const_value);

            if (tmp->prefix != NULL)
                free(tmp->prefix);
            free (tmp->name);
            free (tmp);
        }
        else {
            last_tmp->next = tmp->next;
            htab_delete_param_list(tmp->parameters);
            if (tmp->const_value != NULL)
                free(tmp->const_value);

            if (tmp->prefix != NULL)
                free(tmp->prefix);
            free (tmp->name);
            free (tmp);
        }
    }
    return 0;
}

void htab_clear(htab_t *t)
{
    if (!t)
        return;

    htab_item_t *tmp = NULL, *to_free = NULL;

    for(unsigned i = 0; i < t->arr_size; i++) {
        if (t->list_arr[i] == NULL)
            continue;

        tmp = t->list_arr[i]; // tmp points at first item
        while(tmp != NULL) {
            to_free = tmp;
            tmp = tmp->next;
            htab_delete_param_list(to_free->parameters);
            if (to_free->name != NULL)
                free(to_free->name);
            if (to_free->prefix != NULL)
                free(to_free->prefix);
            if (to_free->const_value != NULL)
            {
                free(to_free->const_value);
            }

            free(to_free);
        }
        t->list_arr[i] = NULL;
    }
    t->n = 0;
}

void htab_destroy(htab_t *t)
{
    htab_clear(t);
    free(t);
}

/*******************************/
void print_hashtable(htab_t *t)
{
    for (unsigned int i = 0; i < t->arr_size; i++)
    {
        htab_item_t *tmp = t->list_arr[i];
        if (tmp != NULL)
        {
            printf("START %s -->", tmp->name);
            tmp = tmp->next;
        }
        else
        {
            printf("START NULL\n");
            continue;
        }

        while (tmp != NULL) {
            printf(" %s -->", tmp->name);
            tmp = tmp->next;
        }
        printf(" NULL \n");
    }
}

void print_params(tParam *head)
{
    printf("    START --> ");
    tParam *tmp;
    while (head)
    {
        tmp = head;
        head = head->next;

        print_param(tmp);
        printf(" --> ");
    }
    printf("\n");
}

void print_param(tParam *param)
{
    if (param == NULL)
        printf ("NULL");
    else
        printf("%p (%d) ", (void *)param, param->type);
}
