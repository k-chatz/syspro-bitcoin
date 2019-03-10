#include "tree.h"
#include "transaction.h"

/*Create
 * Initialize & return a new transaction list*/
listPtr createTransactionList(char *userId) {
    listPtr list = NULL;
    listCreate(&list, userId);
    return list;
}

/*Callback
 * Compare keys function for transaction lists hashtable*/
int cmpTransactionList(listPtr tr1, char *userId) {
    return listGetIdentifier(tr1) != userId;
}

/*Callback
 * Destroy function for transaction hashtable*/
void destroyTransactionList(listPtr list) {
    listDestroy(list);
}
