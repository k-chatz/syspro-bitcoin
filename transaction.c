#include <malloc.h>
#include <string.h>
#include "tree.h"
#include "transaction.h"

/*Create
 * Initialize & return a new wallet*/
Transaction createTransaction(char *buf) {
    Transaction transaction = (Transaction) malloc(sizeof(struct Transaction));
    transaction->transactionId = "EB79AF67353DD4F157FDBFCA66BCAE5D";
    transaction->node = NULL;
    transaction->value = 25;
    transaction->timestamp = 584;
    return transaction;
}

/*Execute
 * Execute transaction from input buffer*/
bool performTransaction(char *buf) {
    bool error = false;
    printf("[%s]\n", buf);
    buf = strtok(buf, " :");
    buf = strtok(NULL, " :");
    buf = strtok(NULL, " :");
    buf = strtok(NULL, " :");
    buf = strtok(NULL, " :");
    buf = strtok(NULL, " :");
    buf = strtok(NULL, " :");
    buf = strtok(NULL, " :");
    buf = strtok(NULL, " :");


/*
    do {
        buf = strtok(NULL, " ");
        if (buf != NULL)
            printf("%s ", buf);
    } while (buf != NULL);
*/


    return error;
}

/*Parse
 * Parse transactions from input stream*/
bool performTransactions(FILE *fp) {
    bool error = false;
    char buf[BUF], *token = NULL;
    while (fgets(buf, BUF, fp) != NULL) {
        token = strtok(buf, "\n");
        error = performTransaction(token);
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
