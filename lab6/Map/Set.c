//
// Created by Антон Чемодуров on 16.12.2021.
//
#include <string.h>
#include "Set.h"
#include "prime.h"

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

Set *set_init(uint64_t start_size) {
    Set *set = malloc(sizeof(Set));
    set->table = malloc(sizeof(busket_set)*start_size);
    for (uint64_t i = 0; i < start_size; i++) {
        set->table[i].key = NULL;
        set->table[i].next = 0;
        set->table[i].prev = 0;
    }
    set->h1 = generate_hash();
    set->h2 = generate_hash();
    set->m = start_size;
    set->elements = 0;
    set->last_add = set->m + 1;
    set->first_add = set->m + 1;
    set->deleted = 0;
    return set;
}


void set_rehashing(Set **set) {
    Set *s = *set;
    uint64_t size = s->m;
    if ((s->elements + s->deleted)* 2 > s->m) {
        size = get_prime(3*s->m, 2*s->m);
    }
    Set *new = set_init(size);
    uint64_t i = s->first_add;
    while (i < s->m) {
        set_add(&new, s->table[i].key);
        i = s->table[i].next;
    }
    free(s);
    *set = new;
}

char set_exist(Set *s, char *k) {
    uint64_t x = hash(k, s->m, s->h1);
    uint64_t y = hash(k, s->m, s->h2);
    for (int i = 0; i < s->m; ++i) {
        if (s->table[x].key == NULL) return 0;
        if (s->table[x].next != UINT64_MAX && strcmp(s->table[x].key, k) == 0) return 1;
        x = (x + y) % s->m;
    }
    return 0;
}

void set_add(Set **set, char *k) {
    if (set_exist(*set, k)) return;
    Set *s = *set;
    uint64_t x = hash(k, s->m, s->h1);
    uint64_t y = hash(k, s->m, s->h2);
    busket_set *table = s->table;
    for (int i = 0; i < s->m; i++) {
        if (table[x].key == NULL || table[x].next == UINT64_MAX) {
            table[x].key = k;
            table[x].prev = s->last_add;
            table[x].next = s->m + 1;
            if (s->last_add < s->m) {
                table[s->last_add].next = x;
            }
            if (s->first_add > s->m) {
                s->first_add = x;
            }
            s->last_add = x;
            s->elements++;
            return;
        }
        x = (x + y) % s->m;
    }
    //printf("REHASHING %d\n", k);
    set_rehashing(set);
    set_add(set, k);
}

void set_delete(Set *s, char *k) {
    uint64_t x = hash(k, s->m, s->h1);
    uint64_t y = hash(k, s->m, s->h2);
    busket_set *table = s->table;
    for (int i = 0; i < s->m; ++i) {
        if (table[x].key == NULL) return;
        if (strcmp(table[x].key, k) == 0) {
            free(table[x].key);
            if (table[x].prev > s->m) {
                s->first_add = table[x].next;
            } else {
                table[table[x].prev].next = table[x].next;
            }
            if (table[x].next > s->m) {
                s->last_add = table[x].prev;
            } else {
                table[table[x].next].prev = table[x].prev;
            }
            table[x].next = UINT64_MAX;
            s->elements--;
            s->deleted++;
        }
        x = (x + y) % s->m;
    }
}

char **set_getall(Set *s) {
    char **elemes = malloc(sizeof(char *) * s->elements);
    uint64_t i = s->first_add;
    uint64_t j = 0;
    while (i < s->m) {
        elemes[j++] = s->table[i].key;
        i = s->table[i].next;
    }
    return elemes;
}

uint64_t set_get_size(Set *set) {
    return set->elements;
}

void set_free(Set *set) {
    for (int i = 0; i < set->elements; i++) {
        free(set->table[i].key);
    }
    free(set->table);
    free(set);
}
