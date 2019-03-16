#ifndef BITCOIN_H
#define BITCOIN_H

#include "wallet.h"
#include "transaction.h"

typedef struct Params {
    unsigned long int bid;
    char *walletId;
    unsigned long int v;
} ht_bitcoin_params;

typedef struct BitCoin *bitcoin;

typedef struct Node *bcNode;

void bcTrace(bitcoin bc, unsigned long int *value, unsigned long int *transactions, unsigned long int *unspent,
             bool traceTransaction, bool printTransactions);

bool bcInsert(bitcoin bc, unsigned long int *rest, unsigned long int *amount, Transaction transaction);

bool bcPrint(bitcoin bc);

long unsigned int bcGetId(bitcoin bc);

/* Create
 * Initialize & return a new wallet*/
bitcoin bcCreate(ht_bitcoin_params *htBitCoinParams);

/* @Callback
 * Compare trees function for bitcoins hashtable*/
int bcCompare(bitcoin bc1, bitcoin bc2);

/* @Callback
 * Hash function for bitcoins hashtable*/
unsigned long int bitcoinHash(const long int *bid, unsigned long int capacity);

/* @Callback
 * Compare keys function for wallets hashtable*/
void bcDestroy(bitcoin bc);

#endif //BITCOIN_H
