#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include "wallet.h"

typedef struct Tree *treePtr;

void treeCreate(treePtr *tree, long unsigned int bid, Wallet w, unsigned long int balance);

long unsigned int treeGetBid(treePtr tree);

bool treeInsert(treePtr, void *);

void treeDestroy(treePtr *tree);

#endif
