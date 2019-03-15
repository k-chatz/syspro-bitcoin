#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <zconf.h>
#include <time.h>
#include "hash.h"
#include "list.h"
#include "wallet.h"
#include "bitcoin.h"
#include "transaction.h"

#define BUFFER_SIZE 256

typedef void *pointer;

bool init_complete = false;

time_t max_transaction_timestamp = 0;

Hashtable walletsHT = NULL, bitcoinsHT = NULL, senderHT = NULL, receiverHT = NULL, transactionsHT = NULL;

void wrongOptionValue(char *opt, char *val) {
    fprintf(stdout, "Wrong value [%s] for option '%s'\n", val, opt);
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

void init(Hashtable *walletsHT, Hashtable *bitcoinsHT, char *a, unsigned long int v, unsigned long int b,
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
                walletsHT,
                h1 > h2 ? h1 : h2,
                b,
                (pointer (*)(pointer)) createWallet,
                (int (*)(pointer, pointer)) cmpWallet,
                (unsigned long (*)(pointer, unsigned long int)) walletHash,
                (unsigned long (*)(pointer)) destroyWallet
        );

        /*Initialize bitcoins hashtable*/
        HT_Init(
                bitcoinsHT,
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
                if (HT_Insert(*walletsHT, token, token, (void **) &wallet)) {

                    assert(wallet != NULL);

                    /*Read bitcoins for current wallet*/
                    do {
                        token = strtok(NULL, " ");
                        if (token != NULL) {
                            bid = (unsigned long int) strtol(token, NULL, 10);

                            /*Initialize parameters for bitcoin*/
                            htBitCoinParams.walletId = wallet->userId;
                            htBitCoinParams.bid = bid;
                            htBitCoinParams.v = v;

                            /*Insert bitcoin into bitcoins hashtable*/
                            if (HT_Insert(*bitcoinsHT, &bid, &htBitCoinParams, (void **) &bc)) {

                                assert(bc != NULL);

                                //printf("[%lu] [%p] \n", bid, bc);
                                wallet->balance += v;

                                /*Insert user bitcoin entry in wallet's bitcoin list*/
                                if (!listInsert(wallet->bitcoins, bc)) {
                                    fprintf(stdout, "\nBitCoin [%lu] was not inserted in wallet list!\n", bid);
                                };

                            } else {
                                fprintf(stdout, "\nHT BitCoin [%p] was not inserted because is duplicate!\n", bc);
                                HT_Destroy(walletsHT, true);
                                HT_Destroy(bitcoinsHT, true);
                                exit(EXIT_FAILURE);
                            }
                        }
                    } while (token != NULL);
                } else {
                    fprintf(stdout, "\nWallet [%s] was not inserted because is duplicate!\n", token);
                    destroyWallet(wallet);
                    HT_Destroy(walletsHT, true);
                    HT_Destroy(bitcoinsHT, true);
                    exit(EXIT_FAILURE);
                }
                //printf("\n\n");
            }
        }
        fclose(fp);
    } else {
        fprintf(stdout, "\nFile '%s' doesn't exists!\n", a);
        exit(EXIT_FAILURE);
    }
}

