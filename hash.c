#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "hash.h"

typedef void *pointer;

void getCount(pointer bucket, int bucketSize, int *count) {
    memcpy(count, bucket + bucketSize - sizeof(int) - sizeof(pointer), sizeof(int));
}

void setCount(pointer bucket, int bucketSize, const int count) {
    memcpy(bucket + bucketSize - sizeof(int) - sizeof(pointer), &count, sizeof(int));
}

void getNext(pointer bucket, int bucketSize, pointer *next) {
    memcpy(next, bucket + bucketSize - sizeof(pointer), sizeof(pointer));
}

void setNext(pointer bucket, int bucketSize, pointer next) {
    memcpy(bucket + bucketSize - sizeof(pointer), &next, sizeof(pointer));
}

void getValue(pointer bucket, int offset, pointer *value) {
    memcpy(value, bucket + offset, sizeof(pointer));
}

void setValue(pointer bucket, int offset, pointer value) {
    memcpy(bucket + offset, &value, sizeof(pointer));
}

struct hashtable {
    int bucketSize;
    pointer *table;
    unsigned long int capacity;
    int (*cmp )(pointer, pointer);
    unsigned long int (*hash)(pointer key, pointer params);
    pointer params;
};

int HT_Create(hashtablePtr *ht, unsigned long capacity, int bucketSize, int (*cmp)(pointer, pointer),
              unsigned long (*hash)(pointer, pointer), pointer params) {
    int i;
    *ht = (hashtablePtr) malloc(sizeof(struct hashtable));
    if ((*ht) != NULL) {
        (*ht)->bucketSize = bucketSize;
        (*ht)->capacity = capacity;
        (*ht)->table = malloc(sizeof(pointer) * capacity);
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

unsigned long int HT_Insert(hashtablePtr ht, char *key, pointer v) {
    int i, count = 0;
    unsigned long int position = 0;
    pointer bucket = NULL, value = NULL, next = NULL;
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
        // Check values for each bucket
        do {
            for (i = 0; i < count; i++) {
                getValue(bucket, i, &value);
                if (!ht->cmp(value, v)) {
                    return HT_ERROR;
                }
            }
            getNext(bucket, ht->bucketSize, &next);
            bucket = next;
        } while (bucket != NULL);
    }
    printf("[%lu] ", position);

    return HT_OK;
}

void HT_Delete(hashtablePtr ht, unsigned int slot, pointer ht_node) {

}
