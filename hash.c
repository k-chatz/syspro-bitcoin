#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "hash.h"

typedef void *pointer;

struct Hashtable {
    unsigned int bucketSize;
    unsigned long int capacity;

    int (*cmp )(pointer, pointer);

    unsigned long int (*hash)(pointer key, pointer params);

    pointer params;
    pointer *table;
};


/***Private functions***/

void _getCount(pointer bucket, unsigned int bucketSize, unsigned int *count) {
    memcpy(count, bucket + bucketSize - sizeof(unsigned int) - sizeof(pointer), sizeof(unsigned int));
}

void _setCount(pointer bucket, unsigned int bucketSize, const unsigned int count) {
    memcpy(bucket + bucketSize - sizeof(unsigned int) - sizeof(pointer), &count, sizeof(unsigned int));
}

void _getNext(pointer bucket, unsigned int bucketSize, pointer *next) {
    memcpy(next, bucket + bucketSize - sizeof(pointer), sizeof(pointer));
}

void _setNext(pointer bucket, unsigned int bucketSize, pointer next) {
    memcpy(bucket + bucketSize - sizeof(pointer), &next, sizeof(pointer));
}

void _getValue(pointer bucket, unsigned int offset, pointer *value) {
    memcpy(value, bucket + offset * sizeof(pointer), sizeof(pointer));
}

void _setValue(pointer bucket, unsigned int offset, pointer value) {
    memcpy(bucket + offset * sizeof(pointer), &value, sizeof(pointer));
}

unsigned int _getEmptySlots(unsigned int bucketSize, unsigned int count) {
    return (bucketSize - sizeof(pointer) - sizeof(unsigned int)) / sizeof(pointer) - count;
}

const pointer _allocBucket(unsigned int size) {
    pointer bucket = malloc((size_t) size);
    _setNext(bucket, size, NULL);
    return bucket;
}


/***Public functions***/

bool HT_Create(hashtablePtr *ht, unsigned long capacity, unsigned int bucketSize, int (*cmp)(pointer, pointer),
               unsigned long (*hash)(pointer, pointer), pointer params) {
    assert(bucketSize >= sizeof(pointer) * 2 + sizeof(unsigned int));
    int i;
    *ht = (hashtablePtr) malloc(sizeof(struct Hashtable));
    if ((*ht) != NULL) {
        (*ht)->bucketSize = bucketSize;
        (*ht)->capacity = capacity;
        (*ht)->cmp = cmp;
        (*ht)->hash = hash;
        (*ht)->params = params;
        (*ht)->table = malloc(sizeof(pointer) * capacity);
        for (i = 0; i < capacity; i++) {
            (*ht)->table[i] = NULL;
        }
    }
    return 0;
}

int HT_Insert(hashtablePtr ht, pointer key, pointer value) {
    unsigned int count = 0, slots = 0, i = 0;
    unsigned long int index = 0;
    pointer bucket = NULL, v = NULL, next = NULL;
    index = ht->hash(key, ht->params);
    printf("\tPOSITION --> [%lu] VALUE --> [%p] ", index, value);
    bucket = ht->table[index];

    /* Check if current bucket exists */
    if (bucket == NULL) {
        bucket = _allocBucket(ht->bucketSize);
        _setValue(bucket, 0, value);
        _setCount(bucket, ht->bucketSize, 1);
        ht->table[index] = bucket;
    } else {
        printf(" --> !!! Collision !!! ");
        next = bucket;

        /* Check each bucket to detect possibly duplicate values and
         * determine where is the target slot to write the new value.*/
        while (next != NULL) {
            _getCount(bucket, ht->bucketSize, &count);
            slots = _getEmptySlots(ht->bucketSize, count);
            for (i = 0; i < count; i++) {
                _getValue(bucket, i, &v);
                if (!ht->cmp(v, value)) {
                    printf(":::DUPLICATE::: ");
                    return HT_ERROR;
                }
            }
            _getNext(bucket, ht->bucketSize, &next);
            if (next != NULL) {
                bucket = next;
            }
        };

        /*Check if there are exists empty slots at the last bucket*/
        if (slots) {
            printf(":::BUCKET [%p] HAS %d SLOTS::: ", bucket, slots - 1);
            _setValue(bucket, i, value);
            _setCount(bucket, ht->bucketSize, count + 1);
        } else {
            printf(":::BUCKET [%p] IS FULL::: ", bucket);
            pointer b = _allocBucket(ht->bucketSize);
            printf(" --> ALLOCATE BUCKET [%p] ", b);
            _setNext(bucket, ht->bucketSize, b);
            _setValue(b, 0, value);
            _setCount(b, ht->bucketSize, 1);
        }
    }
    return HT_OK;
}

void HT_Destroy(hashtablePtr ht) {
    free(ht->table);
}
