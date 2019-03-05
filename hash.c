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

void getRecord(void *bucket, const int offset, struct Wallet *wallet) {
    memcpy(wallet, bucket + offset * sizeof(struct Wallet *), sizeof(struct Wallet *));
}

void setRecord(void *bucket, const int offset, const struct Wallet *wallet) {
    memcpy(bucket + offset * sizeof(struct Wallet *), wallet, sizeof(struct Wallet *));
}

struct hashtable {
    int bucketSize;
    void **table;
    int capacity;
};

int hash(int size, char *key) {
    int i, sum = 0;
    size_t keyLength = strlen(key);
    for (i = 0; i < keyLength; i++) {
        sum += key[i];
    }
    return sum % size;
}

int HT_Create(hashtablePtr *ht, int capacity, int bucketSize) {
    int i;
    *ht = (hashtablePtr) malloc(sizeof(struct hashtable));
    if ((*ht) != NULL) {
        (*ht)->bucketSize = bucketSize;
        (*ht)->capacity = capacity;
        (*ht)->table = malloc(sizeof(void *) * capacity);
        for (i = 0; i < capacity; i++) {
            (*ht)->table[i] = NULL;
        }
    }
    return 0;
}

void HT_Destroy(hashtablePtr ht) {
    free(ht->table);
}

int HT_Insert(hashtablePtr ht, struct Wallet *wallet) {
    int count = 0, position = 0;
    void *bucket = NULL;
    position = hash(ht->capacity, wallet->userId);
    bucket = ht->table[position];
    if (bucket == NULL) {
        bucket = malloc((size_t) ht->bucketSize);
        setRecord(bucket, 0, wallet);
        setCount(bucket, ht->bucketSize, 1);
        setNext(bucket, ht->bucketSize, NULL);
        ht->table[position] = bucket;
    } else {
        printf("BUCKET IS NOT NEW! ");
        getCount(bucket, ht->bucketSize, &count);
        printf("count = %d\n", count);
    }
    return position;
}

void HT_Delete(hashtablePtr ht, unsigned int slot, void *ht_node) {

}
