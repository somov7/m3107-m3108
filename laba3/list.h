//
// Created by Антон Чемодуров on 31.10.2021.
//

#include <stddef.h>

#ifndef LABA3_LIST_H
#define LABA3_LIST_H

#endif //LABA3_LIST_H

typedef struct Node {
    struct Node *next;
    struct Node *prev;
    void *val
}Node;

typedef struct{
    Node *base;
    size_t size;
}List;

List init(size_t element_size);
_Bool is_empty(List list);
int add_last(List list, void *val);
int add_fist(List list, void *val);
void *peek_fist(List list);
void *pop_fist(List list);
void *pop_last(List list);

