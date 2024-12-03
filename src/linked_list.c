#include "linked_list.h"

#include <stdlib.h>

void list_node_init(list_node *node, void *data) {
    node->data = data;
    node->previous = NULL;
    node->next = NULL;
}

void linked_list_init(linked_list *list) {
    list->head = NULL;
    list->tail = NULL;
}

void linked_list_insert_beginning(linked_list *list, void *data) {
    list_node *new_node = malloc(sizeof(list_node));
    list_node_init(new_node, data);

    new_node->next = list->head;

    if (list->head != NULL) {
        list->head->previous = new_node;
    } else {
        list->tail = new_node;
    }

    list->head = new_node;
}

void linked_list_insert_end(linked_list *list, void *data) {
    list_node *new_node = malloc(sizeof(list_node));
    list_node_init(new_node, data);

    new_node->previous = list->tail;

    if (list->tail != NULL) {
        list->tail->next = new_node;
    } else {
        list->head = new_node;
    }

    list->tail = new_node;
}

void linked_list_insert(linked_list *list, void *data, int index) {
    if (index == 0) {
        linked_list_insert_beginning(list, data);
        return;
    }

    list_node *new_node = malloc(sizeof(list_node));
    list_node_init(new_node, data);

    list_node *node = list->head;

    for (int i = 0; i < index; i++) {
        node = node->next;
    }

    if (node == NULL) {
        linked_list_insert_end(list, data);
        return;
    }

    new_node->previous = node->previous;
    new_node->next = node;
    node->previous->next = new_node;
    node->previous = new_node;
}

void linked_list_remove_beginning(linked_list *list) {
    if (list->head == NULL) {
        return;
    }

    list_node *removed_node = list->head;
    list->head = list->head->next;

    if (list->head != NULL) {
        list->head->previous = NULL;
    } else {
        list->tail = NULL;
    }

    free(removed_node);
}

void linked_list_remove_end(linked_list *list) {
    if (list->head == NULL) {
        return;
    }

    list_node *removed_node = list->tail;
    list->tail = list->tail->previous;

    if (list->tail != NULL) {
        list->tail->next = NULL;
    } else {
        list->head = NULL;
    }

    free(removed_node);
}

void linked_list_remove(linked_list *list, int index) {
    if (list->head == NULL) {
        return;
    }

    list_node *removed_node = list->head;

    for (int i = 0; i < index; i++) {
        removed_node = removed_node->next;
    }

    removed_node->previous->next = removed_node->next;
    removed_node->next->previous = removed_node->previous;

    free(removed_node);
}

void linked_list_destroy(linked_list *list) {
    list_node *node = list->head;

    while (node != NULL) {
        list_node *next_node = node->next;
        free(node);
        node = next_node;
    }

    list->head = NULL;
    list->tail = NULL;
}
