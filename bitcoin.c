#include "tree.h"
#include "bitcoin.h"

/*Callback
 * Compare trees function for bitCoins hashtable*/
int cmpBitCoin(treePtr t1, treePtr t2) {
    return getBid(t1) != getBid(t2);
}

/*Callback
 * Hash function for bitCoins hashtable*/
unsigned long int bitCoinHash(const long int *bid1, ht_bitcoin_params *params) {
    return *bid1 % params->capacity;
}

/*Callback
 * Compare keys function for wallets hashtable*/
void destroyBitCoin(treePtr tree) {
    treeDestroy(&tree);
}
