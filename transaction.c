#include <malloc.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include "wallet.h"
#include "transaction.h"
#include "bitcoin.h"

/* Execute
 * transaction*/
bool execute(Wallet senderWallet, listPtr senderTransactions, Wallet receiverWallet, listPtr receiverTransactions,
             Transaction transaction) {
    assert(senderWallet != NULL);
    assert(senderTransactions != NULL);
    assert(receiverWallet != NULL);
    assert(receiverTransactions != NULL);
    assert(transaction != NULL);
    assert(transaction->value > 0);
    bool success = true;
    unsigned long int rest = transaction->value;
    bitcoin bc = NULL;
    printf("\n• • • • • • • • E X E C U T E   T R A N S A C T I O N • • • • • • • •\n");

    printf("Transaction %s: %s --> %s %lu\n",
           transaction->transactionId,
           transaction->senderWalletId,
           transaction->receiverWalletId,
           transaction->value
    );

    /* Access each bitcoin of sender to perform transaction*/
    while (rest > 0 && (bc = listNext(senderWallet->bitcoins)) != NULL) {
        printf("\nTry with bitcoin [%lu] ", bcGetId(bc));
        bcInsert(bc, &rest, transaction);
        printf("rest: [%lu]\n", rest);
    }

    if (rest == 0) {
        senderWallet->balance -= transaction->value;
        receiverWallet->balance += transaction->value;
    } else {
        printf("Summary rest: [%lu]", rest);
        success = false;
    }

    printf("\n• • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • •\n\n");
    return success;
}

/* Perform
 * transaction from input buffer*/
bool performTransaction(char *input, Hashtable *walletsHT, Hashtable *senderHT, Hashtable *receiverHT,
                        Hashtable *transactionsHT) {
    bool error = false;
    char *line = NULL, *transactionId = NULL;
    int i;
    Transaction transaction = NULL;
    Wallet senderWallet = NULL, receiverWallet = NULL;
    listPtr senderTransactions = NULL, receiverTransactions = NULL;

    if (init_complete) {
        transactionId = malloc(sizeof(char) * 51);
        for (i = 0; i < 50; i++) {
            transactionId[i] = (char) (rand() % 26 + 65);
        }
        line = (char *) malloc(strlen(transactionId) + 1 * sizeof(char) + 1 + strlen(input) * sizeof(char) + 1);
        char *tmp = strdup(input);
        strcpy(line, transactionId);
        strcat(line, " ");
        strcat(line, tmp);
        free(tmp);
    } else {
        /* Allocate space for line string to save a copy of token.*/
        line = malloc(strlen(input) * sizeof(char) + 1);
    }

    if (line != NULL) {
        if (!init_complete) {
            strcpy(line, input);
            transactionId = strtok(input, " ");
        } else {
            printf("Request for transaction: [%s]\n", line);
        }

        /*Create a transaction through hashtable from parsed line to ensure there is no other one with the same id.*/
        if (HT_Insert(*transactionsHT, transactionId, line, (void **) &transaction)) {
            assert(transaction != NULL);

            /* Get sender's wallet.*/
            senderWallet = HT_Get(*walletsHT, transaction->senderWalletId);
            if (senderWallet == NULL) {
                fprintf(stdout, "Unacceptable transaction '%s': Sender's wallet '%s' doesn't exists!\n",
                        transaction->transactionId, transaction->senderWalletId);
                error = true;
            } else {

                /* Create/Get sender transactions list hashtable*/
                HT_Insert(*senderHT, transaction->senderWalletId, transaction->senderWalletId,
                          (void **) &senderTransactions);

                assert(senderTransactions != NULL);
            }

            /* Get receiver's wallet.*/
            receiverWallet = HT_Get(*walletsHT, transaction->receiverWalletId);
            if (receiverWallet == NULL) {
                fprintf(stdout, "Unacceptable transaction '%s': Receivers's wallet '%s' doesn't exists!\n",
                        transaction->transactionId, transaction->receiverWalletId);
                error = true;
            } else {

                /*Create/Get receiver transactions list hashtable*/
                HT_Insert(*receiverHT, transaction->senderWalletId, transaction->senderWalletId,
                          (void **) &receiverTransactions);

                assert(receiverTransactions != NULL);
            }

            if (!error) {
                if (transaction->timestamp < max_transaction_timestamp) {
                    error = true;
                    fprintf(stdout, "Unacceptable transaction '%s': Date is less than the current time!\n",
                            transaction->transactionId);
                } else {
                    if (transaction->value > senderWallet->balance) {
                        error = true;
                        fprintf(stdout,
                                "Unacceptable transaction '%s': Sender's money is not enough!\n",
                                transaction->transactionId);
                    } else {
                        if (execute(senderWallet, senderTransactions, receiverWallet, receiverTransactions,
                                    transaction)) {
                            if (transaction->timestamp > max_transaction_timestamp) {
                                max_transaction_timestamp = transaction->timestamp;
                            }
                        } else {
                            fprintf(stdout, "Transaction '%s' was fail!\n", transaction->transactionId);
                            error = true;
                        }
                    }
                }
            } else {
                /* Remove transaction from hashtable in order to normally inserted the next time.*/
                HT_Remove(*transactionsHT, transaction->transactionId, transaction->transactionId, true);
            }
        } else {
            if (transaction != NULL) {
                fprintf(stdout, "Unacceptable transaction '%s': Transaction is duplicate!\n",
                        transaction->transactionId);
            } else {
                fprintf(stdout, "Unacceptable transaction '%s': Bad format!\n", transactionId);
            }
            error = true;
        }
        if (init_complete) {
            free(transactionId);
        }
        free(line);
    } else {
        error = true;
        fprintf(stdout, "Bad transaction format!\n");
    }
    return error;
}

