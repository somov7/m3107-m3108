#ifndef UINT1024_T_UINT_H
#define UINT1024_T_UINT_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef struct uint1024_t {
    int *t;
    size_t size;
} uint1024_t;

uint1024_t from_uint(unsigned int x);

uint1024_t add_op(uint1024_t x, uint1024_t y);

uint1024_t subtr_op(uint1024_t x, uint1024_t y);

uint1024_t mult_op(uint1024_t x, uint1024_t y);

void printf_value(uint1024_t x);

void scanf_value(uint1024_t *x);

#endif