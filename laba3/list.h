//
// Created by Антон Чемодуров on 31.10.2021.
//

#include <stddef.h>

#ifndef LABA3_LIST_H
#define LABA3_LIST_H

typedef struct Node {
    struct Node *next;
    struct Node *prev;
    void *val;
}Node;

typedef struct{
    Node *base;
    size_t elem_size;
    size_t *list_size;
}List;

List init(size_t element_size);
void delete(List list);
_Bool is_empty(List list);
int add_last(List list, void *val);
int add_fist(List list, void *val);
int add_first_cpy(List list, void *val);
int add_last_cpy(List list, void *val);
void *peek_fist(List list);
void *peek_last(List list);
void *pop_fist(List list);
void *pop_last(List list);

#endif //LABA3_LIST_H