#include <string.h>
#include <malloc.h>
#include "list.h"
#include "wallet.h"

/*Create
 * Initialize & return a new wallet*/
struct Wallet *createWallet(char *userId) {
    struct Wallet *wallet = NULL;
    wallet = malloc(sizeof(struct Wallet));
    wallet->userId = malloc(sizeof(char *) * strlen(userId) + 1);
    listCreate(&wallet->bitcoins);
    strcpy(wallet->userId, userId);
    return wallet;
}

/*Callback
 * Compare keys function for wallets hashtable*/
int cmpWallet(struct Wallet *w1, struct Wallet *w2) {
    return strcmp(w1->userId, w2->userId);
}

/*Callback
 * Hash function for wallets hashtable*/
unsigned long int walletHash(char *key, ht_wallet_params *params) {
    int i, sum = 0;
    size_t keyLength = strlen(key);
    for (i = 0; i < keyLength; i++) {
        sum += key[i];
    }
    return sum % params->capacity;
}

/*Callback
 * Compare keys function for wallets hashtable*/
void destroyWallet(struct Wallet *w) {
    free(w->userId);
    listDestroy(w->bitcoins);
    free(w);
}
