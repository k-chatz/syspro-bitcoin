#ifndef WALLET_H
#define WALLET_H

#include "list.h"

struct Wallet {
    char *userId;
    listPtr bitcoins;
};

/*Create
 * Initialize & return a new wallet*/
struct Wallet *createWallet(char *userId);

/*Callback
 * Compare wallet with userId field*/
int cmpWallet(struct Wallet *w1, char *userId);

/*Callback
 * Hash function for wallets hashtable*/
unsigned long int walletHash(char *key, unsigned long int capacity);

/*Callback
 * Destroy wallet*/
void destroyWallet(struct Wallet *w);

#endif //WALLET_H
