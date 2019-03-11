#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <bits/types/time_t.h>
#include "tree.h"
#include "hash.h"

#define BUF 256

typedef struct Transaction {
    char *transactionId;
    void *treeNode;
    unsigned long int value;
    time_t timestamp;
} *Transaction;

/*Create
 * Initialize & return a new transaction*/
Transaction createTransaction(char *userId);

/*Callback
 * Compare transaction with userId field*/
int cmpTransaction(Transaction transaction, char *transactionId);

/*Callback
 * Hash function for transactions hashtable*/
unsigned long int transactionHash(char *key, unsigned long int capacity);

/*Execute
 * Execute transaction from input buffer*/
bool performTransaction(char *token,
                        hashtable *wallets,
                        hashtable *bitCoins,
                        hashtable *senderHashtable,
                        hashtable *receiverHashtable,
                        hashtable *transactionsHashtable
);

/*Parse
 * Parse transactions from input stream*/
bool performTransactions(
        FILE *fp,
        hashtable *wallets,
        hashtable *bitCoins,
        hashtable *senderHashtable,
        hashtable *receiverHashtable,
        hashtable *transactionsHashtable,
        char *delim
);

/*Callback
 * Destroy transaction*/
void destroyTransaction(Transaction transaction);

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
