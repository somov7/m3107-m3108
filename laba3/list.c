//
// Created by Антон Чемодуров on 31.10.2021.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"

List init(size_t element_size) {
    List list = {
            (Node*) malloc(sizeof(Node)),
            element_size
    };
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
    return val;
}

void *pop_last(List list) {
    Node *node = list.base->prev;
    list.base->prev = node->prev;
    if (node->prev) node->prev->next = NULL;

    void *val = node->val;
    node->next = node->prev = NULL;
    free(node);
    return val;
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
    return 0;
}

_Bool is_empty(List list) {
    return list.base->next == NULL || list.base->prev == NULL;
}
