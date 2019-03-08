#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "list.h"
#include "tree.h"
#include "wallet.h"

#define LINE_SIZE 256

typedef void *pointer;

typedef struct Params {
    unsigned long int capacity;
} params_t;

struct Wallet *createWallet(char *userId);

void wrongOptionValue(char *opt, char *val);

struct Wallet *createWallet(char *userId) {
    struct Wallet *wallet = NULL;
    wallet = malloc(sizeof(struct Wallet));
    wallet->userId = malloc(sizeof(char *) * strlen(userId) + 1);
    listCreate(&wallet->bitcoins);
    strcpy(wallet->userId, userId);
    return wallet;
}

void destroyWallet(struct Wallet *wallet) {
    free(wallet->userId);
    //TODO: free(wallet->bitcoins);
    free(wallet);
}

void readOptions(int argc, char **argv, char **a, char **t, unsigned long int *v,
                 unsigned long int *h1, unsigned long int *h2, unsigned long int *b);

int cmpWallet(struct Wallet *w1, struct Wallet *w2) {
    return strcmp(w1->userId, w2->userId);
}

//TODO: Struct Tree * bitcoin;
int cmpBitCoin(const long int *bid1, const long int *bid2) {
    return *bid1 != *bid2;
}

unsigned long int walletHash(char *key, params_t *params) {
    int i, sum = 0;
    size_t keyLength = strlen(key);
    for (i = 0; i < keyLength; i++) {
        sum += key[i];
    }
    return sum % params->capacity;
}

unsigned long int bitCoinHash(const long int *bid1, params_t *params) {
    return *bid1 % params->capacity;
}

int main(int argc, char *argv[]) {
    int i;
    unsigned long int h1 = 0, h2 = 0, b = 0, bid = 0, v = 0;
    char buf[LINE_SIZE], *opt, *optVal, *a = NULL, *t = NULL, *token = NULL;
    hashtable wallets, bitCoins, senderHashtable, recieverHashtable;
    treePtr bc = NULL;

    FILE *fp = NULL;
    struct Wallet *wallet;

    /*Read argument options from command line*/
    readOptions(argc, argv, &a, &t, &v, &h1, &h2, &b);

    /*Create hashtable for Wallets*/
    params_t wp;
    wp.capacity = h1;
    HT_Create(&wallets, wp.capacity, b, (int (*)(pointer, pointer)) cmpWallet,
              (unsigned long (*)(pointer, pointer)) walletHash, &wp);

    /*Create hashtable for Bitcoins*/
    params_t bp;
    bp.capacity = h2;
    HT_Create(&bitCoins, bp.capacity, b, (int (*)(pointer, pointer)) cmpBitCoin,
              (unsigned long (*)(pointer, pointer)) bitCoinHash, &bp);

    /*Open & read bitCoinBalancesFile*/
    fp = fopen(a, "r");
    if (fp != NULL) {
        while (fgets(buf, LINE_SIZE, fp) != NULL) {
            token = strtok(buf, "\n");
            token = strtok(token, " ");
            if (token != NULL) {
                printf("%s \n", token);
                wallet = createWallet(token);

                /*Insert wallet, check if the insertion fails*/
                if (HT_Insert(wallets, wallet->userId, wallet)) {

                    /*Read bitcoins for current wallet*/
                    do {
                        token = strtok(NULL, " ");
                        if (token != NULL) {
                            bc = NULL;
                            bid = (unsigned long) strtol(token, NULL, 10);

                            /*Create bitcoin & insert bitcoin into hashtable*/
                            treeCreate(&bc, bid, wallet, v);
                            printf("%lu [%p] \n", bid, bc);

                            /*Insert bitcoin into bitcoins hashtable*/
                            if (HT_Insert(bitCoins, &bid, bc)) {

                                /*Insert bitcoin (pointer to tree) in wallet's bitcoin list*/
                                if (!listInsert(wallet->bitcoins, bc)) {
                                    fprintf(stderr, "\nBitCoin [%lu] was not inserted in wallet list!\n", bid);
                                    listDestroy(wallet->bitcoins);
                                    exit(EXIT_FAILURE);
                                };
                            } else {
                                fprintf(stderr, "\nHT BitCoin [%p] was not inserted because is duplicate!\n", bc);
                                treeDestroy(&bc);
                                exit(EXIT_FAILURE);
                            }
                        }
                    } while (token != NULL);








                    /*******/
                    pointer x = NULL;
                    while ((x = listNext(wallet->bitcoins)) != NULL) {



                        printf("[%p] ", x);
                    }































                } else {
                    fprintf(stderr, "\nWallet [%s] was not inserted because is duplicate!\n", token);
                    destroyWallet(wallet);
                    exit(EXIT_FAILURE);
                }
                printf("\n\n");
            }
        }
    } else {
        fprintf(stderr, "\nFile '%s' doesn't exists!\n", a);
        exit(EXIT_FAILURE);
    }

    /*Open & read transactionsFile*/
/*    fp = fopen(t, "r");
    if (fp != NULL) {

        while (fgets(buf, LINE_SIZE, fp) != NULL) {

            token = strtok(buf, " ");

            printf("%s ", token);

            do {
                token = strtok(NULL, " ");
                if (token != NULL)
                    printf("%s ", token);
            } while (token != NULL);

            printf("\n");
        }

    } else {
        fprintf(stderr, "File '%s' doesn't exists!\n", t);
        exit(1);
    }*/

    return EXIT_SUCCESS;
}

void readOptions(int argc, char **argv, char **a, char **t, unsigned long int *v,
                 unsigned long int *h1, unsigned long int *h2, unsigned long int *b) {
    int i;
    char *opt, *optVal;
    for (i = 1; i < argc; ++i) {
        opt = argv[i];
        optVal = argv[i + 1];
        if (strcmp(opt, "-a") == 0) {
            if (optVal != NULL && optVal[0] != '-') { /*bitCoinBalancesFile*/
                *a = optVal;
            } else {
                wrongOptionValue(opt, optVal);
            }
        } else if (strcmp(opt, "-t") == 0) { /*transactionsFile*/
            if (optVal != NULL && optVal[0] != '-') {
                *t = optVal;
            } else {
                wrongOptionValue(opt, optVal);
            }
        } else if (strcmp(opt, "-v") == 0) { /*bitCoinValue*/
            if (optVal != NULL && optVal[0] != '-') {
                //*v = atoi(optVal);
                *v = (unsigned long int) strtol(optVal, NULL, 10);
            } else {
                wrongOptionValue(opt, optVal);
            }
        } else if (strcmp(opt, "-h1") == 0) { /*senderHashtableNumOfEntries*/
            if (optVal != NULL && optVal[0] != '-') {
                *h1 = (unsigned long) strtol(optVal, NULL, 10);
            } else {
                wrongOptionValue(opt, optVal);
            }
        } else if (strcmp(opt, "-h2") == 0) { /*receiverHashtableNumOfEntries*/
            if (optVal != NULL && optVal[0] != '-') {
                *h2 = (unsigned long) strtol(optVal, NULL, 10);
            } else {
                wrongOptionValue(opt, optVal);
            }
        } else if (strcmp(opt, "-b") == 0) { /*bucketSize*/
            if (optVal != NULL && optVal[0] != '-') {
                *b = (unsigned long) strtol(optVal, NULL, 10);
            } else {
                wrongOptionValue(opt, optVal);
            }
        }
    }
}

void wrongOptionValue(char *opt, char *val) {
    fprintf(stderr, "Wrong value [%s] for option '%s'\n", val, opt);
    exit(EXIT_FAILURE);
}
