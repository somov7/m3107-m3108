|      	../main.c#include <stdio.h>
#include <string.h>
#include "archiver.h"

int main() {
    char *argv[] = {"../prog.arc", "../main.c",
                    "../list.c", "../archiver.c",
                    "../list.h", "../archiver.h"};
    int argc = sizeof(argv) / sizeof(argv[0]);
    Archive *archive = arc_create(argv[0]);
    for (int i = 1; i < argc; i++) {
        if (arc_add(archive, argv[i]) == -1) {
            printf("ERROR\n");
        }
    }
//    Archive *arc = arc_open("../prog.arc");
//    char **name = set_getall(arc->file_names);
//    for (int i = 0; i < arc->file_names->elements; i++) {
//        puts(name[i]);
//    }
}
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
%
      ../archiver.c//
// Created by Антон Чемодуров on 14.11.2021.
//

#include "archiver.h"
#include "list.h"
#include <string.h>
#include "Map/prime.h"

Archive *init(const char *arc_name, const char *mod) {
    FILE *arc_file = fopen(arc_name, mod);
    if (arc_file == NULL) return NULL;

    Archive *arc = malloc(sizeof(Archive));
    if (arc == NULL)  {
        fclose(arc_file);
        return NULL;
    }

    arc->arc = arc_file;
    arc->file_names = set_init(get_prime(1000, 100));
    if (arc->file_names == NULL) {
        fclose(arc_file);
        return NULL;
    }
    return arc;
}

Archive *arc_create(char *arc_name) {
    return init(arc_name, "wb");
}

long get_file_size(FILE *file) {
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);
    return file_size;
}

Archive *arc_open(const char *arc_name) {
    Archive *arc = init(arc_name, "rb");
    if (arc == NULL) return NULL;
    long i = 0;
    FILE *in  = arc->arc;
    char name_size;
    long file_size = get_file_size(in);
    long read = 0;
    while (!feof(in) && read < file_size) {
        if (fread(&i, sizeof(long), 1, in) != 1) goto error;

        if (fread(&name_size, 1, 1, in) != 1) goto error;
        char *file_name = malloc(name_size + 1);
        fread(file_name, 1, name_size, in);
        file_name[name_size] = 0;

        if (set_exist(arc->file_names, file_name)) goto error;
        set_add(&arc->file_names, file_name);
        fseek(in, i, SEEK_CUR);
        read += i + name_size + sizeof(char) + sizeof(long);
    }
    return arc;
    error:
        arc_close(arc);
        return NULL;
}

long arc_add(Archive *arc, char* file_name) {
    if (set_exist(arc->file_names, file_name)) return -1;

    FILE *in = fopen(file_name, "rb");
    FILE *out = arc->arc;
    if (in == NULL) {
        perror(file_name);
    }

    long file_size = get_file_size(in);

    if (fwrite(&file_size, sizeof(long), 1, out) != 1) {
        goto error;
    }
    size_t file_name_len = strlen(file_name);
    if (file_name_len > 255) goto error;
    fwrite(&file_name_len, 1, 1, out);
    if (fwrite(file_name, sizeof(char), file_name_len, out) != file_name_len) {
        goto error;
    }

    unsigned long read;
    const int bsize = 4096;
    char buff[4096];
    while (!feof(in) && (read = fread(buff, 1, bsize, in)) > 0) {
        if(fwrite(buff, 1, read, out) != read) goto error;
    }
    set_add(&arc->file_names, file_name);
    return file_size;

    error:
        fclose(in);
        return -1;
}

void arc_close(Archive *arc) {
    fclose(arc->arc);
    free(arc);
}�      	../list.h//
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

#endif //LABA3_LIST_H�      ../archiver.h//
// Created by Антон Чемодуров on 14.11.2021.
//

#ifndef ARCHIVER_ARCHIVER_H
#define ARCHIVER_ARCHIVER_H

#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "Map/Set.h"

#define MAX_NAME_SIZE 256

typedef struct Archive {
    FILE* arc;
    Set *file_names;
}Archive;

long arc_add(Archive *arc, char* file_name);
Archive *arc_create(char *arc_name);
void arc_close(Archive *arc);
Archive *arc_open(const char *arc_name);

#endif //ARCHIVER_ARCHIVER_H
