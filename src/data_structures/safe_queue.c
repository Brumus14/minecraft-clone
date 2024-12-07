#include "safe_queue.h"

void safe_queue_init(safe_queue *queue) {
    safe_linked_list_init(&queue->list);
}

void safe_queue_enqueue(safe_queue *queue, void *data) {
    safe_linked_list_insert_end(&queue->list, data);
}

void *safe_queue_dequeue(safe_queue *queue) {
    return safe_linked_list_remove_beginning(&queue->list);
}

bool safe_queue_is_empty(safe_queue *queue) {
    return safe_linked_list_is_empty(&queue->list);
}

int safe_queue_length(safe_queue *queue) {
    return safe_linked_list_length(&queue->list);
}

void *safe_queue_get(safe_queue *queue, int index) {
    return safe_linked_list_get(&queue->list, index);
}

int safe_queue_find(safe_queue *queue, void *data) {
    return safe_linked_list_find(&queue->list, data);
}
