#include "queue.h"

#include <stddef.h>

void queue_init(queue *queue, unsigned long data_size) {
    linked_list_init(&queue->list, data_size);
}

void queue_destroy(queue *queue) {
    while (!queue_is_empty(queue)) {
        queue_dequeue(queue);
    }
}

void queue_enqueue(queue *queue, void *data) {
    linked_list_insert_end(&queue->list, data);
}

void *queue_dequeue(queue *queue) {
    return linked_list_remove_beginning(&queue->list);
}

bool queue_is_empty(queue *queue) {
    return linked_list_is_empty(&queue->list);
}

unsigned int queue_length(queue *queue) {
    return linked_list_length(&queue->list);
}

void *queue_get(queue *queue, int index) {
    return linked_list_get(&queue->list, index);
}

unsigned int queue_find(queue *queue, void *data) {
    return linked_list_find(&queue->list, data);
}
