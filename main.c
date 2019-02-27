#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_SIZE 256

void wrongOptionValue(char *opt, char *val);

void readOptions(int argc, char **argv, char **a, char **t, int *v, int *h1, int *h2, int *b);

int main(int argc, char *argv[]) {
    int i, b = 0, v = 0, h1 = 0, h2 = 0;
    char  buf[LINE_SIZE], *opt, *optVal, *a = NULL, *t = NULL, *token = NULL;

    /*Read argument options from command line*/
    readOptions(argc, argv, &a, &t, &v, &h1, &h2, &b);

    printf("a: %s\n", a);
    printf("t: %s\n", t);
    printf("v: %d\n", v);
    printf("h1: %d\n", h1);
    printf("h2: %d\n", h2);
    printf("b: %d\n", b);

    FILE *fp = fopen(a, "r");
    if (fp != NULL) {

/*        while (fgets(buf, LINE_SIZE, fp) != NULL) {
            token = strtok(buf + 1, ",");

            token = strtok(NULL, ",");

            token++;
            token[strlen(token) - 1] = 0;
            strncpy(primaryRecord.name, token, sizeof(primaryRecord.name));

            token = strtok(NULL, ",");
            token++;
            token[strlen(token) - 1] = 0;
            strncpy(primaryRecord.surname, token, sizeof(primaryRecord.surname));

            token = strtok(NULL, "}");
            strncpy(primaryRecord.address, token, sizeof(primaryRecord.address));

            HT_InsertEntry(info, primaryRecord);
        }*/

    } else {
        fprintf(stderr, "File '%s' doesn't exists!\n", argv[i + 1]);
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
