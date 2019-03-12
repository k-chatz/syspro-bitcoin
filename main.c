#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <zconf.h>
#include "hash.h"
#include "list.h"
#include "wallet.h"
#include "bitcoin.h"
#include "transaction.h"

#define BUFFER_SIZE 256

typedef void *pointer;

void wrongOptionValue(char *opt, char *val) {
    fprintf(stderr, "Wrong value [%s] for option '%s'\n", val, opt);
    exit(EXIT_FAILURE);
}

void readOptions(
        int argc,
        char **argv,
        char **a,                   /*bitcoinBalancesFile*/
        char **t,                   /*transactionsFile*/
        unsigned long int *v,       /*bitcoinValue*/
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

void init(hashtable *wallets, hashtable *bitcoins, char *a, unsigned long int v, unsigned long int b,
          const unsigned long int h1, const unsigned long int h2) {
    FILE *fp = NULL;
    Wallet wallet = NULL;
    char buf[BUFFER_SIZE], *token = NULL;
    unsigned long int bid = 0;
    bitcoin bc = NULL;
    ht_bitcoin_params htBitCoinParams;

    /*Open bitcoinBalancesFile*/
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

        /*Initialize bitcoins hashtable*/
        HT_Init(
                bitcoins,
                h1 > h2 ? h1 : h2,
                b,
                (pointer (*)(pointer)) bcCreate,
                (int (*)(pointer, pointer)) bcCompare,
                (unsigned long (*)(pointer, unsigned long int)) bitcoinHash,
                (unsigned long (*)(pointer)) bcDestroy
        );

        /*Read bitcoinBalancesFile*/
        while (fgets(buf, BUFFER_SIZE, fp) != NULL) {
            token = strtok(buf, "\n");
            token = strtok(token, " ");
            if (token != NULL) {
                //printf("%s \n", token);
                /*Insert wallet, check if the insertion fails*/
                if (HT_Insert(*wallets, token, token, (void **) &wallet)) {

                    /*Read bitcoins for current wallet*/
                    do {
                        token = strtok(NULL, " ");
                        if (token != NULL) {
                            bid = (unsigned long int) strtol(token, NULL, 10);

                            /*Initialize parameters for bitcoin*/
                            htBitCoinParams.wallet = wallet;
                            htBitCoinParams.bid = bid;
                            htBitCoinParams.v = v;

                            /*Insert bitcoin into bitcoins hashtable*/
                            if (HT_Insert(*bitcoins, &bid, &htBitCoinParams, (void **) &bc)) {
                                //printf("[%lu] [%p] \n", bid, bc);

                                /*Insert user bitcoin entry in wallet's bitcoin list*/
                                if (!listInsert(wallet->bitcoins, bc)) {
                                    fprintf(stderr, "\nBitCoin [%lu] was not inserted in wallet list!\n", bid);
                                };

                            } else {
                                fprintf(stderr, "\nHT BitCoin [%p] was not inserted because is duplicate!\n", bc);
                                HT_Destroy(wallets);
                                HT_Destroy(bitcoins);
                                exit(EXIT_FAILURE);
                            }
                        }
                    } while (token != NULL);
                } else {
                    fprintf(stderr, "\nWallet [%s] was not inserted because is duplicate!\n", token);
                    destroyWallet(wallet);
                    HT_Destroy(wallets);
                    HT_Destroy(bitcoins);
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

void initTransactions(hashtable *wallets, hashtable *bitcoins, hashtable *senderHashtable, hashtable *receiverHashtable,
                      hashtable *transactionsHashtable, const unsigned long int h1, const unsigned long int h2,
                      unsigned long int b,
                      char *t, unsigned long int v) {
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
                bitcoins,
                senderHashtable,
                receiverHashtable,
                transactionsHashtable,
                "\n"
        );

        if (error) {
            fprintf(stderr, "Perform transactions function complete with errors!\n");
        }

        fclose(fp);
    } else {
        fprintf(stderr, "File '%s' doesn't exists!\n", t);
        exit(EXIT_FAILURE);
    }
}


void requestTransaction(char *buf) {
    printf("\n[%s]\n", buf);
}

void requestTransactions(char *buf) {
    /*TODO:
          requestTransactions inputFile
          requestTransactions senderWalletID receiverWalletID amount date time;
                            senderWalletID2 receiverWalletID2 amount2 date2 time2;
       */
}

void findEarnings(char *buf) {

}

void findPayments(char *buf) {

}

void walletStatus(char *buf) {

}

void bitCoinStatus(char *buf) {

}

void traceCoin(char *buf) {

}


/* Command line interface
 * Get input from user to perform various cli commands.*/
void cli() {
    int fd;
    char *rest = NULL, *token = NULL;
    size_t len = 0;
    ssize_t read = NULL;

    printf("Welcome to bitcoin transactions simulator, write 'exit' to quit from cli or use default commands.\n\n");
    putchar('>');
    while ((read = getline(&token, &len, stdin)) != EOF) {
        rest = token;
        token = strtok_r(rest, " \n", &rest);
        if (token != NULL) {
            if (strcmp(token, "requestTransaction") == 0) {
                requestTransaction(rest);
                printf("[%s]\n", token);
            } else if (strcmp(token, "requestTransactions") == 0) {
                requestTransactions(rest);
            } else if (strcmp(token, "findEarnings") == 0) {
                findEarnings(rest);
            } else if (strcmp(token, "findPayments") == 0) {
                findPayments(rest);
            } else if (strcmp(token, "walletStatus") == 0) {
                walletStatus(rest);
            } else if (strcmp(token, "bitCoinStatus") == 0) {
                bitCoinStatus(rest);
            } else if (strcmp(token, "traceCoin") == 0) {
                traceCoin(rest);
            } else if (strcmp(token, "exit") == 0) {
                fd = open("/dev/null", O_WRONLY);
                dup2(fd, 0);
                free(token);
                close(fd);
                break;
            } else {
                fprintf(stderr, "Command not found!\n");
            }
        }
        putchar('>');
        putchar(' ');
    }
}

int main(int argc, char *argv[]) {
    unsigned long int h1 = 0, h2 = 0, b = 0, v = 0;
    char *a = NULL, *t = NULL;
    hashtable wallets, bitcoins, senderHashtable, receiverHashtable, transactionsHashtable;

    /*Read argument options from command line*/
    readOptions(argc, argv, &a, &t, &v, &h1, &h2, &b);

    /*Init structures*/
    init(&wallets, &bitcoins, a, v, b, h1, h2);

    /*Initialize with some transactions*/
    initTransactions(&wallets, &bitcoins, &senderHashtable, &receiverHashtable, &transactionsHashtable, h1, h2, b, t,
                     v);

    /*Get input from user to perform various cli commands*/
    cli();

    /*Deallocate previously allocated memory.*/
    HT_Destroy(&wallets);
    HT_Destroy(&bitcoins);
    HT_Destroy(&senderHashtable);
    HT_Destroy(&receiverHashtable);
    HT_Destroy(&transactionsHashtable);
    return EXIT_SUCCESS;
}
