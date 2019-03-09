#ifndef BITCOIN_H
#define BITCOIN_H

#include "tree.h"

typedef struct Params {
    unsigned long int capacity;
} ht_bitcoin_params;

/*Callback
 * Compare trees function for bitCoins hashtable*/
int cmpBitCoin(treePtr t1, treePtr t2, void *key);

/*Callback
 * Hash function for bitCoins hashtable*/
unsigned long int bitCoinHash(const long int *bid1, ht_bitcoin_params *params);

/*Callback
 * Compare keys function for wallets hashtable*/
void destroyBitCoin(treePtr tree);

#endif //BITCOIN_H
