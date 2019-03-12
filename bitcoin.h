#ifndef BITCOIN_H
#define BITCOIN_H

#include "wallet.h"
#include "transaction.h"

typedef struct Params {
    unsigned long int bid;
    Wallet wallet;
    unsigned long int v;
} ht_bitCoin_params;

typedef struct BitCoin *bitCoin;

typedef struct Node *bcNode;

bool bcInsert(bitCoin bc, bcNode l, bcNode r, Transaction transaction);

long unsigned int bcGetId(bitCoin bc);

void bcDestroyNode(bcNode bc);

/* Create
 * Initialize & return a new wallet*/
bitCoin bcCreate(ht_bitCoin_params *htBitCoinParams);

/* @Callback
 * Compare trees function for bitCoins hashtable*/
int bcCompare(bitCoin bc1, bitCoin bc2);

/* @Callback
 * Hash function for bitCoins hashtable*/
unsigned long int bitCoinHash(const long int *bid, unsigned long int capacity);

/* @Callback
 * Compare keys function for wallets hashtable*/
void bcDestroy(bitCoin *bc);

#endif //BITCOIN_H
