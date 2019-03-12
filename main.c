#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "hash.h"
#include "list.h"
#include "wallet.h"
#include "bitcoin.h"
#include "transaction.h"

#define BUFFER_SIZE 256

typedef void *pointer;

void cli();

void wrongOptionValue(char *opt, char *val) {
    fprintf(stderr, "Wrong value [%s] for option '%s'\n", val, opt);
    exit(EXIT_FAILURE);
}

void readOptions(
        int argc,
        char **argv,
        char **a,                   /*bitCoinBalancesFile*/
        char **t,                   /*transactionsFile*/
        unsigned long int *v,       /*bitCoinValue*/
        unsigned long int *h1,      /*senderHashtableNumOfEntries*/
        unsigned long int *h2,      /*receiverHashtableNumOfEntries*/
        unsigned long int *b        /*bucketSize*/
) {
    int i;
    char *opt, *optVal;
    for (i = 1; i < argc; ++i) {
        opt = argv[i];
        optVal = argv[i + 1];
        if (strcmp(opt, "-a") == 0) {
            if (optVal != NULL && optVal[0] != '-') {
                *a = optVal;
            } else {
                wrongOptionValue(opt, optVal);
            }
        } else if (strcmp(opt, "-t") == 0) {
            if (optVal != NULL && optVal[0] != '-') {
                *t = optVal;
            } else {
                wrongOptionValue(opt, optVal);
            }
        } else if (strcmp(opt, "-v") == 0) {
            if (optVal != NULL && optVal[0] != '-') {
                //*v = atoi(optVal);
                *v = (unsigned long int) strtol(optVal, NULL, 10);
            } else {
                wrongOptionValue(opt, optVal);
            }
        } else if (strcmp(opt, "-h1") == 0) {
            if (optVal != NULL && optVal[0] != '-') {
                *h1 = (unsigned long) strtol(optVal, NULL, 10);
            } else {
                wrongOptionValue(opt, optVal);
            }
        } else if (strcmp(opt, "-h2") == 0) {
            if (optVal != NULL && optVal[0] != '-') {
                *h2 = (unsigned long) strtol(optVal, NULL, 10);
            } else {
                wrongOptionValue(opt, optVal);
            }
        } else if (strcmp(opt, "-b") == 0) {
            if (optVal != NULL && optVal[0] != '-') {
                *b = (unsigned long) strtol(optVal, NULL, 10);
            } else {
                wrongOptionValue(opt, optVal);
            }
        }
    }
}