void initTransactions(Hashtable *walletsHT, Hashtable *bitcoins, Hashtable *senderHT, Hashtable *receiverHT,
                      Hashtable *transactionsHT, const unsigned long int h1, const unsigned long int h2,
                      unsigned long int b,
                      char *t, unsigned long int v) {
    FILE *fp = NULL;

    /*Open transactionsFile*/
    fp = fopen(t, "r");
    if (fp != NULL) {

        /* Initialize hashtable for sender transactions list hashtable.*/
        HT_Init(
                senderHT,
                h1,
                b,
                (pointer (*)(pointer)) createTransactionList,
                (int (*)(pointer, pointer)) cmpTransactionList,
                (unsigned long (*)(pointer, unsigned long int)) walletHash,
                (unsigned long (*)(pointer)) destroyTransactionList
        );

        /* Initialize hashtable for receiver transactions list hashtable.*/
        HT_Init(
                receiverHT,
                h2,
                b,
                (pointer (*)(pointer)) createTransactionList,
                (int (*)(pointer, pointer)) cmpTransactionList,
                (unsigned long (*)(pointer, unsigned long int)) walletHash,
                (unsigned long (*)(pointer)) destroyTransactionList
        );

        /* Initialize hashtable for transactions hashtable.*/
        HT_Init(
                transactionsHT,
                h1 > h2 ? h1 : h2,
                b,
                (pointer (*)(pointer)) createTransaction,
                (int (*)(pointer, pointer)) cmpTransaction,
                (unsigned long (*)(pointer, unsigned long int)) transactionHash,
                (unsigned long (*)(pointer)) destroyTransaction
        );

        /* Run transactions from file in order to initialize the simulation.*/
        performTransactions(fp, walletsHT, senderHT, receiverHT, transactionsHT, "\n");

        fclose(fp);
    } else {
        fprintf(stdout, "File '%s' doesn't exists!\n", t);
        exit(EXIT_FAILURE);
    }
}

/* Cli command*/
void requestTransaction(char *input) {
    if (!performTransaction(input, &walletsHT, &senderHT, &receiverHT, &transactionsHT)) {
        fprintf(stdout, "~ success: The transaction was executed successfully.\n");
    }
}

/* Cli command*/
void requestTransactions(char *input) {
    FILE *fp = NULL;
    int q, s;
    char buf[BUFFER_SIZE];
    if (input != NULL) {
        /* Use this sscanf only for check the number of arguments to determine if the input is file of transactions
         * or actual transactions.*/
        s = sscanf(input, "%s %s %d %d-%d-%d %d:%d;", buf, buf, &q, &q, &q, &q, &q, &q);
        if (s == 1) {
            fp = fopen(input, "r");
            if (fp != NULL) {
                performTransactions(fp, &walletsHT, &senderHT, &receiverHT, &transactionsHT, ";\n");
                fclose(fp);
            } else {
                fprintf(stdout, "\nFile '%s' not found!\n", input);
            }
        } else if (s == 8) {
            performTransaction(input, &walletsHT, &senderHT, &receiverHT, &transactionsHT);
            performTransactions(stdin, &walletsHT, &senderHT, &receiverHT, &transactionsHT, ";\n");
        } else {
            fprintf(stdout, "~ error: bad input!\n");
        }
    } else {
        fprintf(stdout, "~ error: bad input!\n");
    }
}

/* TODO Cli command*/

/*
* ​/findEarnings walletID [time1][year1][time2][year2]
*  HASHTABLE!!!
Η εφαρμογή πρώτα επιστρέφει το συνολικό ποσόν που έχει λάβει μέσω συναλλαγών ο χρηστής με userID
walletID ​(με επιλογή στο εύρος χρόνου ή/και ημερομηνίας).

Αν υπάρχει ορισμός για [time1] θα πρέπει να υφίσταται και ορισμός για [time2].

Επίσης το ίδιο ισχύει και για την χρήση των μη υποχρεωτικών παραμέτρων ​[year1] και [year2]​.

Στη συνεχεία, παρουσιάζει όλες τις εγγραφές συναλλαγών του χρήστη (ως παραλήπτης) που εκτελέστηκαν επιτυχώς
μέσα στο συγκεκριμένο διάστημα. Αν δεν ορίζεται διάστημα, τότε η εφαρμογή θα παρουσιάζει την πλήρη
ιστορία συναλλαγών όπου το ​walletID ​είναι παραλήπτης.
*/
void findEarnings(char *input) {
    List list = NULL;
    Transaction transaction = NULL;
    Wallet wallet = HT_Get(walletsHT, input);
    if (input != NULL) {
        if (wallet != NULL) {
            printf("Wallet status for '%s' is: %lu$\n", wallet->userId, wallet->balance);
            list = HT_Get(receiverHT, wallet->userId);
            if (list != NULL) {
                while ((transaction = listNext(list)) != NULL) {
                    transactionPrint(transaction);
                }
            } else {
                fprintf(stdout, "~ error: earnings list for wallet '%s' not found!\n", input);
            }
        } else {
            fprintf(stdout, "~ error: wallet '%s' not found!\n", input);
        }
    } else {
        fprintf(stdout, "~ error: bad input\n");
    }
}

