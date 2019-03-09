#include "tree.h"
#include "transaction.h"

/*Callback
 * Compare keys function for transaction lists hashtable*/
int cmpTransactionList(listPtr *tr1, listPtr *tr2, void *key) {

    return 0;
}

/*Callback
 * Destroy function for transaction hashtable*/
void destroyTransactionList(listPtr list) {
    listDestroy(list);
}
