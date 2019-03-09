#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdbool.h>
#include <stdio.h>

typedef struct Hashtable *hashtable;

bool HT_Init(hashtable *ht, unsigned long capacity, unsigned long int bucketSize, int (*cmp)(void *, void *, void *),
             unsigned long (*hash)(void *, void *), unsigned long (*destroy)(void *), void *params);

int HT_Insert(hashtable ht, void *key, void *value);

void *HT_Get(hashtable ht, void *key);

void HT_Destroy(hashtable *ht);

#endif
