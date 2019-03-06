#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "wallet.h"
#include <stdio.h>

#define HT_ERROR 1;
#define HT_OK 0;

typedef struct hashtable *hashtablePtr;

int HT_Create(hashtablePtr *ht, unsigned long capacity, int bucketSize, int (*cmp)(void *, void *),
              unsigned long (*hash)(void *, void *), void * params);

void HT_Destroy(hashtablePtr ht);

unsigned long int HT_Insert(hashtablePtr ht, char *key, void *v);

void HT_Delete(hashtablePtr ht, unsigned int slot, void *ht_node);

#endif