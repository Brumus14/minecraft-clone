#ifndef SPMC_QUEUE_H
#define SPMC_QUEUE_H

// A SPMC (Single Producer, Multiple Consumer) queue
// Using a sentinel node to avoid edge cases

#include "stdbool.h"
#include <pthread.h>

typedef struct spmc_queue_node spmc_queue_node;

typedef struct spmc_queue_node {
    void *data;
    spmc_queue_node *next;
} spmc_queue_node;

void spmc_queue_node_init(spmc_queue_node *node, void *data);

typedef struct spmc_queue {
    spmc_queue_node *head;
    spmc_queue_node *tail;
    pthread_mutex_t head_mutex;
} spmc_queue;

void spmc_queue_init(spmc_queue *queue);
void spmc_queue_destroy(spmc_queue *queue);
void spmc_queue_enqueue(spmc_queue *queue, void *data);
void *spmc_queue_dequeue(spmc_queue *queue);
bool spmc_queue_is_empty(spmc_queue *queue);

#endif
