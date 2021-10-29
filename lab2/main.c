#include <stdio.h>
#include <string.h>

typedef struct uint1024 {
    unsigned char str[128];
} uint1024_t;

uint1024_t from_uint(unsigned int x) {
    uint1024_t y;
    for (int i = 0; i < 128; ++i) {
        y.str[i] = x % 256;
        x = x / 256;
    }
    return y;
}

uint1024_t add_op(uint1024_t x, uint1024_t y) {
    uint1024_t resultAdd = {0};
    short carry = 0;
    for (int i = 0; i < 128; ++i) {
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
