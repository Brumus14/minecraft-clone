#ifndef SAFE_LINKED_LIST_H
#define SAFE_LINKED_LIST_H

#include <pthread.h>
#include "stdbool.h"
#include "linked_list.h"

typedef struct list_node list_node;

typedef struct safe_linked_list {
    linked_list list;
    pthread_mutex_t mutex;
} safe_linked_list;

void safe_linked_list_init(safe_linked_list *list);
void safe_linked_list_insert_beginning(safe_linked_list *list, void *data);
void safe_linked_list_insert_end(safe_linked_list *list, void *data);
void safe_linked_list_insert(safe_linked_list *list, void *data, int index);
void *safe_linked_list_remove_beginning(safe_linked_list *list);
void *safe_linked_list_remove_end(safe_linked_list *list);
void *safe_linked_list_remove(safe_linked_list *list, int index);
void safe_linked_list_destroy(safe_linked_list *list);
int safe_linked_list_length(safe_linked_list *list);
void *safe_linked_list_get(safe_linked_list *list, int index);
bool safe_linked_list_is_empty(safe_linked_list *list);
int safe_linked_list_find(safe_linked_list *list, void *data);

#endif
