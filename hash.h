#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdbool.h>
#include <stdio.h>

typedef struct Hashtable *hashtable;

bool HT_Init(
        hashtable *ht,
        unsigned long capacity,
        unsigned long int bucketSize,
        void *(*createValue)(void *),
        int (*cmp)(void *, void *),
        unsigned long (*hash)(void *, unsigned long int),
        unsigned long (*destroy)(void *)
);

int HT_Insert(hashtable ht, void *key, void *valueParams, void **value);

void *HT_Get(hashtable ht, void *key);

void HT_Destroy(hashtable *ht);

#endif
