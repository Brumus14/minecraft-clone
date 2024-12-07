#include "safe_linked_list.h"

void safe_linked_list_init(safe_linked_list *list) {
    linked_list_init(&list->list);

    pthread_mutex_init(&list->mutex, NULL);
}

void safe_linked_list_insert_beginning(safe_linked_list *list, void *data) {
    pthread_mutex_lock(&list->mutex);
    linked_list_insert_beginning(&list->list, data);
    pthread_mutex_unlock(&list->mutex);
}

void safe_linked_list_insert_end(safe_linked_list *list, void *data) {
    pthread_mutex_lock(&list->mutex);
    linked_list_insert_end(&list->list, data);
    pthread_mutex_unlock(&list->mutex);
}

void safe_linked_list_insert(safe_linked_list *list, void *data, int index) {
    pthread_mutex_lock(&list->mutex);
    linked_list_insert(&list->list, data, index);
    pthread_mutex_unlock(&list->mutex);
}

void *safe_linked_list_remove_beginning(safe_linked_list *list) {
    pthread_mutex_lock(&list->mutex);
    void *removed_data = linked_list_remove_beginning(&list->list);
    pthread_mutex_unlock(&list->mutex);

    return removed_data;
}

void *safe_linked_list_remove_end(safe_linked_list *list) {
    pthread_mutex_lock(&list->mutex);
    void *removed_data = linked_list_remove_end(&list->list);
    pthread_mutex_unlock(&list->mutex);

    return removed_data;
}

void *safe_linked_list_remove(safe_linked_list *list, int index) {
    pthread_mutex_lock(&list->mutex);
    void *removed_data = linked_list_remove(&list->list, index);
    pthread_mutex_unlock(&list->mutex);

    return removed_data;
}

void safe_linked_list_destroy(safe_linked_list *list) {
    pthread_mutex_lock(&list->mutex);
    linked_list_destroy(&list->list);
    pthread_mutex_unlock(&list->mutex);
}

int safe_linked_list_length(safe_linked_list *list) {
    pthread_mutex_lock(&list->mutex);
    int length = linked_list_length(&list->list);
    pthread_mutex_unlock(&list->mutex);

    return length;
}

void *safe_linked_list_get(safe_linked_list *list, int index) {
    pthread_mutex_lock(&list->mutex);
    void *data = linked_list_get(&list->list, index);
    pthread_mutex_unlock(&list->mutex);

    return data;
}

bool safe_linked_list_is_empty(safe_linked_list *list) {
    pthread_mutex_lock(&list->mutex);
    bool empty = linked_list_is_empty(&list->list);
    pthread_mutex_unlock(&list->mutex);

    return empty;
}

int safe_linked_list_find(safe_linked_list *list, void *data) {
    pthread_mutex_lock(&list->mutex);
    int index = linked_list_find(&list->list, data);
    pthread_mutex_unlock(&list->mutex);

    return index;
}