/* TODO Cli command*/
void findPayments(char *input) {
    List list = NULL;
    Transaction transaction = NULL;
    char *rest = NULL, *walletId = NULL;
    int x;
    Wallet wallet = NULL;
    time_t start = 0, end = 0;

    struct tm s = {0}, e = {0};

    if (input != NULL) {

        walletId = strtok_r(input, " \n", &rest);
        if (rest != NULL) {
            /*Parse date & time*/
            x = sscanf(rest, "%d-%d-%d %d:%d", &s.tm_mday, &s.tm_mon, &s.tm_year, &s.tm_hour, &s.tm_min);

            if (x == 5) {
                s.tm_year = s.tm_year - 1900;
                s.tm_mon = s.tm_mon - 1;
                s.tm_isdst = -1;
                start = mktime(&s);
          /*      if (transaction->timestamp < 0) {
                    fprintf(stdout, "\nBad datetime!\n");
                }*/
            }
        }

        wallet = HT_Get(walletsHT, walletId);
        if (wallet != NULL) {
            printf("Wallet status for '%s' is: %lu$\n", wallet->userId, wallet->balance);
            list = HT_Get(senderHT, wallet->userId);
            if (list != NULL) {
                while ((transaction = listNext(list)) != NULL) {

                    transactionPrint(transaction);

                    /*- ​/findPayments walletID [time1][year1][time2][year2]
                   *  HASHTABLE!!!
                   Η εφαρμογή επιστρέφει το συνολικό ποσόν που έχει στείλει επιτυχώς μέσω συναλλαγών ο χρηστής με userID
                   walletID ​(με επιλογή στο εύρος χρόνου ή/και ημερομηνίας).

                   Στη συνεχεία, παρουσιάζει όλες τις εγγραφές συναλλαγών του χρήστη (ως αποστολέας) που εκτελέστηκαν επιτυχώς
                   μέσα στο διάστημα που έχει δοθεί στη γραμμή εντολής.

                   Αν δεν ορίζεται διάστημα, τότε η εφαρμογή θα παρουσιάζει την πλήρη ιστορία συναλλαγών όπου το ​walletID ​είναι
                   αποστολέας.
                   */

                }
            } else {
                fprintf(stdout, "~ error: earnings list for wallet '%s' not found!\n", input);
            }
        } else {
            fprintf(stdout, "~ error: wallet '%s' not found!\n", input);
        }
    } else {
        fprintf(stdout, "~ error: bad input\n");
    }
}

/* Cli command*/
void walletStatus(char *input) {
    if (input != NULL) {
        Wallet wallet = HT_Get(walletsHT, input);
        if (wallet != NULL) {
            printf("Wallet status for '%s' is: %lu$\n", wallet->userId, wallet->balance);
        } else {
            fprintf(stdout, "~ error: wallet '%s' not found!\n", input);
        }
    } else {
        fprintf(stdout, "~ error: bad input!\n");
    }
}

/* Cli command*/
void bitCoinStatus(char *input) {
    unsigned long int bitcoinId = 0, value = 0, transactions = 0, unspent = 0;
    if (input != NULL) {
        bitcoinId = (unsigned long int) strtol(input, NULL, 10);
        if (bitcoinId > 0) {
            bitcoin bc = HT_Get(bitcoinsHT, &bitcoinId);
            if (bc != NULL) {
                bcTrace(bc, &value, &transactions, &unspent, true, false);
                printf("%lu %lu %lu %lu\n", bitcoinId, value, transactions, unspent);
            } else {
                fprintf(stdout, "~ error: bitcoin %lu doesn't exists!\n", bitcoinId);
            }
        } else {
            fprintf(stdout, "~ error: bad input, not a number!\n");
        }
    } else {
        fprintf(stdout, "~ error: bad input, expected bitcoin number!\n");
    }
}

