#ifndef WALLET_H
#define WALLET_H

#include "list.h"

struct Wallet {
    char *userId;
    listPtr bitcoins;
};

typedef struct HT_Wallet_Params {
    unsigned long int capacity;
} ht_wallet_params;

struct Wallet *createWallet(char *userId);

/*Callback
 * Compare keys function for wallets hashtable*/
int cmpWallet(struct Wallet *w1, struct Wallet *w2);

/*Callback
 * Hash function for wallets hashtable*/
unsigned long int walletHash(char *key, ht_wallet_params *params);

/*Callback
 * Compare keys function for wallets hashtable*/
void destroyWallet(struct Wallet *w);

#endif //WALLET_H
