#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stdio.h>

typedef struct List *listPtr;

void listCreate(listPtr *list, void *identifier);

void *listGetIdentifier(listPtr);

bool listInsert(listPtr, void *);

void listSetCurrentToStart(listPtr list);

void *listNext(listPtr);

void listDestroy(listPtr *list);

#endif
