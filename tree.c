#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "tree.h"

typedef struct Node *treeNodePtr;

typedef void *pointer;

struct Tree {
    unsigned long int bid;
    treeNodePtr root;
};

struct Node {
    struct Wallet *wallet;
    unsigned long int balance;
    treeNodePtr left, right;
};

/***Private functions***/

treeNodePtr _createNode() {
    treeNodePtr node = (treeNodePtr) malloc(sizeof(struct Node));
    if (node != NULL) {
        node->right = NULL;
        node->left = NULL;
        return node;
    } else
        return NULL;
}


/***Public functions***/

void treeCreate(treePtr *tree, const unsigned long int bid, struct Wallet *w, const unsigned long int balance) {
    assert(*tree == NULL);
    *tree = (treePtr) malloc(sizeof(struct Tree));
    if ((*tree) != NULL) {
        (*tree)->bid = bid;
        (*tree)->root = _createNode();
        (*tree)->root->wallet = w;
        (*tree)->root->balance = balance;
    }
}

long unsigned int getBid(treePtr tree) {
    return tree->bid;
}

void treeDestroy(treePtr *tree) {
    assert(*tree != NULL);
    //TODO Delete all tree nodes!
    //free(tree);
}

bool treeInsert(treePtr tree, void *data) {
    assert(tree != NULL);
    assert(data != NULL);
    return NULL;
}