void init(hashtable *wallets,
          hashtable *bitCoins,
          char *a,
          unsigned long int v,
          unsigned long int b,
          const unsigned long int h1,
          const unsigned long int h2
) {
    FILE *fp = NULL;
    Wallet wallet = NULL;
    char buf[BUFFER_SIZE], *token = NULL;
    unsigned long int bid = 0;
    bitCoin bc = NULL;
    ht_bitCoin_params htBitCoinParams;

    /*Open bitCoinBalancesFile*/
    fp = fopen(a, "r");
    if (fp != NULL) {

        /*Initialize Wallets hashtable*/
        HT_Init(
                wallets,
                h1 > h2 ? h1 : h2,
                b,
                (pointer (*)(pointer)) createWallet,
                (int (*)(pointer, pointer)) cmpWallet,
                (unsigned long (*)(pointer, unsigned long int)) walletHash,
                (unsigned long (*)(pointer)) destroyWallet
        );

        /*Initialize BitCoins hashtable*/
        HT_Init(
                bitCoins,
                h1 > h2 ? h1 : h2,
                b,
                (pointer (*)(pointer)) bcCreate,
                (int (*)(pointer, pointer)) bcCompare,
                (unsigned long (*)(pointer, unsigned long int)) bitCoinHash,
                (unsigned long (*)(pointer)) bcDestroy
        );

        /*Read bitCoinBalancesFile*/
        while (fgets(buf, BUFFER_SIZE, fp) != NULL) {
            token = strtok(buf, "\n");
            token = strtok(token, " ");
            if (token != NULL) {
                //printf("%s \n", token);
                /*Insert wallet, check if the insertion fails*/
                if (HT_Insert(*wallets, token, token, (void **) &wallet)) {

                    /*Read BitCoins for current wallet*/
                    do {
                        token = strtok(NULL, " ");
                        if (token != NULL) {
                            bid = (unsigned long int) strtol(token, NULL, 10);

                            /*Initialize parameters for bitCoin*/
                            htBitCoinParams.wallet = wallet;
                            htBitCoinParams.bid = bid;
                            htBitCoinParams.v = v;

                            /*Insert bitCoin into bitCoins hashtable*/
                            if (HT_Insert(*bitCoins, &bid, &htBitCoinParams, (void **) &bc)) {
                                //printf("[%lu] [%p] \n", bid, bc);

                                /*Insert bitCoin in wallet's bitCoin list*/
                                if (!listInsert(wallet->bitcoins, bc)) {
                                    fprintf(stderr, "\nBitCoin [%lu] was not inserted in wallet list!\n", bid);
                                    listDestroy(&wallet->bitcoins);
                                    HT_Destroy(wallets);
                                    HT_Destroy(bitCoins);
                                    exit(EXIT_FAILURE);
                                };
                            } else {
                                fprintf(stderr, "\nHT BitCoin [%p] was not inserted because is duplicate!\n", bc);
                                HT_Destroy(wallets);
                                HT_Destroy(bitCoins);
                                exit(EXIT_FAILURE);
                            }
                        }
                    } while (token != NULL);
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
        fclose(fp);
    } else {
        fprintf(stderr, "\nFile '%s' doesn't exists!\n", a);
        exit(EXIT_FAILURE);
    }
}

void initTransactions(
        hashtable *wallets,
        hashtable *bitCoins,
        hashtable *senderHashtable,
        hashtable *receiverHashtable,
        hashtable *transactionsHashtable,
        const unsigned long int h1,
        const unsigned long int h2,
        unsigned long int b,
        char *t,
        unsigned long int v
) {
    FILE *fp = NULL;
    bool error = false;

    /*Open transactionsFile*/
    fp = fopen(t, "r");
    if (fp != NULL) {

        /* Initialize hashtable for sender transactions list hashtable*/
        HT_Init(
                senderHashtable,
                h1,
                b,
                (pointer (*)(pointer)) createTransactionList,
                (int (*)(pointer, pointer)) cmpTransactionList,
                (unsigned long (*)(pointer, unsigned long int)) walletHash,
                (unsigned long (*)(pointer)) destroyTransactionList
        );

        /* Initialize hashtable for receiver transactions list hashtable*/
        HT_Init(
                receiverHashtable,
                h2,
                b,
                (pointer (*)(pointer)) createTransactionList,
                (int (*)(pointer, pointer)) cmpTransactionList,
                (unsigned long (*)(pointer, unsigned long int)) walletHash,
                (unsigned long (*)(pointer)) destroyTransactionList
        );

        /* Initialize hashtable for transactions hashtable*/
        HT_Init(
                transactionsHashtable,
                h1 > h2 ? h1 : h2,
                b,
                (pointer (*)(pointer)) createTransaction,
                (int (*)(pointer, pointer)) cmpTransaction,
                (unsigned long (*)(pointer, unsigned long int)) transactionHash,
                (unsigned long (*)(pointer)) destroyTransaction
        );

        error = performTransactions(
                fp,
                wallets,
                bitCoins,
                senderHashtable,
                receiverHashtable,
                transactionsHashtable,
                "\n"
        );

        if (error) {
            fprintf(stderr, "Perform transactions function complete with errors!\n");
            exit(EXIT_FAILURE);
        }

        fclose(fp);
    } else {
        fprintf(stderr, "File '%s' doesn't exists!\n", t);
        exit(EXIT_FAILURE);
    }
}

void cli() {
// TODO: Get input from user to perform various cli commands (switch case)
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, stdin)) != EOF) {
        printf("[%s]", line);
        putchar('>');
        putchar(' ');
    }

    free(line);

//    char line[256];
//    char *pos;
//    while (1) {
//        fgets(line, sizeof(line), stdin);
//
//        // deleting the newline captured by fgets
//        if ((pos = strchr(line, '\n')) != NULL)
//            *pos = '\0';
//
//        putchar('>');
//
//    }

/*    while ((read = getdelim(&s, &len, ';', stdin)) != EOF) {
        if (s[strlen(s) - 1] == ';')
            s[strlen(s) - 1] = '\0';
        s[0] = '/';
        printf("\n[%s]\n", s);
    }*/

//    while (getline(&cmd, &size, stdin) != EOF) {
//        cmd = strtok(cmd, "\n");
//        printf("\\033[<1>C");
//        printf("\n[%s]\n>", cmd);
//
//    }

    //scanf("%[^\n]%*c", buf);
    //printf("[%s]\n", buf);

    //while (fgets(buf, BUFFER_SIZE, stdin) != NULL) {
    //cmd = strtok(buf, "\n");
    //printf("[%s]", cmd);
    //}
}

int main(int argc, char *argv[]) {
    unsigned long int h1 = 0, h2 = 0, b = 0, v = 0;
    char *a = NULL, *t = NULL;
    hashtable wallets, bitCoins, senderHashtable, receiverHashtable, transactionsHashtable;

    /*Read argument options from command line*/
    readOptions(argc, argv, &a, &t, &v, &h1, &h2, &b);

    /*Init structures*/
    init(&wallets, &bitCoins, a, v, b, h1, h2);

    /*Initialize with some transactions*/
    initTransactions(&wallets, &bitCoins, &senderHashtable, &receiverHashtable, &transactionsHashtable, h1, h2, b, t,
                     v);

    /*Get input from user to perform various cli commands*/
    cli();

    HT_Destroy(&wallets);
    HT_Destroy(&bitCoins);
    HT_Destroy(&senderHashtable);
    HT_Destroy(&receiverHashtable);
    HT_Destroy(&transactionsHashtable);
    return EXIT_SUCCESS;
}
