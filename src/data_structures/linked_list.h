#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "stdbool.h"

typedef struct list_node list_node;

typedef struct list_node {
    void *data;
    list_node *previous;
    list_node *next;
} list_node;

void list_node_init(list_node *node, void *data);

typedef struct linked_list {
    list_node *head;
    list_node *tail;
} linked_list;

void linked_list_init(linked_list *list);
void linked_list_insert_beginning(linked_list *list, void *data);
void linked_list_insert_end(linked_list *list, void *data);
void linked_list_insert(linked_list *list, void *data, int index);
void *linked_list_remove_beginning(linked_list *list);
void *linked_list_remove_end(linked_list *list);
void *linked_list_remove(linked_list *list, int index);
void linked_list_destroy(linked_list *list);
int linked_list_length(linked_list *list);
void *linked_list_get(linked_list *list, int index);
bool linked_list_is_empty(linked_list *list);
int linked_list_find(linked_list *list, void *data);

#endif
