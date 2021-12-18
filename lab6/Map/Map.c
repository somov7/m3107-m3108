//
// Created by Антон Чемодуров on 09.12.2021.
//

#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "Map.h"
#include "prime.h"

void rehashing(Map *map, CMP);

Hash generate_hash() {
    Hash h = {get_prime(100000, 10000), get_prime(100000, 10000)};
    return h;
}

uint64_t hash(char *key, uint64_t m, Hash h) {
    uint64_t ha =0, mod = 1;
    for (int i = 0; key[i]; ++i) {
        ha = (ha + key[i] * mod) % h.p;
        mod = (mod*h.k) % h.p;
    }
    return ha % m;

}

void table_init(basket *table, size_t size) {
    for (int i = 0; i < size; ++i) {
        table[i] = (basket){NULL, NULL, 0, 1, -1};
    }
}

Map *map_create(size_t start_size, size_t key_size, size_t value_size) {
    Map *map = malloc(sizeof(Map));
    if (map == NULL) return NULL;
    map->size = start_size;
    map->elements = 0;
    map->key_s = key_size;
    map->val_s = value_size;
    map->h1 = generate_hash();
    map->h2 = generate_hash();
    map->first_added = -1;
    map->last_added = -1;
    map->table = malloc(start_size * sizeof(basket));
    table_init(map->table, start_size);
    return map;
}

void map_put(Map *map, void *key, void *val, CMP) {
    uint64_t x = hash(key, map->size, map->h1);
    uint64_t y = hash(key, map->size, map->h2);
    basket *table = map->table;
    for (int i = 0; i < map->size; i++) {
        if (table[x].unused || table[x].deleted) {
            table[x] = (basket){key, val, 0, 0, -1};

            if (map->last_added != -1) {
                table[map->last_added].next = x;
            }
            if (map->first_added == -1) {
                map->first_added = x;
                map->last_added = x;
            }
            map->elements++;
            return;
        }
        if (cmp(table[x].key, key) == 0) {
            free(table[x].val);
            table[x].val = val;
            return;
        }
        x = (x + y) % map->size;
    }
    rehashing(map, cmp);
    map_put(map, key, val, cmp);
}

void rehashing(Map *map, CMP) {
    size_t size = map->size;
    if (map->elements * 2 >= map->size) {
        size = get_prime(2*map->size, map->size);
    }
    basket *table = map->table;
    map->table = malloc(sizeof(basket) * size);
    table_init(map->table, size);
    size_t old_size = map->size;
    map->size = size;
    map->h1 = generate_hash();
    map->h2 = generate_hash();
    for (int i = 0; i < old_size; i++) {
        if (table[i].unused && !table[i].deleted) {
            map_put(map, table[i].key, table[i].val, cmp);
        }
    }
}

void map_remove(Map *map, void *key, CMP) {
    uint64_t x = hash(key, map->size, map->h1);
    uint64_t y = hash(key, map->size, map->h2);
    basket *table = map->table;
    for (int i = 0; i < map->size; i++) {
        if (table[x].unused) return;
        if (!table[x].deleted && cmp(table[x].key, key) == 0) {
            table[x].deleted = 1;
            free(table[x].key);
            free(table[x].val);
            table[x].key = NULL;
            table[x].val = NULL;
            return;
        }
        x = (x + y) % map->size;
    }
}

void *map_get(Map *map, void *key, CMP) {
    uint64_t x = hash(key, map->size, map->h1);
    uint64_t y = hash(key, map->size, map->h2);
    basket *table = map->table;
    for (int i = 0; i < map->size; i++) {
        if (table[x].unused) return NULL;
        if (!table[x].deleted && cmp(table[x].key, key) == 0) {
            return table[x].val;
        }
        x = (x + y) % map->size;
    }
}

void *map_getall(Map *map) {
    uint64_t i = map->first_added;
    while (i > 0) {

    }
}