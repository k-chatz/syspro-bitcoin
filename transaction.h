#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "tree.h"

struct Transaction {
    char *transactionId;
    char *senderWalletId;
    char *reciverWalletId;
    int value;
    int date; //TODO type check
    int time;
};

typedef struct HT_TransactionsListParams {
    unsigned long int capacity;
} ht_transaction_list_params;

/*Callback
 * Compare keys function for transaction lists hashtable*/
int cmpTransactionList(listPtr *tr1, listPtr *tr2, void *key);

/*Callback
 * Compare keys function for wallets hashtable*/
void destroyTransactionList(listPtr list);

#endif //TRANSACTION_H
