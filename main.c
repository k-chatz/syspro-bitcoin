#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "list.h"
#include "tree.h"
#include "wallet.h"
#include "bitcoin.h"

#define LINE_SIZE 256

typedef void *pointer;

void wrongOptionValue(char *opt, char *val);

void readOptions(int argc, char **argv, char **a, char **t, unsigned long int *v,
                 unsigned long int *h1, unsigned long int *h2, unsigned long int *b);

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
    ht_wallet_params wp;
    wp.capacity = h1;
    HT_Create(&wallets, wp.capacity, b, (int (*)(pointer, pointer)) cmpWallet,
              (unsigned long (*)(pointer, pointer)) walletHash, (unsigned long (*)(void *)) destroyWallet, &wp);

    /*Create hashtable for BitCoins*/
    ht_bitcoin_params bp;
    bp.capacity = h2;
    HT_Create(&bitCoins, bp.capacity, b, (int (*)(pointer, pointer)) cmpBitCoin,
              (unsigned long (*)(pointer, pointer)) bitCoinHash, (unsigned long (*)(void *)) destroyBitCoin, &bp);

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
                            bid = (unsigned long int) strtol(token, NULL, 10);

                            /*Create bitCoin & insert bitCoin into hashtable*/
                            treeCreate(&bc, bid, wallet, v);
                            printf("%lu [%p] \n", bid, bc);

                            /*Insert bitCoin into bitCoins hashtable*/
                            if (HT_Insert(bitCoins, &bid, bc)) {

                                /*Insert bitCoin (pointer to tree) in wallet's bitCoin list*/
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

                    /***********************************JUST FOR TEST*/
                    pointer x = NULL;
                    while ((x = listNext(wallet->bitcoins)) != NULL) {
                        printf("[%p] ", x);
                    }
                    /*************************************************/

                    printf("\n");
                    printf(" ");
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

    HT_Destroy(&wallets);
    HT_Destroy(&bitCoins);
    //HT_Destroy(senderHashtable);
    //HT_Destroy(recieverHashtable);

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
            if (optVal != NULL && optVal[0] != '-') {               /*bitCoinBalancesFile*/
                *a = optVal;
            } else {
                wrongOptionValue(opt, optVal);
            }
        } else if (strcmp(opt, "-t") == 0) {                        /*transactionsFile*/
            if (optVal != NULL && optVal[0] != '-') {
                *t = optVal;
            } else {
                wrongOptionValue(opt, optVal);
            }
        } else if (strcmp(opt, "-v") == 0) {                        /*bitCoinValue*/
            if (optVal != NULL && optVal[0] != '-') {
                //*v = atoi(optVal);
                *v = (unsigned long int) strtol(optVal, NULL, 10);
            } else {
                wrongOptionValue(opt, optVal);
            }
        } else if (strcmp(opt, "-h1") == 0) {                       /*senderHashtableNumOfEntries*/
            if (optVal != NULL && optVal[0] != '-') {
                *h1 = (unsigned long) strtol(optVal, NULL, 10);
            } else {
                wrongOptionValue(opt, optVal);
            }
        } else if (strcmp(opt, "-h2") == 0) {                       /*receiverHashtableNumOfEntries*/
            if (optVal != NULL && optVal[0] != '-') {
                *h2 = (unsigned long) strtol(optVal, NULL, 10);
            } else {
                wrongOptionValue(opt, optVal);
            }
        } else if (strcmp(opt, "-b") == 0) {                        /*bucketSize*/
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
