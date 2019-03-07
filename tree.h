#ifndef TREE_H
#define TREE_H

#include <stdio.h>

typedef struct Tree *treePtr;

void treeCreate(treePtr *tree, long int bid, void * data);

bool treeInsert(treePtr, void *);

void treeDestroy(treePtr *tree);

#endif