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

/*Create
 * Initialize & return a new transaction list*/
listPtr createTransactionList(char *userId);

/*Callback
 * Compare keys function for transaction lists hashtable*/
int cmpTransactionList(listPtr tr1, char *userId);

/*Callback
 * Compare keys function for wallets hashtable*/
void destroyTransactionList(listPtr list);

#endif //TRANSACTION_H
