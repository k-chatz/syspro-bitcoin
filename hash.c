#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "hash.h"

typedef void *pointer;

struct Hashtable {
    unsigned long int bucketSize;
    unsigned long int capacity;

    pointer (*createValue )(pointer);

    int (*cmp )(pointer, pointer);

    unsigned long int (*hash)(pointer, unsigned long int);

    unsigned long int (*destroy)(pointer);

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

bool HT_Init(hashtable *ht,
             unsigned long int capacity,
             unsigned long int bucketSize,
             pointer (*createValue)(pointer),
             int (*cmp)(pointer, pointer),
             unsigned long (*hash)(pointer, unsigned long int),
             unsigned long (*destroy)(pointer)
) {
    assert(bucketSize >= sizeof(pointer) * 2 + sizeof(unsigned long int));
    assert(capacity > 0);
    int i;
    *ht = (hashtable) malloc(sizeof(struct Hashtable));
    if ((*ht) != NULL) {
        (*ht)->bucketSize = bucketSize;
        (*ht)->capacity = capacity;
        (*ht)->createValue = createValue;
        (*ht)->cmp = cmp;
        (*ht)->hash = hash;
        (*ht)->destroy = destroy;
        (*ht)->table = malloc(sizeof(pointer) * capacity);
        for (i = 0; i < capacity; i++) {
            (*ht)->table[i] = NULL;
        }
        return true;
    }
    return false;
}

int HT_Insert(
        hashtable ht,
        pointer key,
        pointer valueParams,
        pointer *value
) {
    unsigned long int index = 0, count = 0, slots = 0, slot = 0;
    pointer bucket = NULL, next = NULL, slotValue = NULL;
    index = ht->hash(key, ht->capacity);
    //printf("[%.3lu] ", index);
    bucket = ht->table[index];

    /* Check if current bucket exists */
    if (bucket == NULL) {
        bucket = _allocBucket(ht->bucketSize);
        slotValue = ht->createValue(valueParams);
        _setValue(bucket, 0, slotValue);
        *value = slotValue;
        //printf("--> [%p] ", *value);
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

            /*Get value for each slot of bucket*/
            for (slot = 0; slot < count; slot++) {
                _getValue(bucket, slot, &slotValue);
                if (!ht->cmp(slotValue, valueParams)) {
                    //printf(":::DUPLICATE::: ");
                    //printf("\n");
                    *value = slotValue;
                    return false;
                }
            }

            /*Get next pointer to determine if this bucket has an overflow bucket*/
            _getNext(bucket, ht->bucketSize, &next);

            if (next != NULL) {
                bucket = next;
            }
        };

        /*Check if there are exists empty slots at the last bucket*/
        if (slots) {
            //printf(":::BUCKET [%p] HAS %lu SLOTS::: ", bucket, slots - 1);
            slotValue = ht->createValue(valueParams);
            _setValue(bucket, slot, slotValue);
            *value = slotValue;
            _setCount(bucket, ht->bucketSize, count + 1);
        } else {
            //printf(":::BUCKET [%p] IS FULL::: ", bucket);
            pointer b = _allocBucket(ht->bucketSize);
            //printf(" --> ALLOCATE BUCKET [%p] ", b);
            _setNext(bucket, ht->bucketSize, b);
            slotValue = ht->createValue(valueParams);
            _setValue(b, 0, slotValue);
            *value = slotValue;
            _setCount(b, ht->bucketSize, 1);
        }
    }
    //printf("\n");
    return true;
}

pointer HT_Get(
        hashtable ht,
        pointer key
) {
    unsigned long int index = 0, count = 0, slot = 0;
    pointer bucket = NULL, v = NULL, next = NULL;

    index = ht->hash(key, ht->capacity);
    bucket = ht->table[index];

    /* Check if current bucket exists */
    if (bucket != NULL) {
        next = bucket;

        /* Check each bucket to determine where is the target slot.*/
        while (next != NULL) {
            _getCount(bucket, ht->bucketSize, &count);

            /*Get value for each slot of bucket*/
            for (slot = 0; slot < count; slot++) {
                _getValue(bucket, slot, &v);
                if (!ht->cmp(v, key)) {
                    return v;
                }
            }

            /*Get next pointer to determine if this bucket has an overflow bucket*/
            _getNext(bucket, ht->bucketSize, &next);
            if (next != NULL) {
                bucket = next;
            }
        };
    }
    return NULL;
}

void HT_Destroy(
        hashtable *ht
) {
    assert((*ht) != NULL);
    pointer next = NULL, v = NULL, bucket = NULL;
    unsigned long int count = 0, i, slot;
    //printf("\n");
    for (i = 0; i < (*ht)->capacity; i++) {
        bucket = next = (*ht)->table[i];
        if (bucket != NULL) {
            //printf("[%.3lu] -->", i);

            while (next != NULL) {
                _getCount(bucket, (*ht)->bucketSize, &count);

                /*Get value for each slot of bucket to destroy it.*/
                for (slot = 0; slot < count; slot++) {
                    _getValue(bucket, slot, &v);
                    (*ht)->destroy(v);
                    //printf("[%p] ", v);
                }

                /*Get next pointer to determine if this bucket has an overflow bucket.*/
                _getNext(bucket, (*ht)->bucketSize, &next);

                free(bucket);

                if (next != NULL) {
                    bucket = next;
                }
            };

            //printf("\n");

        }
    }
    free((*ht)->table);
    free(*ht);
}
