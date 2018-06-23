
#include "ilist.h"
#include <errno.h>
#include "error_ifj.h"
#include <stdlib.h>

tIList *iListInit()
{
    tIList *toRet = calloc(1, sizeof(tIList));
    if (toRet == NULL)
        errno = E_SYS;
    return toRet;
}

void iListDestroy(tIList *list)
{
 	tIListItem *first = list->head;
    tIListItem *tmp = NULL;

    while (first != NULL){
        tmp = first;
        first = first->next;
        free(tmp);
    }
	
    free(list);
}

int iListAppend(tIList *list, tInstruction instr)
{
    tIListItem *toAdd = (tIListItem *) malloc(sizeof(tIListItem));
    if (toAdd == NULL) {
        errno = E_SYS;
        return -1;
    }

    toAdd->instr = instr;
    toAdd->next = NULL;

    // if list not empty
    if (list->tail != NULL) {
        list->tail->next = toAdd;
        list->tail = toAdd;
    }
    else {
        list->tail = list->head = toAdd;
    }

    return 0;
}


int iListCreateAppend(tIList *list, tOperator oper, htab_item_t *op1, htab_item_t *op2, htab_item_t *op3)
{
    tInstruction instr;

    instr.oper = oper;
    instr.op1 = op1;
    instr.op2 = op2;
    instr.op3 = op3;

    return iListAppend(list, instr);
}

/*
void main()
{
    tIList *mujList = iListInit();
    if (mujList == NULL)
        return;

    iListCreateAppend(mujList, BLA, NULL, NULL, NULL);
    iListCreateAppend(mujList, BLA, NULL, NULL, NULL);
    iListCreateAppend(mujList, BLA, NULL, NULL, NULL);
    iListCreateAppend(mujList, BLA, NULL, NULL, NULL);
    iListCreateAppend(mujList, BLA, NULL, NULL, NULL);

    iListDestroy(mujList);

}
*/
