#ifndef BITCOIN_H
#define BITCOIN_H

#include "wallet.h"

typedef struct Params {
    unsigned long int bid;
    Wallet wallet;
    unsigned long int v;
} ht_bitCoin_params;

typedef struct BitCoin *bitCoin;

typedef struct Node *bitCoinNode;

bool bcInsert(bitCoin tree, void *data);

long unsigned int bcGetId(bitCoin bc);

/*Create
 * Initialize & return a new wallet*/
bitCoin bcCreate(ht_bitCoin_params *htBitCoinParams);

/*Callback
 * Compare trees function for bitCoins hashtable*/
int bcCompare(bitCoin bc1, bitCoin bc2);

/*Callback
 * Hash function for bitCoins hashtable*/
unsigned long int bitCoinHash(const long int *bid, unsigned long int capacity);

/*Callback
 * Compare keys function for wallets hashtable*/
void bcDestroy(bitCoin *bc);

#endif //BITCOIN_H
