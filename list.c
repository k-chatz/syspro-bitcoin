#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

typedef struct Node *nodePtr;

struct List {
    nodePtr start;
    nodePtr current;
};

struct Node {
    nodePtr right, left;
    void *data;
};

int l_existsNode(nodePtr node) {
    return node != NULL;
}

int l_isFirst(nodePtr target) {
    return target->left == NULL;
}

int l_isLast(nodePtr node) {
    return node->right == NULL;
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

nodePtr _attachNode(listPtr list, nodePtr newNode) {
    assert(list != NULL);
    assert(newNode != NULL);
    newNode->right = list->start;
    if (list->start != NULL)
        list->start->left = newNode;
    list->start = newNode;
    list->current = newNode;
    return newNode;
}

bool listExists(listPtr *list) {
    return (*list != NULL);
}

void listCreate(listPtr *list) {
    assert(*list == NULL);
    *list = (listPtr) malloc(sizeof(struct List));
    if ((*list) != NULL) {
        (*list)->start = NULL;
        (*list)->current = NULL;
    }
}

void *listInsert(listPtr list, void *data) {
    assert(list != NULL);
    assert(data != NULL);
    nodePtr newNode = l_createNode();
    if (newNode != NULL) {
        newNode = _attachNode(list, newNode);
        if (newNode != NULL) {
            list->current = newNode;
            list->current->data = data;
            return list->current->data;
        }
    }
    return NULL;
}

void *listGetFirstData(listPtr list) {
    assert(list != NULL);
    if (list->start != NULL)
        return list->start->data;
    else
        return NULL;
}

void *listNext(listPtr list) {
    assert(list != NULL);
    if (!l_isLast(list->current)) {
        list->current = list->current->right;
        return list->current->data;
    }
    return NULL;
}
