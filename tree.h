#ifndef TREE_H
#define TREE_H

#include <stdio.h>

typedef struct Tree *treePtr;

void treeCreate(treePtr *tree, long int bid, void * data);

void *treeInsert(treePtr, void *);

#endif