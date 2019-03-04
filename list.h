#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stdio.h>

typedef struct list *listPtr;

bool listExists(listPtr *list);

void listCreate(listPtr *list);

void *listGetFirstData(listPtr list);

void *listInsert(listPtr, void *);

void *listNext(listPtr);

void *listPrevious(listPtr);

void listPrintContent(listPtr list, FILE *out);

#endif