/* Perform
 * transactions from input stream*/
bool performTransactions(FILE *fp, Hashtable *walletsHT, Hashtable *senderHT,
                         Hashtable *receiverHT, Hashtable *transactionsHT, char *delimiter) {
    bool error = false;
    char buf[BUF], *token = NULL;
    while (fgets(buf, BUF, fp) != NULL) {
        token = strtok(buf, delimiter);
        if (token != NULL) {
            error = performTransaction(token, walletsHT, senderHT, receiverHT, transactionsHT);
        } else {
            break;
        }
    }
    return error;
}

/* @Callback
 * Initialize & return a new transaction*/
Transaction createTransaction(char *token) {
    Transaction transaction = NULL;

    /* Initialize t to avoid valgrind errors as described here:
     * https://stackoverflow.com/questions/9037631/valgrind-complaining-about-mktime-is-that-my-fault
     * by freitass's comment.*/
    struct tm t = {0};
    int x = 0;
    transaction = (Transaction) malloc(sizeof(struct Transaction));
    if (transaction != NULL && token != NULL) {

        token = strtok(token, " "); // TransactionId
        transaction->transactionId = malloc(strlen(token) * sizeof(char *) + 1);
        if (transaction->transactionId) {
            strcpy(transaction->transactionId, token);
        }

        token = strtok(NULL, " "); // SenderWalletId
        transaction->senderWalletId = malloc(strlen(token) * sizeof(char *) + 1);
        if (transaction->senderWalletId != NULL) {
            strcpy(transaction->senderWalletId, token);
        }

        token = strtok(NULL, " "); // RecieverWalletId
        transaction->receiverWalletId = malloc(strlen(token) * sizeof(char *) + 1);
        if (transaction->receiverWalletId != NULL) {
            strcpy(transaction->receiverWalletId, token);
        }

        token = strtok(NULL, " "); // Amount
        transaction->value = (unsigned long int) strtol(token, NULL, 10);

        token = strtok(NULL, "\n"); // Date time

        if (token != NULL) {
            /*Parse date & time*/
            x = sscanf(token, "%d-%d-%d %d:%d",
                       &t.tm_mday, &t.tm_mon, &t.tm_year, &t.tm_hour, &t.tm_min
            );
            if (x == 5) {
                t.tm_year = t.tm_year - 1900;
                t.tm_mon = t.tm_mon - 1;
                t.tm_isdst = -1;
                transaction->timestamp = mktime(&t);
                if (transaction->timestamp < 0) {
                    fprintf(stdout, "\nBad datetime!\n");
                    return NULL;
                }
            } else {
                time(&transaction->timestamp);
            }
        } else {
            time(&transaction->timestamp);
        }
    }
    return transaction;
}

/* @Callback
 * Compare transaction*/
int cmpTransaction(Transaction transaction, char *transactionStr) {
    int result = 0;
    char *token = malloc(strlen(transactionStr) * sizeof(char *) + 1);
    strcpy(token, transactionStr);
    token = strtok(token, " ");
    result = strcmp(transaction->transactionId, token);
    free(token);
    return result;
}

/* @Callback
 * Hash function for transactions*/
unsigned long int transactionHash(char *key, unsigned long int capacity) {
    int i, sum = 0;
    size_t keyLength = strlen(key);
    for (i = 0; i < keyLength; i++) {
        sum += key[i];
    }
    return sum % capacity;
}

/* @Callback
 * Destroy transaction*/
void destroyTransaction(Transaction transaction) {
    free(transaction->transactionId);
    free(transaction->senderWalletId);
    free(transaction->receiverWalletId);
    free(transaction);
}

/* @Callback
 * Initialize & return a new transaction list*/
listPtr createTransactionList(char *userId) {
    listPtr list = NULL;
    listCreate(&list, userId);
    return list;
}

/* @Callback
 * Compare keys function for transaction lists hashtable*/
int cmpTransactionList(listPtr tr1, char *userId) {
    return listGetIdentifier(tr1) != userId;
}

/* @Callback
 * Destroy function for transaction hashtable*/
void destroyTransactionList(listPtr list) {
    listDestroy(&list);
}
