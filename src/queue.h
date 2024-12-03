#ifndef QUEUE_H
#define QUEUE_H

#include "linked_list.h"

typedef struct queue {
    linked_list list;
} queue;

void queue_init(queue *queue);
void queue_enqueue(queue *queue, void *data);
void queue_dequeue(queue *queue);

#endif
