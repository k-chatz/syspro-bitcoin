#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stdio.h>

typedef struct List *listPtr;

void listCreate(listPtr *list);

bool listInsert(listPtr, void *);

void *listNext(listPtr);

void listDestroy(listPtr list);

#endif
