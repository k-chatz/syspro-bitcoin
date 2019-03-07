#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "hash.h"

typedef void *pointer;

struct hashtable {
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
    memcpy(value, bucket + offset, sizeof(pointer));
}

void _setValue(pointer bucket, unsigned int offset, pointer value) {
    memcpy(bucket + offset, &value, sizeof(pointer));
}

bool bucketIsFull(pointer bucket, unsigned int count) {

   // bucket + bucketSize - sizeof(unsigned int) - sizeof(pointer)

    return 0;
}


/***Public functions***/

bool HT_Create(hashtablePtr *ht, unsigned long capacity, unsigned int bucketSize, int (*cmp)(pointer, pointer),
               unsigned long (*hash)(pointer, pointer), pointer params) {
    int i;
    *ht = (hashtablePtr) malloc(sizeof(struct hashtable));
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

void HT_Destroy(hashtablePtr ht) {
    free(ht->table);
}

unsigned long int HT_Insert(hashtablePtr ht, char *key, pointer v) {
    unsigned int count = 0, i;
    unsigned long int position = 0;
    pointer bucket = NULL, value = NULL, next = NULL;
    position = ht->hash(key, ht->params);
    printf("[%lu] ", position);
    bucket = ht->table[position];

    /* Check if current bucket exists */
    if (bucket == NULL) {
        bucket = malloc((size_t) ht->bucketSize);
        _setValue(bucket, 0, v);
        _setCount(bucket, ht->bucketSize, 1);
        _setNext(bucket, ht->bucketSize, NULL);
        ht->table[position] = bucket;
    } else {
        printf(":::Collision::: ");

        _getCount(bucket, ht->bucketSize, &count);

        //TODO: CHECK IF CURRENT BUCKET IS FULL

        pointer kati = NULL;
        // Check values for each bucket
        do {

            for (i = 0; i < count; i++) {
                _getValue(bucket, i, &value);

                if (!ht->cmp(value, v)) {
                    return HT_ERROR;
                }

                kati = bucket + i + sizeof(pointer);
            }

            if (bucketIsFull(bucket, count)) {

            }

            _getNext(bucket, ht->bucketSize, &next);
            bucket = next;
        } while (bucket != NULL);

        //_getValue(bucket, i, &value);
    }


    return HT_OK;
}


void HT_Delete(hashtablePtr ht, unsigned int slot, pointer ht_node) {

}
