#ifndef SAFE_QUEUE_H
#define SAFE_QUEUE_H

#include "queue.h"
#include "stdbool.h"
#include <pthread.h>

typedef struct safe_queue {
    queue queue;
    pthread_mutex_t mutex;
} safe_queue;

void safe_queue_init(safe_queue *queue);
void safe_queue_destroy(safe_queue *queue);
void safe_queue_enqueue(safe_queue *queue, void *data);
void *safe_queue_dequeue(safe_queue *queue);
bool safe_queue_is_empty(safe_queue *queue);

#endif
