#include "queue.h"

#include <stddef.h>

void queue_init(queue *queue) {
    linked_list_init(&queue->list);
}

void queue_enqueue(queue *queue, void *data) {
    linked_list_insert_end(&queue->list, data);
}

void queue_dequeue(queue *queue) {
    linked_list_remove_beginning(&queue->list);
}
