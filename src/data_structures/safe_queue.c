#include "safe_queue.h"
#include "queue.h"
#include <pthread.h>

void safe_queue_init(safe_queue *queue) {
    queue_init(&queue->queue);
    pthread_mutex_init(&queue->mutex, NULL);
}

void safe_queue_destroy(safe_queue *queue) {
    queue_destroy(&queue->queue);
    pthread_mutex_destroy(&queue->mutex);
}

void safe_queue_enqueue(safe_queue *queue, void *data) {
    pthread_mutex_lock(&queue->mutex);
    queue_enqueue(&queue->queue, data);
    pthread_mutex_unlock(&queue->mutex);
}

void *safe_queue_dequeue(safe_queue *queue) {
    pthread_mutex_lock(&queue->mutex);
    void *item = queue_dequeue(&queue->queue);
    pthread_mutex_unlock(&queue->mutex);

    return item;
}

bool safe_queue_is_empty(safe_queue *queue) {
    pthread_mutex_lock(&queue->mutex);
    bool empty = queue_is_empty(&queue->queue);
    pthread_mutex_unlock(&queue->mutex);

    return empty;
}

int safe_queue_length(safe_queue *queue) {
    pthread_mutex_lock(&queue->mutex);
    int length = queue_length(&queue->queue);
    pthread_mutex_unlock(&queue->mutex);

    return length;
}

void *safe_queue_get(safe_queue *queue, int index) {
    pthread_mutex_lock(&queue->mutex);
    void *item = queue_get(&queue->queue, index);
    pthread_mutex_unlock(&queue->mutex);

    return item;
}

int safe_queue_find(safe_queue *queue, void *data) {
    pthread_mutex_lock(&queue->mutex);
    int index = queue_find(&queue->queue, data);
    pthread_mutex_unlock(&queue->mutex);

    return index;
}
