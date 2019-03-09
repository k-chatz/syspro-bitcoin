#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "list.h"
#include "tree.h"
#include "wallet.h"
#include "bitcoin.h"
#include "transaction.h"

#define LINE_SIZE 256

typedef void *pointer;

void wrongOptionValue(char *opt, char *val) {
    fprintf(stderr, "Wrong value [%s] for option '%s'\n", val, opt);
    exit(EXIT_FAILURE);
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

void init(hashtable *wallets, hashtable *bitCoins, char *a, unsigned long int v, unsigned long int b,
          const unsigned long int h1,
          const unsigned long int h2) {
    FILE *fp = NULL;
    struct Wallet *wallet = NULL;
    char buf[LINE_SIZE], *token = NULL;
    unsigned long int bid = 0;
    treePtr bc = NULL;

    /*Open bitCoinBalancesFile*/
    fp = fopen(a, "r");
    if (fp != NULL) {

        /*Initialize Wallets hashtable*/
        ht_wallet_params wp;
        wp.capacity = h1;
        HT_Init(
                wallets,
                wp.capacity,
                b,
                (pointer (*)(pointer)) createWallet,
                (int (*)(pointer, pointer)) cmpWallet,
                (unsigned long (*)(pointer, pointer)) walletHash,
                (unsigned long (*)(void *)) destroyWallet,
                &wp
        );

        /*Initialize BitCoins hashtable*/
        ht_bitcoin_params bp;
        bp.capacity = h2;
        HT_Init(
                bitCoins,
                bp.capacity,
                b,
                NULL,
                (int (*)(pointer, pointer)) cmpBitCoin,
                (unsigned long (*)(pointer, pointer)) bitCoinHash,
                (unsigned long (*)(void *)) destroyBitCoin,
                &bp
        );

        /*Read bitCoinBalancesFile*/
        while (fgets(buf, LINE_SIZE, fp) != NULL) {
            token = strtok(buf, "\n");
            token = strtok(token, " ");
            if (token != NULL) {

                //printf("%s \n", token);
                //wallet = createWallet(token);

                /*Insert wallet, check if the insertion fails*/
                if (HT_Insert(*wallets, token, token, (void **)&wallet)) {

                    /*Read bitcoins for current wallet*/
                    do {
                        token = strtok(NULL, " ");
                        if (token != NULL) {
                            bc = NULL;
                            bid = (unsigned long int) strtol(token, NULL, 10);

                            /*Create bitCoin & insert bitCoin into hashtable*/
                            treeCreate(&bc, bid, wallet, v);
                            //printf("%lu [%p] \n", bid, bc);

                            /*Insert bitCoin into bitCoins hashtable*/
                            if (/*HT_Insert(*bitCoins, &bid, bc)*/1) {

                                /*Insert bitCoin (pointer to tree) in wallet's bitCoin list*/
                                if (!listInsert(wallet->bitcoins, bc)) {
                                    fprintf(stderr, "\nBitCoin [%lu] was not inserted in wallet list!\n", bid);
                                    listDestroy(wallet->bitcoins);
                                    HT_Destroy(wallets);
                                    HT_Destroy(bitCoins);
                                    exit(EXIT_FAILURE);
                                };

                            } else {
                                fprintf(stderr, "\nHT BitCoin [%p] was not inserted because is duplicate!\n", bc);
                                treeDestroy(&bc);
                                HT_Destroy(wallets);
                                HT_Destroy(bitCoins);
                                exit(EXIT_FAILURE);
                            }

                        }
                    } while (token != NULL);

                    /***********************************JUST FOR TEST
                    pointer x = NULL;
                    while ((x = listNext(wallet->bitcoins)) != NULL) {
                        printf("[%p] ", x);
                    }
                    printf("\n");
                    printf(" ");
                    */
                } else {
                    fprintf(stderr, "\nWallet [%s] was not inserted because is duplicate!\n", token);
                    destroyWallet(wallet);
                    HT_Destroy(wallets);
                    HT_Destroy(bitCoins);
                    exit(EXIT_FAILURE);
                }
                //printf("\n\n");
            }
        }
    } else {
        fprintf(stderr, "\nFile '%s' doesn't exists!\n", a);
        exit(EXIT_FAILURE);
    }
}

void initTransactions(hashtable *wallets, hashtable *bitCoins, hashtable *senderHashtable, hashtable *receiverHashtable,
                      const unsigned long int h1, const unsigned long int h2, unsigned long int b, char *t,
                      unsigned long int v) {
    FILE *fp = NULL;
    struct Wallet *wallet;
    char buf[LINE_SIZE], *token = NULL;
    unsigned long int bid = 0;
    treePtr bc = NULL;

    /*Open transactionsFile*/
    fp = fopen(t, "r");
    if (fp != NULL) {

        /* Initialize hashtable for sender transactions list hashtable*/
        ht_transaction_list_params tp1;
        tp1.capacity = h2;
        HT_Init(
                senderHashtable,
                tp1.capacity,
                b,
                NULL,
                (int (*)(pointer, pointer)) cmpTransactionList,
                (unsigned long (*)(pointer, pointer)) walletHash,
                (unsigned long (*)(void *)) destroyTransactionList,
                &tp1
        );

        /* Initialize hashtable for receiver transactions list hashtable*/
        ht_transaction_list_params tp2;
        tp2.capacity = h2;
        HT_Init(
                receiverHashtable,
                tp2.capacity,
                b,
                NULL,
                (int (*)(pointer, pointer)) cmpTransactionList,
                (unsigned long (*)(pointer, pointer)) walletHash,
                (unsigned long (*)(void *)) destroyTransactionList,
                &tp2
        );

        /*Read bitCoinBalancesFile*/
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
    }
}

int main(int argc, char *argv[]) {
    unsigned long int h1 = 0, h2 = 0, b = 0, v = 0;
    char *a = NULL, *t = NULL;
    hashtable wallets, bitCoins, senderHashtable, receiverHashtable;

    /*Read argument options from command line*/
    readOptions(argc, argv, &a, &t, &v, &h1, &h2, &b);

    /*Init structures*/
    init(&wallets, &bitCoins, a, v, b, h1, h2);

    struct Wallet *w1 = HT_Get(wallets, "A");

    /*TODO:Initialize with some transactions*/
    //initTransactions(&wallets, &bitCoins, &senderHashtable, &receiverHashtable, b, h1, h2, t, v);

    HT_Destroy(&wallets);
    //HT_Destroy(&bitCoins);
    //HT_Destroy(&senderHashtable);
    //HT_Destroy(&receiverHashtable);
    return EXIT_SUCCESS;
}
