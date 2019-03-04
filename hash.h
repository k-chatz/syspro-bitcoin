#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "wallet.h"
#include <stdio.h>

typedef struct hashtable *hashtablePtr;

int HT_Create(hashtablePtr *ht, int size, int bucketSize);

void HT_Destroy(hashtablePtr ht);

int HT_Insert(hashtablePtr ht, struct Wallet *wallet);

void HT_Delete(hashtablePtr ht, unsigned int slot, void *ht_node);

#endif