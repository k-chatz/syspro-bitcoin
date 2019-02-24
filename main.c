#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void wrongOptionValue(char *opt, char *val);

int main(int argc, char *argv[]) {
    int i, bucketSize = 0, bitCoinValue = 0, senderHashtableNumOfEntries = 0, receiverHashtableNumOfEntries = 0;
    char *opt, *optVal, *bitCoinBalancesFile = NULL, *transactionsFile = NULL;
    for (i = 1; i < argc; ++i) {
        opt = argv[i];
        optVal = argv[i + 1];

        if (strcmp(opt, "-a") == 0) {
            if (optVal != NULL && optVal[0] != '-') { /*bitCoinBalancesFile*/
                bitCoinBalancesFile = optVal;
            } else {
                wrongOptionValue(opt, optVal);
            }
        } else if (strcmp(opt, "-t") == 0) { /*transactionsFile*/
            if (optVal != NULL && optVal[0] != '-') {
                transactionsFile = optVal;
            } else {
                wrongOptionValue(opt, optVal);
            }
        } else if (strcmp(opt, "-v") == 0) { /*bitCoinValue*/
            if (optVal != NULL && optVal[0] != '-') {
                bitCoinValue = atoi(optVal);
            } else {
                wrongOptionValue(opt, optVal);
            }
        } else if (strcmp(opt, "-h1") == 0) { /*senderHashtableNumOfEntries*/
            if (optVal != NULL && optVal[0] != '-') {
                senderHashtableNumOfEntries = atoi(optVal);
            } else {
                wrongOptionValue(opt, optVal);
            }
        } else if (strcmp(opt, "-h2") == 0) { /*receiverHashtableNumOfEntries*/
            if (optVal != NULL && optVal[0] != '-') {
                receiverHashtableNumOfEntries = atoi(optVal);
            } else {
                wrongOptionValue(opt, optVal);
            }
        } else if (strcmp(opt, "-b") == 0) { /*bucketSize*/
            if (optVal != NULL && optVal[0] != '-') {
                bucketSize = atoi(optVal);
            } else {
                wrongOptionValue(opt, optVal);
            }
        }
    }

    printf("bitCoinBalancesFile: %s\n", bitCoinBalancesFile);
    printf("transactionsFile: %s\n", transactionsFile);
    printf("bitCoinValue: %d\n", bitCoinValue);
    printf("senderHashtableNumOfEntries: %d\n", senderHashtableNumOfEntries);
    printf("receiverHashtableNumOfEntries: %d\n", receiverHashtableNumOfEntries);
    printf("bucketSize: %d\n", bucketSize);


    FILE *pFile = fopen(bitCoinBalancesFile, "r");
    if (pFile != NULL) {

        int nbytes = 1024;
        char *line = (char *) malloc(nbytes);
        char *buf = (char *) malloc(nbytes);

        size_t bytes_read;
        int linesize = 0;
        while (fgets(buf, nbytes, pFile) != NULL) {
            bytes_read = strlen(buf);
            // if line length larger than size of line buffer
            if (linesize + bytes_read > nbytes) {
                char *tmp = line;
                nbytes += nbytes / 2;
                line = (char *) malloc(nbytes);
                memcpy(line, tmp, linesize);
                free(tmp);
            }
            memcpy(line + linesize, buf, bytes_read);
            linesize += bytes_read;

            if (feof(pFile) || buf[bytes_read - 1] == '\n') {

                printf("line: %s", line);
                //handle_line(line);
                linesize = 0;
                memset(line, '\0', nbytes);
            }
        }

        free(buf);
        free(line);

        printf("\n");

    } else {
        fprintf(stderr, "File '%s' doesn't exists!\n", argv[i + 1]);
        exit(1);
    }


    return EXIT_SUCCESS;
}

void wrongOptionValue(char *opt, char *val) {
    fprintf(stderr, "Wrong value [%s] for option '%s'\n", val, opt);
    exit(1);
}
