//
// Created by Антон Чемодуров on 16.12.2021.
//

#ifndef ARCHIVER_SET_H
#define ARCHIVER_SET_H
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    uint64_t k;
    uint64_t p;
}Hash;

typedef struct {
    char *key;
    uint64_t prev;
    uint64_t next;
}busket_set;


typedef struct {
    busket_set *table;
    Hash h1;
    Hash h2;
    uint64_t m;
    uint64_t elements;
    uint64_t deleted;
    uint64_t last_add;
    uint64_t first_add;
}Set;

Set *set_init(uint64_t start_size);
char set_exist(Set *s, char *k);
void set_add(Set **set, char *k);
void set_delete(Set *s, char *k);
char **set_getall(Set *s);
uint64_t set_get_size(Set *set);
void set_free(Set *set);
#endif //ARCHIVER_SET_H
