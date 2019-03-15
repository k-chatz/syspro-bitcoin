#include <malloc.h>
#include <assert.h>
#include <stdbool.h>
#include "queue.h"

typedef void *pointer;

struct Queue {
    unsigned long int front;
    unsigned long int rear;
    unsigned long int counter;
    unsigned long int size;
    pointer *queue;
};

/***Private functions***/

bool _isEmpty(Queue q) {
    return (q->counter == 0);
}

unsigned long int _size(Queue q) {
    return q->counter;
}

bool _isFull(Queue q) {
    return (q->counter == q->size);
}

/***Public functions***/

void queueCreate(Queue *queue, unsigned long int size) {
    assert(*queue == NULL);
    int i = 0;
    *queue = (Queue) malloc(sizeof(struct Queue));
    (*queue)->counter = 0;
    (*queue)->front = 0;
    (*queue)->rear = 0;
    (*queue)->size = size;
    (*queue)->queue = malloc(sizeof(pointer) * size);
    for (i = 0; i < (*queue)->size; i++) {
        (*queue)->queue[i] = NULL;
    }
}

bool enQueue(Queue q, pointer item) {
    if (_isFull(q))
        return false;
    else {
        q->counter++;
        q->queue[q->rear] = item;
        q->rear = (q->rear + 1) % q->size;
    }
    return true;
}

pointer deQueue(Queue q) {
    pointer item = NULL;
    if (!_isEmpty(q)) {
        q->counter--;
        item = q->queue[q->front];
        q->front = (q->front + 1) % q->size;
    }
    return item;
}

void queueDestroy(Queue *queue) {
    assert(*queue != NULL);
    free((*queue)->queue);
    free(*queue);
}
