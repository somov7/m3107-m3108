//
//  uint1024_t.h
//  BigInteger
//
//  Created by Антон Чемодуров on 02.10.2021.
//

#ifndef uint1024_t_h
#define uint1024_t_h

#include <stdio.h>
#include <stdint.h>

#define MAX_SIZE 16

typedef struct {
    uint64_t nums[MAX_SIZE];
    uint8_t size;
} uint1024_t;

uint1024_t from_uint(uint64_t x);
uint1024_t from_uint1024(uint1024_t x);
uint1024_t from_str(const char* str);

uint1024_t add_op(uint1024_t x, uint1024_t y);
void add_op_ptr(uint1024_t *x, uint1024_t *y);

void subtr_op_ptr(uint1024_t *x, uint1024_t *y);
uint1024_t subtr_op(uint1024_t x, uint1024_t y);

uint1024_t mult_op(uint1024_t x, uint1024_t y);

uint1024_t div_uint1024_uint64(uint1024_t x, uint64_t y, uint64_t *mod);
uint1024_t l_shift(const uint1024_t *x, unsigned int t);

int uint1024_cmp(uint1024_t *x, uint1024_t *y);

void printf_value(uint1024_t x);
void scanf_value(uint1024_t* x);
void print(uint1024_t x);

char *toString(uint1024_t x);
#endif /* uint1024_t_h */
