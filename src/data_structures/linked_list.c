#include "linked_list.h"

#include <stdlib.h>
#include <string.h>

void list_node_init(list_node *node, void *data) {
    node->data = data;
    node->previous = NULL;
    node->next = NULL;
}

void linked_list_init(linked_list *list, unsigned long data_size) {
    list->head = NULL;
    list->tail = NULL;
    list->data_size = data_size;
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
    int list_length = linked_list_length(list);

    if (index > list_length) {
        return;
    }

    if (index == 0) {
        linked_list_insert_beginning(list, data);
        return;
    } else if (index == list_length) {
        linked_list_insert_end(list, data);
    } else {
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
}

void *linked_list_remove_beginning(linked_list *list) {
    if (list->head == NULL) {
        return NULL;
    }

    list_node *removed_node = list->head;
    list->head = list->head->next;

    if (list->head != NULL) {
        list->head->previous = NULL;
    } else {
        list->tail = NULL;
    }

    void *removed_data = removed_node->data;

    free(removed_node);

    return removed_data;
}

void *linked_list_remove_end(linked_list *list) {
    if (list->head == NULL) {
        return NULL;
    }

    list_node *removed_node = list->tail;
    list->tail = list->tail->previous;

    if (list->tail != NULL) {
        list->tail->next = NULL;
    } else {
        list->head = NULL;
    }

    void *removed_data = removed_node->data;

    free(removed_node);

    return removed_data;
}

void *linked_list_remove(linked_list *list, int index) {
    int list_length = linked_list_length(list);

    if (index >= list_length) {
        return NULL;
    }

    if (index == 0) {
        return linked_list_remove_beginning(list);
    } else if (index == list_length - 1) {
        return linked_list_remove_end(list);
    } else {
        list_node *removed_node = list->head;

        for (int i = 0; i < index; i++) {
            removed_node = removed_node->next;
        }

        removed_node->previous->next = removed_node->next;
        removed_node->next->previous = removed_node->previous;

        void *removed_data = removed_node->data;

        free(removed_node);

        return removed_data;
    }
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

unsigned int linked_list_length(linked_list *list) {
    int length = 0;
    list_node *current_node = list->head;

    while (current_node != NULL) {
        length++;
        current_node = current_node->next;
    }

    return length;
}

void *linked_list_get(linked_list *list, int index) {
    if (index >= linked_list_length(list)) {
        return NULL;
    }

    list_node *current_node = list->head;

    for (int i = 0; i < index; i++) {
        current_node = current_node->next;
    }

    return current_node->data;
}

bool linked_list_is_empty(linked_list *list) {
    return (list->head == NULL);
}

unsigned int linked_list_find(linked_list *list, void *data) {
    list_node *current_node = list->head;
    int index = 0;

    while (current_node != list->tail &&
           memcmp(current_node->data, data, list->data_size) == 0) {
        current_node = current_node->next;
        index++;
    }

    if (current_node == list->tail) {
        return -1;
    }

    return index;
}
