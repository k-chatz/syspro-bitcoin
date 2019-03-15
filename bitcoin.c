#include "bitcoin.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "queue.h"

typedef void *pointer;

struct BitCoin {
    unsigned long int bid;
    bcNode root;
};

struct Node {
    struct Transaction *transaction;
    char *walletId;
    unsigned long int balance;
    bcNode parrent, left, right;
};


/***Private functions***/

bool _isLeaf(bcNode node) {
    return node->left == NULL && node->right == NULL;
}

bool _isTarget(bcNode node, char *walletId) {
    return !strcmp(node->walletId, walletId);
}

bcNode _createNode() {
    bcNode node = (bcNode) malloc(sizeof(struct Node));
    if (node != NULL) {
        node->parrent = NULL;
        node->right = NULL;
        node->left = NULL;
        return node;
    } else
        return NULL;
}

void _destroyNode(bcNode bc) {
    fprintf(stdout, "Bitcoin node to be destroyed: [%p]\n", bc);


    free(bc);

}

/***Public functions***/

bool bcInsert(bitcoin bc, unsigned long int *rest, Transaction transaction) {
    assert(bc != NULL);
    assert(*rest > 0);
    assert(transaction != NULL);

            printf(" ");
        }

        r = deQueue((pointer) queue, &front);
    }

    printf("\n");
    return true;
}


long unsigned int bcGetId(bitcoin bc) {
    return bc->bid;
}

/* @Callback
 * Initialize & return a new BitCoin*/
bitcoin bcCreate(ht_bitcoin_params *htBitCoinParams) {
    bitcoin bc = NULL;
    bc = (bitcoin) malloc(sizeof(struct BitCoin));
    if (bc != NULL) {
        bc->bid = htBitCoinParams->bid;
        bc->root = _createNode();
        if (bc->root != NULL) {
            bc->root->transaction = NULL;
            bc->root->walletId = malloc(strlen(htBitCoinParams->walletId) * sizeof(char) + 1);
            strcpy(bc->root->walletId, htBitCoinParams->walletId);
            bc->root->balance = htBitCoinParams->v;
        }
    }
    return bc;
}

/* @Callback
 * Compare bitcoins*/
int bcCompare(bitcoin bc1, bitcoin bc2) {
    return bc1->bid != bc2->bid;
}

/* @Callback
 * Hash bitcoin*/
unsigned long int bitcoinHash(const long int *bid, unsigned long int capacity) {
    return *bid % capacity;
}

/* @Callback
 * Destroy bitcoin*/
void bcDestroy(bitcoin bc) {
    assert(bc != NULL);
    printf("\nbcDestroy: [%p] ", bc);

    //TODO: BFS to destroy all nodes!
    _destroyNode(bc->root);

    
    free(bc);
}
