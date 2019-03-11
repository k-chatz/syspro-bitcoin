#include <malloc.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "tree.h"
#include "transaction.h"

/*Create
 * Initialize & return a new transaction*/
Transaction createTransaction(char *token) {
    Transaction transaction = NULL;
    char *senderWalletId = NULL, *receiverWalletId = NULL;
    time_t timestamp;
    struct tm tmVar;

    transaction = (Transaction) malloc(sizeof(struct Transaction));
    if (transaction != NULL) {

        transaction->treeNode = NULL;
        printf("[%s]\n", token);

        token = strtok(token, " "); // TransactionId
        transaction->transactionId = malloc(strlen(token) * sizeof(char *));
        strcpy(transaction->transactionId, token);

        token = strtok(NULL, " "); // SenderWalletId
        senderWalletId = token;

        token = strtok(NULL, " "); // RecieverWalletId
        receiverWalletId = token;

        token = strtok(NULL, " "); // Amount
        transaction->value = (unsigned long int) strtol(token, NULL, 10);

        token = strtok(NULL, "\0"); // Date time

        if (sscanf(token, "%d-%d-%d %d:%d", &tmVar.tm_mday, &tmVar.tm_mon, &tmVar.tm_year, &tmVar.tm_hour,
                   &tmVar.tm_min) ==
            5) {
            transaction->timestamp = mktime(&tmVar);
        } else {
            printf("error");

        }

    }
    return transaction;
}

/*Callback
 * Compare transaction with userId field*/
int cmpTransaction(Transaction transaction, char *transactionId) {
    return strcmp(transaction->transactionId, transactionId);
}

/*Callback
 * Hash function for transactions hashtable*/
unsigned long int transactionHash(char *key, unsigned long int capacity) {
    int i, sum = 0;
    size_t keyLength = strlen(key);
    for (i = 0; i < keyLength; i++) {
        sum += key[i];
    }
    return sum % capacity;
}

/*Callback
 * Destroy transaction*/
void destroyTransaction(Transaction transaction) {
    free(transaction->transactionId);
    free(transaction);
}

/*Execute
 * Execute transaction from input buffer*/
bool performTransaction(char *token,
                        hashtable *wallets,
                        hashtable *bitCoins,
                        hashtable *senderHashtable,
                        hashtable *receiverHashtable,
                        hashtable *transactionsHashtable
) {
    bool error = false;
    Transaction transaction = NULL;
    char *senderWalletId = NULL, *recieverWalletId = NULL, *line = NULL, *transactionId = NULL;
    time_t timestamp;
    struct tm tmVar;


    //line = token;
    transactionId = strtok_r(token, " ", &line);

    //TODO: Change this
    line = strcat(token, " ");
    line = strcat(line, line);

    if (HT_Insert(*transactionsHashtable, transactionId, strcat(token, line), (void **) &transaction)) {
        printf("transaction: [%s]", transaction->transactionId);
    }

    //Wallet = HT_Get(wallets, "A");
//    char *userId = "A";
//    listPtr transactions = NULL;
//    HT_Insert(*senderHashtable, userId, userId, (void **) &transactions);
//    printf("list of A: [%p] \n", transactions);
//
//    HT_Insert(*senderHashtable, userId, userId, (void **) &transactions);
//    printf("I expect the same list: [%p] \n", transactions);
//
//    userId = "BBB";
//
//    HT_Insert(*senderHashtable, userId, userId, (void **) &transactions);
//    printf("I need new list for BBB: [%p] \n", transactions);

    return error;
}

/*Parse
 * Parse transactions from input stream*/
bool performTransactions(
        FILE *fp,
        hashtable *wallets,
        hashtable *bitCoins,
        hashtable *senderHashtable,
        hashtable *receiverHashtable,
        hashtable *transactionsHashtable,
        char *delim
) {
    bool error = false;
    char buf[BUF], *token = NULL;
    while (fgets(buf, BUF, fp) != NULL) {
        token = strtok(buf, delim);
        error = performTransaction(
                token,
                wallets,
                bitCoins,
                senderHashtable,
                receiverHashtable,
                transactionsHashtable
        );
    }
    return error;
}

/*Create
 * Initialize & return a new transaction list*/
listPtr createTransactionList(char *userId) {
    listPtr list = NULL;
    listCreate(&list, userId);
    return list;
}

/*Callback
 * Compare keys function for transaction lists hashtable*/
int cmpTransactionList(listPtr tr1, char *userId) {
    return listGetIdentifier(tr1) != userId;
}

/*Callback
 * Destroy function for transaction hashtable*/
void destroyTransactionList(listPtr list) {
    listDestroy(&list);
}
