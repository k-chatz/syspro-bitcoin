#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

#include "list.h"
#include "wallet.h"

#define LINE_SIZE 256

void wrongOptionValue(char *opt, char *val);

void readOptions(int argc, char **argv, char **a, char **t, int *v, int *h1, int *h2, int *b);

int cmpWalletId(void *s1, void *s2) {
    return strcmp((char *) s1, (char *) s2);
}

/*
unsigned long int hash(void *element, void *params) {
    int ascii;
    params_t *parm = (params_t *) params;
    char *strkey = ((vertex_t *) element)->id;
    unsigned long int hv = parm->seed;
    while ((ascii = *(strkey++)))
        hv = ((hv << 5) + hv) ^ ascii;

    return (((hv * parm->a) + parm->b) % parm->p) % parm->m;
}

*/

typedef struct Params {
    unsigned long int capacity;
} params_t;

unsigned long int hash(char *key, params_t *params) {
    int i, sum = 0;
    size_t keyLength = strlen(key);
    for (i = 0; i < keyLength; i++) {
        sum += key[i];
    }
    return sum % params->capacity;
}

int main(int argc, char *argv[]) {
    int i, b = 0, v = 0, h1 = 0, h2 = 0;
    char buf[LINE_SIZE], *opt, *optVal, *a = NULL, *t = NULL, *token = NULL, *bitCoinId;
    hashtablePtr senderHashTable;
    FILE *fp = NULL;
    struct Wallet *wallet;

    /*Read argument options from command line*/
    readOptions(argc, argv, &a, &t, &v, &h1, &h2, &b);

    params_t p1;
    p1.capacity = (unsigned long) h1;
    HT_Create(&senderHashTable, p1.capacity, b, cmpWalletId, (unsigned long (*)(void *, void *)) hash, &p1);


//    printf("a: %s\n", a);
//    printf("t: %s\n", t);
//    printf("v: %d\n", v);
//    printf("h1: %d\n", h1);
//    printf("h2: %d\n", h2);
//    printf("b: %d\n\n", b);


    /*Open & read bitCoinBalancesFile*/
    fp = fopen(a, "r");
    if (fp != NULL) {
        while (fgets(buf, LINE_SIZE, fp) != NULL) {
            token = strtok(buf, "\n");
            token = strtok(token, " ");
            if (token != NULL) {

                printf("%s ", token);

                //createWallet(token, &wallet);

                wallet = malloc(sizeof(struct Wallet));
                wallet->userId = malloc(sizeof(token + 1));
                listCreate(&wallet->bitcoins);
                strcpy(wallet->userId, token);


                printf("[%lu]", HT_Insert(senderHashTable, wallet->userId, wallet));
                printf("\n");

                //TODO eisagogi tou wallet sto hash table, elegxos an isixthi sosta
                if (1) {
                    printf("\n");

                    do {
                        token = strtok(NULL, " ");
                        if (token != NULL) {

                            bitCoinId = malloc(sizeof(token + 1));
                            strcpy(bitCoinId, token);

                            //listInsert(wallet->bitcoins, bitCoinId);
                            //TODO: eisagogi tou bitcoin sto hash table
                            // tha ginete kai elegxos gia diplotipa opote
                            // kaliptete kai i lathos periptosi na exei kapoios
                            // to idio bitcoin parapano fores h to idio bitcoin
                            // na briskete se allon.
                            //printf("%s ", token);
                        }
                    } while (token != NULL);
                    /*           char *x ;//= listGetFirstData(wallet->bitcoins);
                                 while (x = listNext(wallet->bitcoins) != NULL) {
                                     printf("[%s]\n", x);
                                 }*/

                    printf("\n");

                }

            }
        }

    } else {
        fprintf(stderr, "File '%s' doesn't exists!\n", a);
        exit(1);
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

void readOptions(int argc, char **argv, char **a, char **t, int *v, int *h1, int *h2, int *b) {
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
                *v = atoi(optVal);
            } else {
                wrongOptionValue(opt, optVal);
            }
        } else if (strcmp(opt, "-h1") == 0) { /*senderHashtableNumOfEntries*/
            if (optVal != NULL && optVal[0] != '-') {
                *h1 = atoi(optVal);
            } else {
                wrongOptionValue(opt, optVal);
            }
        } else if (strcmp(opt, "-h2") == 0) { /*receiverHashtableNumOfEntries*/
            if (optVal != NULL && optVal[0] != '-') {
                *h2 = atoi(optVal);
            } else {
                wrongOptionValue(opt, optVal);
            }
        } else if (strcmp(opt, "-b") == 0) { /*bucketSize*/
            if (optVal != NULL && optVal[0] != '-') {
                *b = atoi(optVal);
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