/* Cli command*/
void traceCoin(char *input) {
    unsigned long int bitcoinId = 0, value = 0, transactions = 0, unspent = 0;
    bitcoinId = (unsigned long int) strtol(input, NULL, 10);
    if (input != NULL) {
        if (bitcoinId > 0) {
            bitcoin bc = HT_Get(bitcoinsHT, &bitcoinId);
            if (bc != NULL) {
                bcTrace(bc, &value, &transactions, &unspent, true, true);
            } else {
                fprintf(stdout, "~ error: bitcoin %lu doesn't exists!\n", bitcoinId);
            }
        } else {
            fprintf(stdout, "~ error: bad input, not a number!\n");
        }
    } else {
        fprintf(stdout, "~ error: bad input, expected bitcoin number!\n");
    }
}

/* Command line interface
 * Get input from user to perform various cli commands.*/
void cli() {
    int fd;
    char *input = NULL, *line = NULL, *cmd = NULL;
    size_t len = 0;
    ssize_t read;
    printf("Welcome to bitcoin transactions simulator, write 'exit' to quit from cli or use default commands.\n\n");
    putchar('>');
    while ((read = getline(&line, &len, stdin)) != EOF) {
        input = line;
        cmd = strtok_r(input, " \n", &input);
        input = strtok(input, "\n");
        if (cmd != NULL) {
            if (strcmp(cmd, "requestTransaction") == 0 || strcmp(cmd, "rt") == 0 || strcmp(cmd, "RT") == 0) {
                requestTransaction(input);
            } else if (strcmp(cmd, "requestTransactions") == 0 || strcmp(cmd, "rts") == 0 || strcmp(cmd, "RTS") == 0) {
                requestTransactions(input);
            } else if (strcmp(cmd, "findEarnings") == 0) {
                findEarnings(input);
            } else if (strcmp(cmd, "findPayments") == 0) {
                findPayments(input);
            } else if (strcmp(cmd, "walletStatus") == 0) {
                walletStatus(input);
            } else if (strcmp(cmd, "bitCoinStatus") == 0) {
                bitCoinStatus(input);
            } else if (strcmp(cmd, "traceCoin") == 0) {
                traceCoin(input);
            } else if (strcmp(cmd, "exit") == 0) {
                fd = open("/dev/null", O_WRONLY);
                dup2(fd, 0);
                close(fd);
                break;
            } else {
                fprintf(stdout, "~ error: %s: command not found!\n", cmd);
            }
        }
        putchar('>');
        putchar(' ');
    }
    free(line);
}

int main(int argc, char *argv[]) {
    unsigned long int h1 = 0, h2 = 0, b = 0, v = 0;
    char *a = NULL, *t = NULL;

    srand((unsigned int) time(NULL));

    /*Read argument options from command line*/
    readOptions(argc, argv, &a, &t, &v, &h1, &h2, &b);

    /*Init structures*/
    init(&walletsHT, &bitcoinsHT, a, v, b, h1, h2);

    /*Initialize with some transactions*/
    initTransactions(&walletsHT, &bitcoinsHT, &senderHT, &receiverHT, &transactionsHT, h1, h2, b, t,
                     v);

    /*Set init flag to true*/
    init_complete = true;

    /*Get input from user to perform various cli commands*/
    cli();

    /*Deallocate previously allocated memory.*/
    HT_Destroy(&walletsHT, true);
    HT_Destroy(&bitcoinsHT, true);
    HT_Destroy(&senderHT, true);
    HT_Destroy(&receiverHT, true);
    HT_Destroy(&transactionsHT, true);
    return EXIT_SUCCESS;
}
