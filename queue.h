#ifndef QUEUE_H
#define QUEUE_H

typedef struct Queue *Queue;

void queueCreate(Queue *queue, unsigned long int size);

bool enQueue(Queue q, void *item);

void *deQueue(Queue q);

void queueDestroy(Queue *queue);

#endif
