#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <bits/types/time_t.h>
#include "tree.h"

#define BUF 256

typedef struct Transaction {
    char *transactionId;
    void *node;
    unsigned long int value;
    time_t timestamp;
} *Transaction;

/*Create
 * Initialize & return a new transaction*/
Transaction createTransaction(char *buf);

bool performTransaction(char *buf);

/*Parse
 * Parse transactions from input stream*/
bool performTransactions(FILE *fp);

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
