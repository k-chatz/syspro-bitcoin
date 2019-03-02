#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

#include "list.h"
#include "wallet.h"

#define LINE_SIZE 256

void wrongOptionValue(char *opt, char *val);

void readOptions(int argc, char **argv, char **a, char **t, int *v, int *h1, int *h2, int *b);

int main(int argc, char *argv[]) {
    int i, b = 0, v = 0, h1 = 0, h2 = 0;
    char buf[LINE_SIZE], *opt, *optVal, *a = NULL, *t = NULL, *token = NULL;
    FILE *fp_a = NULL, *fp_t = NULL;
    struct Wallet *wallet;
    /*Read argument options from command line*/
    readOptions(argc, argv, &a, &t, &v, &h1, &h2, &b);

    printf("a: %s\n", a);
    printf("t: %s\n", t);
    printf("v: %d\n", v);
    printf("h1: %d\n", h1);
    printf("h2: %d\n", h2);
    printf("b: %d\n\n", b);

    /*Open & read bitCoinBalancesFile*/
    fp_a = fopen(a, "r");
    if (fp_a != NULL) {
        while (fgets(buf, LINE_SIZE, fp_a) != NULL) {
            token = strtok(buf, "\n");
            token = strtok(token, " ");
            if (token != NULL) {

                printf("%s ", token);

                wallet = malloc(sizeof(struct Wallet));
                wallet->userId = malloc(sizeof(token + 1));
                strcpy(wallet->userId, token);

                printf("\n");

                do {
                    token = strtok(NULL, " ");

                    //if (token != NULL)
                    //printf("%s ", token);

                } while (token != NULL);

                printf("\n");
            }
        }

    } else {
        fprintf(stderr, "File '%s' doesn't exists!\n", a);
        exit(1);
    }


    return EXIT_SUCCESS;


    /*Open & read transactionsFile*/
    fp_t = fopen(t, "r");
    if (fp_t != NULL) {

        while (fgets(buf, LINE_SIZE, fp_t) != NULL) {

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
