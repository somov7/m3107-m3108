�      	../list.c//
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
�      	../list.h//
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

List list_init(size_t element_size);
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

#endif //LABA3_LIST_H�      	../main.c#include <stdio.h>
#include <string.h>
#include "archiver.h"
#include <getopt.h>

void using() {

}

int main(int argc, char **argv) {
   if (argc < 2) {
       puts("too few args given");
       using();
       exit(EXIT_FAILURE);
   }
   Archive *arc = NULL;
   char *arc_name = NULL;
   int i = 1;
    while (i < argc) {
       if (argv[i][2] == 'f') {
            arc_name = argv[++i];
       } else if (argv[i][2] == 'c') {
           arc = arc_create(arc_name);
           i++;
           while (i < argc) {
               arc_add(arc, argv[i++]);
           }
       } else if (argv[i][2] == 'e') {
           arc_extract(arc_name);
       } else if (argv[i][2] == 'l') {
            arc = arc_open(arc_name);
            char **names = set_getall(arc->file_names);
            for (int i = 0; i < set_get_size(arc->file_names); i++) {
                printf("%d: %s\n", i+1, names[i]);
            }
       } else if (argv[i][2] == 'a') {
           arc = arc_open(arc_name);
           i++;
           while (i < argc) {
               if (arc_add(arc, argv[i++]) == -1) {
                   printf("Error\n");
               }
           }
       }
       i++;
   }
}
1      ../test23 69 6e 63 6c 75 64 65
20 3c 73 74 64 69 6f 2e 68 3e 0a 23 69 6e 63 6c
75 64 65 20 3c 73 74 72 69 6e 67 2e 68 3e 0a 23
69 6e 63 6c 75 64 65 20 22 61 72 63 68 69 76 65
72 2e 68 22 0a 0a 69 6e 74 20 6d 61 69 6e 28 29
20 7b 0a 20 20 20 20 63 68 61 72 20 2a 61 72 67
76 5b 5d 20 3d 20 7b 22 2e 2e 2f 70 72 6f 67 2e
61 72 63 22 2c 20 22 2e 2e 2f 6d 61 69 6e 2e 63
22 2c 20 22 2e 2e 2f 6c 69 73 74 2e 63 22 2c 20  |", "../list.c", |
22 2e 2e 2f 61 72 63 68 69 76 65 72 2e 63 22 2c  |"../archiver.c",|
20 22 2e 2e 2f 6c 69 73 74 2e 68 22 7d 3b 0a 20  | "../list.h"};. |
20 20 20 69 6e 74 20 61 72 67 63 20 3d 20 73 69  |   int argc = si|
7a 65 6f 66 28 61 72 67 76 29 20 2f 20 73 69 7a  |zeof(argv) / siz|
65 6f 66 28 61 72 67 76 5b 30 5d 29 3b 0a 20 20  |eof(argv[0]);.  |
20 20 41 72 63 68 69 76 65 20 61 72 63 68 69 76  |  Archive archiv|
65 20 3d 20 63 72 65 61 74 65 28 61 72 67 76 5b  |e = create(argv[|
30 5d 29 3b 0a 20 20 20 20 66 6f 72 20 28 69 6e  |0]);.    for (in|
74 20 69 20 3d 20 31 3b 20 69 20 3c 20 61 72 67  |t i = 1; i < arg|
63 3b 20 69 2b 2b 29 20 7b 0a 20 20 20 20 20 20  |c; i++) {.      |
20 20 61 64 64 28 26 61 72 63 68 69 76 65 2c 20  |  add(&archive, |
61 72 67 76 5b 69 5d 29 3b 0a 20 20 20 20 7d 0a  |argv[i]);.    }.|
20 20 20 20 63 6c 6f 73 65 28 26 61 72 63 68 69  |    close(&archi|
76 65 29 3b 0a 7d