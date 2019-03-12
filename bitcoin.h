#ifndef BITCOIN_H
#define BITCOIN_H

#include "wallet.h"
#include "transaction.h"

typedef struct Params {
    unsigned long int bid;
    Wallet wallet;
    unsigned long int v;
} ht_bitcoin_params;

typedef struct BitCoin *bitcoin;

typedef struct Node *bcNode;

bool bcInsert(bitcoin bc, bcNode l, bcNode r, Transaction transaction);

long unsigned int bcGetId(bitcoin bc);

void bcDestroyNode(bcNode bc);

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
void bcDestroy(bitcoin *bc);

#endif //BITCOIN_H
