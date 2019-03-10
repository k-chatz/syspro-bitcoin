#ifndef BITCOIN_H
#define BITCOIN_H

#include "tree.h"

typedef struct Params {
    unsigned long int bid;
    struct Wallet *wallet;
    unsigned long int v;
} ht_bitCoin_params;

/*Create
 * Initialize & return a new wallet*/
treePtr createBitCoin(ht_bitCoin_params *htBitCoinParams);

/*Callback
 * Compare trees function for bitCoins hashtable*/
int cmpBitCoin(treePtr t1, treePtr t2);

/*Callback
 * Hash function for bitCoins hashtable*/
unsigned long int bitCoinHash(const long int *bid1, unsigned long int capacity);

/*Callback
 * Compare keys function for wallets hashtable*/
void destroyBitCoin(treePtr tree);

#endif //BITCOIN_H
