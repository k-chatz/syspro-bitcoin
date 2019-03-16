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
    unsigned long int nodes;
};

struct Node {
    struct Transaction *transaction;
    char *walletId;
    unsigned long int balance;
    bcNode parent, left, right;
};


/***Private functions***/

bool _isLeaf(bcNode node) {
    return node->left == NULL && node->right == NULL;
}


bool _isLeft(bcNode root) {
    return root->parent->left == root;
}


bool _isRight(bcNode root) {
    return root->parent->right == root;
}

bool _isTarget(bcNode node, char *walletId) {
    return !strcmp(node->walletId, walletId);
}

bcNode _createNode(bcNode parent, unsigned long int balance, char *walletId, Transaction transaction) {
    bcNode node = (bcNode) malloc(sizeof(struct Node));
    if (node != NULL) {
        node->balance = balance;
        node->walletId = malloc(strlen(walletId) * sizeof(char) + 1);
        strcpy(node->walletId, walletId);
        node->transaction = transaction;
        node->parent = parent;
        node->right = NULL;
        node->left = NULL;
        return node;
    } else
        return NULL;
}

/***Public functions***/

void bcTrace(bitcoin bc, unsigned long int *value, unsigned long int *transactions, unsigned long int *unspent,
             bool traceTransaction, bool printTransactions) {
    assert(bc != NULL);
    Queue queue = NULL;
    queueCreate(&queue, bc->nodes);
    bcNode root = bc->root;
    List list = NULL;
    listCreate(&list, "");
    Transaction transaction = NULL;
    bool duplicate;

    printf("Trace for bitcoin %lu with initial owner '%s' and initial balance: %lu$\n",
           bc->bid, bc->root->walletId, root->balance);

    *value = root->balance;

    while (root != NULL) {
        duplicate = false;

        if (root->parent != NULL) {
            if (_isLeaf(root) && _isRight(root)) {
                (*unspent) += root->balance;
            }
        }

        if (traceTransaction && root->transaction != NULL) {
            while ((transaction = listNext(list)) != NULL) {
                if (!strcmp(transaction->transactionId, root->transaction->transactionId)) {
                    duplicate = true;
                }
            }

            if (!duplicate && _isLeft(root)) {
                (*transactions)++;
                if (printTransactions) {
                    listInsert(list, root->transaction);
                }
            }
        }

        if (root->left != NULL) {
            enQueue(queue, root->left);
        }
        if (root->right != NULL) {
            enQueue(queue, root->right);
        }
        root = deQueue(queue);
    }
    listSetCurrentToStart(list);
    while ((transaction = listNext(list)) != NULL) {
        transactionPrint(transaction);
    }
    queueDestroy(&queue);
}

bool bcInsert(bitcoin bc, unsigned long int *rest, unsigned long int *amount, Transaction transaction) {
    assert(bc != NULL);
    assert(*rest > 0);
    assert(transaction != NULL);
    Queue queue = NULL;
    queueCreate(&queue, bc->nodes);
    bcNode r = bc->root;
    unsigned long int balance = 0;

    while (r != NULL) {
        //printf("BID: '%lu'\tNODE: [%p]\tBalance: %lu$\n", bc->bid, sender, sender->balance);
        printf("Visit: [%p][%s|%lu$]\n", r, r->walletId, r->balance);

        balance = r->balance;

        if (r->left != NULL) {
            enQueue(queue, r->left);
        }
        if (r->right != NULL) {
            enQueue(queue, r->right);
        }

        /* Check if this node is target*/
        if (_isTarget(r, transaction->senderWalletId)) {

            /* Is current node a leaf ?*/
            if (_isLeaf(r)) {
                *amount += balance;

                printf("Target leaf: [%p][%s|%lu$]\n", r, r->walletId, r->balance);

                if (*rest >= balance) {

                    /* Left node for receiver*/
                    r->left = _createNode(r, balance, transaction->receiverWalletId, transaction);
                    bc->nodes++;
                    printf("New left child: [%p][%s|%lu$]\n", r->left, transaction->receiverWalletId, balance);
                    *rest -= balance;
                    *amount -= balance;

                } else if (*rest < balance) {

                    /* Left node for receiver*/
                    r->left = _createNode(r, *rest, transaction->receiverWalletId, transaction);
                    bc->nodes++;
                    printf("New left child: [%p][%s|%lu$] | ", r->left, transaction->receiverWalletId, *rest);
                    *amount -= *rest;

                    /* Right node for sender's rest*/
                    r->right = _createNode(r, r->balance - *rest, transaction->senderWalletId, transaction);
                    bc->nodes++;
                    printf("New right child: [%p][%s|%lu$]\n", r->right, transaction->senderWalletId,
                           r->balance - *rest);
                    *rest = 0;
                }
            }
        }
        r = deQueue(queue);
    }
    queueDestroy(&queue);
    return true;
}

bool bcPrint(bitcoin bc) {
    assert(bc != NULL);
    Queue queue = NULL;
    queueCreate(&queue, bc->nodes);
    bcNode r = bc->root;
    printf("\nPrint bitcoin %lu nodes:\n", bc->bid);
    while (r != NULL) {
        printf("[%s|%lu$]\n", r->walletId, r->balance);
        if (r->left != NULL) {
            enQueue(queue, r->left);
        }
        if (r->right != NULL) {
            enQueue(queue, r->right);
        }
        r = deQueue(queue);
    }
    queueDestroy(&queue);
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
        bc->nodes = 1;
        bc->bid = htBitCoinParams->bid;
        bc->root = _createNode(NULL, htBitCoinParams->v, htBitCoinParams->walletId, NULL);
        if (bc->root == NULL) {
            bcDestroy(bc);
            return NULL;
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
    Queue queue = NULL;
    queueCreate(&queue, bc->nodes);
    bcNode root = bc->root;
    while (root) {
        if (root->left) {
            enQueue(queue, root->left);
        }

        if (root->right) {
            enQueue(queue, root->right);
        }

        free(root->walletId);
        free(root);

        root = deQueue(queue);
    }
    queueDestroy(&queue);
    free(bc);
}
