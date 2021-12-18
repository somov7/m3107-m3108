//
// Created by Антон Чемодуров on 12.12.2021.
//

#ifndef LAB5_MAP_H
#define LAB5_MAP_H

#include <string.h>
#include <stdlib.h>
#include <strhash.h>

typedef struct {
    uint64_t k;
    uint64_t p;
}Hash;

typedef struct {
    void *key;
    void *val;
    char deleted;
    char unused;
    uint64_t next;
}basket;

typedef struct {
    basket *table;
    size_t size;
    Hash h1;
    Hash h2;
    size_t elements;
    uint64_t first_added;
    uint64_t last_added;
    size_t key_s;
    size_t val_s;
}Map;

#define CMP int (*cmp) (const void *, const void *)

Map *map_create(size_t start_size, size_t key_size, size_t value_size);
void map_put(Map *map, void *key, void *val, CMP);
void map_remove(Map *map, void *key, CMP);
void *map_getall(Map* map);
void *map_get(Map *map, void *key, CMP);

#endif //LAB5_MAP_H
