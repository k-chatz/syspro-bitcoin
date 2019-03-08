#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdbool.h>
#include <stdio.h>

typedef struct Hashtable *hashtable;

bool HT_Create(hashtable *ht, unsigned long capacity, unsigned long int bucketSize, int (*cmp)(void *, void *),
               unsigned long (*hash)(void *, void *), void *params);

int HT_Insert(hashtable ht, void *key, void *value);

void HT_Destroy(hashtable ht);

#endif
