#ifndef QUEUE_H
#define QUEUE_H

#include "linked_list.h"

typedef struct queue {
    linked_list list;
} queue;

void queue_init(queue *queue);
void queue_enqueue(queue *queue, void *data);
void *queue_dequeue(queue *queue);
bool queue_is_empty(queue *queue);
int queue_length(queue *queue);
void *queue_get(queue *queue, int index);
int queue_find(queue *queue, void *data);

#endif
