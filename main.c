#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int i;

    for (i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-a") == 0) {
            if (argv[i + 1] != NULL && argv[i + 1][0] != '-') {
                fprintf(stdout, "Argument %s receive the value: %s\n", argv[i], argv[i + 1]);
            } else {
                fprintf(stderr, "Argument %s receive wrong value: %s\n", argv[i], argv[i + 1]);
            }
        } else if (strcmp(argv[i], "-t") == 0) {
            if (argv[i + 1] != NULL && argv[i + 1][0] != '-') {
                fprintf(stdout, "Argument %s receive the value: %s\n", argv[i], argv[i + 1]);
            } else {
                fprintf(stderr, "Argument %s receive wrong value: %s\n", argv[i], argv[i + 1]);
            }
        } else if (strcmp(argv[i], "-v") == 0) {
            if (argv[i + 1] != NULL && argv[i + 1][0] != '-') {
                fprintf(stdout, "Argument %s receive the value: %s\n", argv[i], argv[i + 1]);
            } else {
                fprintf(stderr, "Argument %s receive wrong value: %s\n", argv[i], argv[i + 1]);
            }
        } else if (strcmp(argv[i], "-h1") == 0) {
            if (argv[i + 1] != NULL && argv[i + 1][0] != '-') {
                fprintf(stdout, "Argument %s receive the value: %s\n", argv[i], argv[i + 1]);
            } else {
                fprintf(stderr, "Argument %s receive wrong value: %s\n", argv[i], argv[i + 1]);
            }
        } else if (strcmp(argv[i], "-h2") == 0) {
            if (argv[i + 1] != NULL && argv[i + 1][0] != '-') {
                fprintf(stdout, "Argument %s receive the value: %s\n", argv[i], argv[i + 1]);
            } else {
                fprintf(stderr, "Argument %s receive wrong value: %s\n", argv[i], argv[i + 1]);
            }
        } else if (strcmp(argv[i], "-b") == 0) {
            if (argv[i + 1] != NULL && argv[i + 1][0] != '-') {
                fprintf(stdout, "Argument %s receive the value: %s\n", argv[i], argv[i + 1]);
            } else {
                fprintf(stderr, "Argument %s receive wrong value: %s\n", argv[i], argv[i + 1]);
            }
        }
    }
    return EXIT_SUCCESS;
}
