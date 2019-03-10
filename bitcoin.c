#include "tree.h"
#include "bitcoin.h"

/*Create
 * Initialize & return a new wallet*/
treePtr createBitCoin(ht_bitCoin_params *htBitCoinParams) {
    treePtr bc = NULL;
    /*Create bitCoin & insert bitCoin into hashtable*/
    treeCreate(&bc, htBitCoinParams->bid, htBitCoinParams->wallet, htBitCoinParams->v);
    return bc;
}


/*Callback
 * Compare trees function for bitCoins hashtable*/
int cmpBitCoin(treePtr t1, treePtr t2) {
    return treeGetBid(t1) != treeGetBid(t2);
}

/*Callback
 * Hash function for bitCoins hashtable*/
unsigned long int bitCoinHash(const long int *bid1, unsigned long int capacity) {
    return *bid1 % capacity;
}

/*Callback
 * Compare keys function for wallets hashtable*/
void destroyBitCoin(treePtr tree) {
    treeDestroy(&tree);
}
