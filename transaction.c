#include <malloc.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "wallet.h"
#include "transaction.h"
#include "bitcoin.h"

/* Execute
 * transaction*/
bool execute(Wallet senderWallet, listPtr senderTransactions, Wallet receiverWallet, listPtr receiverTransactions,
             Transaction transaction, listPtr rollback) {
    bool error = false;
    bitcoin bc = NULL;
    printf("\n• • • E X E C U T E   T R A N S A C T I O N • • •\n");

    /* Access each bitcoin of sender to perform transaction*/
    while ((bc = listNext(senderWallet->bitcoins)) != NULL) {

        printf("Try with bitcoin %lu\n", bcGetId(bc));
        bcNode l = NULL, r = NULL;

        bool done = bcInsert(bc, l, r, transaction);

        //TODO: ADD bcNode to rollback stack!
    }


    printf("\n• • • • • • • • • • • • • • • • • • • • • • • • •\n");
    return error;
}

/* Perform
 * transaction from input buffer*/
bool performTransaction(char *token, hashtable *wallets, hashtable *bitcoins, hashtable *senderHashtable,
                        hashtable *receiverHashtable, hashtable *transactionsHashtable) {
    bool error = false;
    char *senderWalletId = NULL, *receiverWalletId = NULL, *line = NULL, *transactionId = NULL;
    Transaction transaction = NULL;
    Wallet senderWallet = NULL, receiverWallet = NULL;
    listPtr rollback = NULL, senderTransactions = NULL, receiverTransactions = NULL;
    bcNode bn, parrent = NULL;

    /* Allocate space for line string to save a copy of token.*/
    line = malloc(strlen(token) * sizeof(char) + 1);
    if (line != NULL) {
        strcpy(line, token);
        transactionId = strtok(token, " ");
        printf("\nTransaction string: [%s]", line);

        /*Create a transaction through hashtable from parsed line to ensure there is no other one with the same id.*/
        if (HT_Insert(*transactionsHashtable, transactionId, line, (void **) &transaction)) {

            /* Get sender's wallet.*/
            senderWallet = HT_Get(*wallets, transaction->senderWalletId);
            if (senderWallet != NULL) {
                //printf("\nSender's wallet: [%p,'%s'] --> ", senderWallet, senderWallet->userId);
            } else {
                fprintf(stderr, "\nTransaction.c | performTransaction | Sender Wallet Hashtable GET error\n");
                error = true;
            }

            /* Get receiver's wallet.*/
            receiverWallet = HT_Get(*wallets, transaction->receiverWalletId);
            if (receiverWallet != NULL) {
                //printf("Receiver's wallet: [%p,'%s']\n", receiverWallet, receiverWallet->userId);
            } else {
                fprintf(stderr, "\nTransaction.c | performTransaction | Receiver Wallet Hashtable GET error\n");
                error = true;
            }

            /* Create/Get sender transactions list hashtable*/
            if (HT_Insert(*senderHashtable, transaction->senderWalletId, transaction->senderWalletId,
                          (void **) &senderTransactions)) {
                //printf("Transactions list of '%s': [%p]\n", transaction->senderWalletId, senderTransactions);
            } else {
                fprintf(stderr, "\nTransaction.c | performTransaction | Sender Hashtable INSERT error\n");
                error = true;
            }

            /*Create/Get receiver transactions list hashtable*/
            if (HT_Insert(*receiverHashtable, transaction->senderWalletId, transaction->senderWalletId,
                          (void **) &receiverTransactions)) {
                //printf("Transactions list of '%s': [%p]\n", transaction->receiverWalletId, receiverTransactions);
            } else {
                fprintf(stderr, "\nTransaction.c | performTransaction | Receiver Hashtable INSERT error\n");
                error = true;
            }

            /* List that stores bitcoin nodes to restore the tree to its original format in case of failure to execute
             * the transaction.*/
            listCreate(&rollback, &transaction);

            if (!error) {
                error = execute(senderWallet,
                                senderTransactions,
                                receiverWallet,
                                receiverTransactions,
                                transaction,
                                rollback
                );
            }

            if (error) {
                fprintf(stderr, "\n!! The transaction was failed, now a rollback will be performed !!\n");

                /* RollBack,
                 * remove every new item from bitcoin tree in order to restore the tree to its original state.*/
                printf("Rollback");
                while ((bn = listNext(rollback)) != NULL) {
                    printf("• ");
                    //printf("Rollback : [%p]\n", bn);
                    bcDestroyNode(bn);
                }
                printf("\n");
            }

            //listDestroy(&rollback);
        } else {
            fprintf(stderr, "\nTransaction.c | performTransaction | Transactions Hashtable INSERT error\n");
            error = true;
        }
        printf("\n");
        free(line);
    } else {
        error = true;
    }
    return error;
}

/* Perform
 * transactions from input stream*/
bool performTransactions(FILE *fp, hashtable *wallets, hashtable *bitcoins, hashtable *senderHashtable,
                         hashtable *receiverHashtable, hashtable *transactionsHashtable, char *delimiter) {
    bool error = false;
    char buf[BUF], *token = NULL;
    while (fgets(buf, BUF, fp) != NULL) {
        token = strtok(buf, delimiter);
        error = performTransaction(
                token,
                wallets,
                bitcoins,
                senderHashtable,
                receiverHashtable,
                transactionsHashtable
        );
    }
    return error;
}

/* @Callback
 * Initialize & return a new transaction*/
Transaction createTransaction(char *token) {
    Transaction transaction = NULL;
    struct tm tmVar;

    transaction = (Transaction) malloc(sizeof(struct Transaction));
    if (transaction != NULL) {

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

        token = strtok(NULL, "\0"); // Date time
        if (sscanf(token, "%d-%d-%d %d:%d", &tmVar.tm_mday, &tmVar.tm_mon, &tmVar.tm_year, &tmVar.tm_hour,
                   &tmVar.tm_min) ==
            5) {
            transaction->timestamp = mktime(&tmVar);
            if (transaction->timestamp < 0) {
                fprintf(stderr, "\nBad datetime!\n");
                return NULL;
            }
        } else {
            fprintf(stderr, "\nBad datetime format!\n");
            return NULL;
        }
    }
    return transaction;
}

/* @Callback
 * Compare transaction with userId field*/
int cmpTransaction(Transaction transaction, char *transactionId) {
    return strcmp(transaction->transactionId, transactionId);
}

/* @Callback
 * Hash function for transactions hashtable*/
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
