#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

typedef struct Node *nodePtr;

typedef void *pointer;

struct List {
    nodePtr start;
    nodePtr current;
};

struct Node {
    nodePtr right, left;
    pointer data;
};


/***Private functions***/

int l_existsNode(nodePtr node) {
    return node != NULL;
}

int l_isFirst(nodePtr target) {
    return target->left == NULL;
}

int l_isEmpty(listPtr list) {
    return list->start == NULL;
}

nodePtr l_createNode() {
    nodePtr node = (nodePtr) malloc(sizeof(struct Node));
    if (node != NULL) {
        node->right = NULL;
        node->left = NULL;
        return node;
    } else
        return NULL;
}

bool listExists(listPtr *list) {
    return (*list != NULL);
}


/***Public functions***/

void listCreate(listPtr *list) {
    assert(*list == NULL);
    *list = (listPtr) malloc(sizeof(struct List));
    if ((*list) != NULL) {
        (*list)->start = NULL;
        (*list)->current = NULL;
    }
}

bool listInsert(listPtr list, pointer data) {
    assert(list != NULL);
    assert(data != NULL);
    nodePtr newNode = l_createNode();
    if (newNode != NULL) {
        newNode->data = data;
        newNode->right = list->start;
        if (!l_isEmpty(list))
            list->start->left = newNode;
        list->start = newNode;
        list->current = newNode;
        return true;
    }
    return false;
}

pointer listNext(listPtr list) {
    assert(list != NULL);
    nodePtr tmp = NULL;
    if (list->current != NULL) {
        tmp = list->current;
        list->current = list->current->right;
        return tmp->data;
    }
    return NULL;
}

void listDestroy(listPtr list){
    //TODO: DESTROY ALL LIST NODES AND LIST!!
}
