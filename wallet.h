#ifndef WALLET_H
#define WALLET_H

#include "list.h"

typedef struct Wallet {
    char *userId;
    listPtr bitcoins;
} *Wallet;

/* @Callback
 * Initialize & return a new wallet*/
Wallet createWallet(char *userId);

/* @Callback
 * Compare wallet with userId field*/
int cmpWallet(Wallet w1, char *userId);

/* @Callback
 * Hash function for wallets hashtable*/
unsigned long int walletHash(char *key, unsigned long int capacity);

/* @Callback
 * Destroy wallet*/
void destroyWallet(Wallet w);

#endif //WALLET_H
