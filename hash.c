#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "hash.h"

typedef void *pointer;

struct Hashtable {
    unsigned long int bucketSize;
    unsigned long int capacity;

    int (*cmp )(pointer, pointer);

    unsigned long int (*hash)(pointer key, pointer params);

    pointer params;
    pointer *table;
};


/***Private functions***/

void _getCount(pointer bucket, unsigned long int bucketSize, unsigned long int *count) {
    memcpy(count, bucket + bucketSize - sizeof(unsigned long int) - sizeof(pointer), sizeof(unsigned long int));
}

void _setCount(pointer bucket, unsigned long int bucketSize, const unsigned long int count) {
    memcpy(bucket + bucketSize - sizeof(unsigned long int) - sizeof(pointer), &count, sizeof(unsigned long int));
}

void _getNext(pointer bucket, unsigned long int bucketSize, pointer *next) {
    memcpy(next, bucket + bucketSize - sizeof(pointer), sizeof(pointer));
}

void _setNext(pointer bucket, unsigned long int bucketSize, pointer next) {
    memcpy(bucket + bucketSize - sizeof(pointer), &next, sizeof(pointer));
}

void _getValue(pointer bucket, unsigned long int offset, pointer *value) {
    memcpy(value, bucket + offset * sizeof(pointer), sizeof(pointer));
}

void _setValue(pointer bucket, unsigned long int offset, pointer value) {
    memcpy(bucket + offset * sizeof(pointer), &value, sizeof(pointer));
}

unsigned long int _getEmptySlots(unsigned long int bucketSize, unsigned long int count) {
    return (bucketSize - sizeof(pointer) - sizeof(unsigned long int)) / sizeof(pointer) - count;
}

const pointer _allocBucket(unsigned long int size) {
    pointer bucket = malloc((size_t) size);
    _setNext(bucket, size, NULL);
    return bucket;
}


/***Public functions***/

bool HT_Create(hashtable *ht, unsigned long int capacity, unsigned long int bucketSize, int (*cmp)(pointer, pointer),
               unsigned long (*hash)(pointer, pointer), pointer params) {
    assert(bucketSize >= sizeof(pointer) * 2 + sizeof(unsigned long int));
    int i;
    *ht = (hashtable) malloc(sizeof(struct Hashtable));
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

int HT_Insert(hashtable ht, pointer key, pointer value) {
    unsigned int i = 0;
    unsigned long int index = 0, count = 0, slots = 0;
    pointer bucket = NULL, v = NULL, next = NULL;
    index = ht->hash(key, ht->params);
    //printf("\tPOSITION --> [%lu] VALUE --> [%p] ", index, value);
    bucket = ht->table[index];

    /* Check if current bucket exists */
    if (bucket == NULL) {
        bucket = _allocBucket(ht->bucketSize);
        _setValue(bucket, 0, value);
        _setCount(bucket, ht->bucketSize, 1);
        ht->table[index] = bucket;
    } else {
        //printf(" --> !!! Collision !!! ");
        next = bucket;

        /* Check each bucket to detect possibly duplicate values and
         * determine where is the target slot to write the new value.*/
        while (next != NULL) {
            _getCount(bucket, ht->bucketSize, &count);
            slots = _getEmptySlots(ht->bucketSize, count);
            for (i = 0; i < count; i++) {
                _getValue(bucket, i, &v);
                if (!ht->cmp(v, value)) {
                    //printf(":::DUPLICATE::: ");
                    return false;
                }
            }
            _getNext(bucket, ht->bucketSize, &next);
            if (next != NULL) {
                bucket = next;
            }
        };

        /*Check if there are exists empty slots at the last bucket*/
        if (slots) {
            //printf(":::BUCKET [%p] HAS %d SLOTS::: ", bucket, slots - 1);
            _setValue(bucket, i, value);
            _setCount(bucket, ht->bucketSize, count + 1);
        } else {
            //printf(":::BUCKET [%p] IS FULL::: ", bucket);
            pointer b = _allocBucket(ht->bucketSize);
            //printf(" --> ALLOCATE BUCKET [%p] ", b);
            _setNext(bucket, ht->bucketSize, b);
            _setValue(b, 0, value);
            _setCount(b, ht->bucketSize, 1);
        }
    }
    return true;
}

void HT_Destroy(hashtable ht) {
    free(ht->table);
}
