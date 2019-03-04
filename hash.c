#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "hash.h"

struct hashtable {
    int bucketSize;
    void **table;
    int size;
};

int hash(int size, char *key) {
    int i, sum = 0;
    size_t keyLength = strlen(key);
    for (i = 0; i < keyLength; i++) {
        sum += key[i];
    }
    return sum % size;
}

int HT_Create(hashtablePtr *ht, int size, int bucketSize) {
    *ht = (hashtablePtr) malloc(sizeof(struct hashtable));
    if ((*ht) != NULL) {
        (*ht)->bucketSize = bucketSize;
        (*ht)->size = size;
        (*ht)->table = malloc(size * sizeof(void *));
    }
   return 0;
}

void HT_Destroy(hashtablePtr ht) {
    free(ht->table);
}

int HT_Insert(hashtablePtr ht, struct Wallet *wallet) {
    int position = hash(ht->size, wallet->userId);
    return position;
}

void HT_Delete(hashtablePtr ht, unsigned int slot, void *ht_node) {

}
