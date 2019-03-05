#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "hash.h"

void getCount(void *bucket, int bucketSize, int *count) {
    memcpy(count, bucket + bucketSize - sizeof(int) - sizeof(void *), sizeof(int));
}

void setCount(void *bucket, int bucketSize, const int count) {
    memcpy(bucket + bucketSize - sizeof(int) - sizeof(void *), &count, sizeof(int));
}

void getNext(void *bucket, int bucketSize, void *next) {
    memcpy(next, bucket + bucketSize - sizeof(void *), sizeof(void *));
}

void setNext(void *bucket, int bucketSize, const void *next) {
    memcpy(bucket + bucketSize - sizeof(void *), &next, sizeof(void *));
}

void getValue(void *bucket, const int offset, void *value) {
    memcpy(&value, bucket + offset * sizeof(void *), sizeof(void *));
}

void setValue(void *bucket, const int offset, const void *value) {
    memcpy(bucket + offset * sizeof(void *), &value, sizeof(void *));
}

struct hashtable {
    int bucketSize;
    void **table;
    unsigned long int capacity;

    int (*cmp )(void *, void *);

    unsigned long int (*hash)(void *key, void *params);

    void *params;
};

int HT_Create(hashtablePtr *ht, unsigned long capacity, int bucketSize, int (*cmp)(void *, void *),
              unsigned long (*hash)(void *, void *), void *params) {
    int i;
    *ht = (hashtablePtr) malloc(sizeof(struct hashtable));
    if ((*ht) != NULL) {
        (*ht)->bucketSize = bucketSize;
        (*ht)->capacity = capacity;
        (*ht)->table = malloc(sizeof(void *) * capacity);
        for (i = 0; i < capacity; i++) {
            (*ht)->table[i] = NULL;
        }
        (*ht)->cmp = cmp;
        (*ht)->hash = hash;
        (*ht)->params = params;
    }
    return 0;
}

void HT_Destroy(hashtablePtr ht) {
    free(ht->table);
}

unsigned long int HT_Insert(hashtablePtr ht, char *key, void *v) {
    int i, count = 0;
    unsigned long int position = 0;
    void *bucket = NULL, *value = NULL;

    position = ht->hash(key, ht->params);
    bucket = ht->table[position];
    if (bucket == NULL) {
        bucket = malloc((size_t) ht->bucketSize);
        setValue(bucket, 0, v);
        setCount(bucket, ht->bucketSize, 1);
        setNext(bucket, ht->bucketSize, NULL);
        ht->table[position] = bucket;
    } else {
        printf("BUCKET IS NOT NEW! ");
        getCount(bucket, ht->bucketSize, &count);
        for (i = 0; i < count; i++) {
            getValue(bucket, i, value);
            printf("[%p]", value);
        }
        printf("count = %d\n", count);
    }
    return position;
}

void HT_Delete(hashtablePtr ht, unsigned int slot, void *ht_node) {

}
