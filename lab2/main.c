#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int BASE = 1000000000;

typedef struct uint1024 {
    int* number;
    size_t size;
} uint1024_t;

uint1024_t from_uint(unsigned int x) {
    uint1024_t y;
    int size;
    if (x >= BASE) {
        size = 2;
    } else {
        size = 1;
    }
    y.number = malloc(size * sizeof(int));
    y.size = size;
    y.number[0] = x % BASE;
    if (size > 1) {
        y.number[1] = x / BASE;
    }
    return y;
}

uint1024_t add_op(uint1024_t x, uint1024_t y) {
    uint1024_t resultAdd;
    short carry = 0;
    for (int i = 0; i < 2; ++i) {
        resultAdd.str[i] = x.str[i] + y.str[i] + carry;
        carry = 0;
        if (x.str[i] + y.str[i] > 255) {
            carry = 1;
        }
    }
    return resultAdd;
}

uint1024_t subtr_op(uint1024_t x, uint1024_t y) {
    uint1024_t resultSubtr = {0};
    short carry;
    for (int i = 0; i < 128; ++i) {
        resultSubtr.str[i] = x.str[i] - y.str[i] - carry;
        carry = 0;
        if (x.str[i] - y.str[i] < 0) {
            carry = 1;
        }
    }
    return resultSubtr;
}

int main() {
    unsigned int x = 4222222222;
    unsigned int y = 222922222;
    uint1024_t x_ = from_uint(x);
    uint1024_t y_ = from_uint(y);
    uint1024_t zAdd = add_op(x_, x_);
    uint1024_t zSubstr = subtr_op(x_, y_);
}
