#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdbool.h>
#include <stdio.h>

#define HT_ERROR 1;
#define HT_OK 0;

typedef struct Hashtable *hashtablePtr;

bool HT_Create(hashtablePtr *ht, unsigned long capacity, unsigned int bucketSize, int (*cmp)(void *, void *),
               unsigned long (*hash)(void *, void *), void *params);

int HT_Insert(hashtablePtr ht, void *key, void *value);

void HT_Destroy(hashtablePtr ht);

#endif
