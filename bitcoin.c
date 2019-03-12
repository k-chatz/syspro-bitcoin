#include "bitcoin.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

typedef void *pointer;

struct BitCoin {
    unsigned long int bid;
    bitCoinNode root;
};

struct Node {
    pointer *parrent;
    struct Transaction *transaction;
    Wallet wallet;
    unsigned long int balance;
    bitCoinNode left, right;
};

/***Private functions***/

bitCoinNode _createNode() {
    bitCoinNode node = (bitCoinNode) malloc(sizeof(struct Node));
    if (node != NULL) {
        node->right = NULL;
        node->left = NULL;
        return node;
    } else
        return NULL;
}


/***Public functions***/
bool bcInsert(bitCoin tree, void *data) {
    assert(tree != NULL);
    assert(data != NULL);


    return NULL;
}

long unsigned int bcGetId(bitCoin bc) {
    return bc->bid;
}

/*Create
 * Initialize & return a new BitCoin*/
bitCoin bcCreate(ht_bitCoin_params *htBitCoinParams) {
    bitCoin bc = NULL;
    bc = (bitCoin) malloc(sizeof(struct BitCoin));
    if (bc != NULL) {
        bc->bid = htBitCoinParams->bid;
        bc->root = _createNode();
        bc->root->wallet = htBitCoinParams->wallet;
        bc->root->balance = htBitCoinParams->v;
    }
    return bc;
}

/*Callback
 * Compare trees function for bitCoins hashtable*/
int bcCompare(bitCoin bc1, bitCoin bc2) {
    return bc1->bid != bc2->bid;
}

/*Callback
 * Hash function for bitCoins hashtable*/
unsigned long int bitCoinHash(const long int *bid, unsigned long int capacity) {
    return *bid % capacity;
}

/*Callback
 * Compare keys function for wallets hashtable*/
void bcDestroy(bitCoin *bc) {
    printf("[%p]", *bc);
}
