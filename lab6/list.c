//
// Created by Антон Чемодуров on 31.10.2021.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"

List list_init(size_t element_size) {
    List list = {
            (Node*) malloc(sizeof(Node)),
            element_size,
            malloc(sizeof(size_t))
    };
    *list.list_size = 0;
    list.base->next = list.base->prev = NULL;
    return list;
}

int first_add(List list, void *val) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        return -1;
    }
    node->prev = node->next = NULL;
    list.base->next = list.base->prev = node;
    list.base->next->val = val;
    (*list.list_size)++;
    return 0;
}

int add_first_cpy(List list, void *val) {
    if (add_fist(list, val) == -1) return -1;
    char *buf = malloc(list.elem_size * sizeof(char));
    if (buf == NULL) return -1;
    memcpy(buf, val, list.elem_size);
    list.base->next->val = buf;
    return 0;
}

int add_last_cpy(List list, void *val) {
    if (add_last(list, val) == -1) return -1;
    char *buf = malloc(list.elem_size * sizeof(char));
    if (buf == NULL) return -1;
    memcpy(buf, val, list.elem_size);
    list.base->prev->val = buf;
    return 0;
}


int add_fist(List list, void *val) {
    if (is_empty(list)) {
        return first_add(list, val);
    }
    Node *tmp = malloc(sizeof(Node));
    if (tmp == NULL) {
        return -1;
    }
    Node *node = list.base->next;
    node->prev = tmp;
    list.base->next = tmp;
    tmp->next = node;
    tmp->prev = NULL;
    tmp->val = val;
    (*list.list_size)++;
    return 0;
}

int add_last(List list, void *val) {
    if (is_empty(list)) {
        return first_add(list, val);
    }
    Node *tmp = (Node *) malloc(sizeof(Node));
    if (tmp == NULL) {
        return -1;
    }
    Node *node = list.base->prev;
    node->next = tmp;
    list.base->prev = tmp;
    tmp->next = NULL;
    tmp->prev = node;
    tmp->val = val;
    (*list.list_size)++;
    return 0;
}

void *peek_fist(List list) {
    if (is_empty(list)) return NULL;
    return list.base->next->val;
}

void *peek_last(List list) {
    if (is_empty(list)) return NULL;
    return list.base->prev->val;
}

void *pop_fist(List list) {
    Node *node = list.base->next;
    list.base->next = node->next;
    if (node->next) node->next->prev = NULL;

    void *val = node->val;
    node->next = node->prev = NULL;
    free(node);
    (*list.list_size)--;
    return val;
}

void *pop_last(List list) {
    Node *node = list.base->prev;
    list.base->prev = node->prev;
    if (node->prev) node->prev->next = NULL;

    void *val = node->val;
    node->next = node->prev = NULL;
    free(node);
    (*list.list_size)--;
    return val;
}

void delete(List list) {
    while (!is_empty(list)) {
        Node *node = pop_last(list);
        free(node);
    }
    free(list.list_size);
    free(list.base);
}

_Bool is_empty(List list) {
    return list.base->next == NULL || list.base->prev == NULL;
}
