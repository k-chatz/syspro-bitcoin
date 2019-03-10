#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include "wallet.h"

typedef struct Tree *treePtr;

void treeCreate(treePtr *tree, long unsigned int bid, struct Wallet * w, unsigned long int balance);

long unsigned int getBid(treePtr);

bool treeInsert(treePtr, void *);

void treeDestroy(treePtr *tree);

#endif
