#include "bitcoin.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

typedef void *pointer;

struct BitCoin {
    unsigned long int bid;
    bcNode root;
};

struct Node {
    struct Transaction *transaction;
    Wallet wallet;
    unsigned long int balance;
    bcNode parrent, left, right;
};

/***Private functions***/

bcNode _createNode(bcNode parrent) {
    bcNode node = (bcNode) malloc(sizeof(struct Node));
    if (node != NULL) {
        node->parrent = parrent;
        node->right = NULL;
        node->left = NULL;
        return node;
    } else
        return NULL;
}


/***Public functions***/
bool bcInsert(bitCoin bc, bcNode l, bcNode r, Transaction transaction) {
    assert(bc != NULL);
    assert(transaction != NULL);



    _createNode(NULL);


    return true;
}

long unsigned int bcGetId(bitCoin bc) {
    return bc->bid;
}

void bcDestroyNode(bcNode bc){
    if(bc !=NULL){
        printf("\nbc node to be destroyed: [%p]\n", bc);
    }
}

/* @Callback
 * Initialize & return a new BitCoin*/
bitCoin bcCreate(ht_bitCoin_params *htBitCoinParams) {
    bitCoin bc = NULL;
    bc = (bitCoin) malloc(sizeof(struct BitCoin));
    if (bc != NULL) {
        bc->bid = htBitCoinParams->bid;
        bc->root = _createNode(NULL);
        bc->root->wallet = htBitCoinParams->wallet;
        bc->root->balance = htBitCoinParams->v;
    }
    return bc;
}

/* @Callback
 * Compare trees function for bitCoins hashtable*/
int bcCompare(bitCoin bc1, bitCoin bc2) {
    return bc1->bid != bc2->bid;
}

/* @Callback
 * Hash function for bitCoins hashtable*/
unsigned long int bitCoinHash(const long int *bid, unsigned long int capacity) {
    return *bid % capacity;
}

/* @Callback
 * Compare keys function for wallets hashtable*/
void bcDestroy(bitCoin *bc) {
    printf("[%p]", *bc);
}
