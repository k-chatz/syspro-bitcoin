#include <string.h>
#include <malloc.h>
#include "list.h"
#include "wallet.h"

/*Create
 * Initialize & return a new wallet*/
Wallet createWallet(char *userId) {
    Wallet wallet = malloc(sizeof(struct Wallet));
    wallet->userId = malloc(sizeof(char *) * strlen(userId) + 1);
    listCreate(&wallet->bitcoins, userId);
    strcpy(wallet->userId, userId);
    return wallet;
}

/*Callback
 * Compare wallet with userId field*/
int cmpWallet(Wallet w1, char *userId) {
    return strcmp(w1->userId,  userId);
}

/*Callback
 * Hash function for wallets hashtable*/
unsigned long int walletHash(char *key, unsigned long int capacity) {
    int i, sum = 0;
    size_t keyLength = strlen(key);
    for (i = 0; i < keyLength; i++) {
        sum += key[i];
    }
    return sum % capacity;
}

/*Callback
 * Destroy wallet*/
void destroyWallet(Wallet w) {
    free(w->userId);
    listDestroy(&w->bitcoins);
    free(w);
    //TODO: Fix this!!! doesn't work !!!

    printf(" ");
}
