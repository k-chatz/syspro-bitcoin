#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tree.h"

typedef struct Node *nodePtr;

typedef void *pointer;

struct Tree {
    long int bid;
    nodePtr root;
};

struct Node {
    nodePtr left, right;
    pointer data;
};


/***Private functions***/

nodePtr _createNode() {
    nodePtr node = (nodePtr) malloc(sizeof(struct Node));
    if (node != NULL) {
        node->right = NULL;
        node->left = NULL;
        return node;
    } else
        return NULL;
}


/***Public functions***/

void treeCreate(treePtr *tree, const long int bid, pointer data) {
    assert(*tree == NULL);
    *tree = (treePtr) malloc(sizeof(struct Tree));
    if ((*tree) != NULL) {
        (*tree)->bid = bid;
        (*tree)->root = _createNode();
        (*tree)->root->data = data;
    }
}

void *treeInsert(treePtr tree, void *data) {
    assert(tree != NULL);
    assert(data != NULL);
    return NULL;
}
