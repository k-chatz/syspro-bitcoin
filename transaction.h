#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <bits/types/time_t.h>
#include "hash.h"
#include "wallet.h"

#define BUF 256

extern time_t max_transaction_timestamp;
extern bool init_complete;

typedef struct Transaction {
    char *transactionId;
    char *senderWalletId;
    char *receiverWalletId;
    unsigned long int value;
    time_t timestamp;
} *Transaction;

/* Execute
 * transaction*/
bool execute(Wallet senderWallet, listPtr senderTransactions, Wallet receiverWallet, listPtr receiverTransactions,
             Transaction transaction);

/* Perform
 * transaction from input buffer*/
bool performTransaction(char *token, Hashtable *walletsHT, Hashtable *senderHT, Hashtable *receiverHT,
                        Hashtable *transactionsHT);

/* Perform
 * transactions from input stream*/
bool performTransactions(FILE *fp, Hashtable *walletsHT, Hashtable *senderHT, Hashtable *receiverHT,
                         Hashtable *transactionsHT, char *delimiter);

/* @Callback
 * Initialize & return a new transaction*/
Transaction createTransaction(char *token);

/* @Callback
 * Compare transaction with userId field*/
int cmpTransaction(Transaction transaction, char *transactionStr);

/* @Callback
 * Hash function for transactions hashtable*/
unsigned long int transactionHash(char *key, unsigned long int capacity);

/* @Callback
 * Destroy transaction*/
void destroyTransaction(Transaction transaction);

/* @Callback
 * Initialize & return a new transaction list*/
listPtr createTransactionList(char *userId);

/* @Callback
 * Compare keys function for transaction lists hashtable*/
int cmpTransactionList(listPtr tr1, char *userId);

/* @Callback
 * Destroy function for transaction hashtable*/
void destroyTransactionList(listPtr list);

#endif //TRANSACTION_H
