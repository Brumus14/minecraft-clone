#ifndef SAFE_QUEUE_H
#define SAFE_QUEUE_H

#include "safe_linked_list.h"

typedef struct safe_queue {
    safe_linked_list list;
} safe_queue;

void safe_queue_init(safe_queue *queue);
void safe_queue_enqueue(safe_queue *queue, void *data);
void *safe_queue_dequeue(safe_queue *queue);
bool safe_queue_is_empty(safe_queue *queue);
int safe_queue_length(safe_queue *queue);
void *safe_queue_get(safe_queue *queue, int index);
int safe_queue_find(safe_queue *queue, void *data);

#endif
