#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#define BASE 1000000000

typedef struct uint1024_t {
    uint32_t *number;
    size_t size;
} uint1024_t;

uint1024_t from_uint(unsigned int x) {
    uint1024_t uint1024;
    if (x < BASE) {
        uint1024.size = 1;
        uint1024.number = malloc(uint1024.size * sizeof(uint32_t));
        uint1024.number[0] = x;
    } else {
        uint1024.size = 2;
        uint1024.number = malloc(uint1024.size * sizeof(uint32_t));
        uint1024.number[0] = x % BASE;
        uint1024.number[1] = x / BASE;
    }
    return uint1024;
}

void scanf_value(uint1024_t *x) {
    char str[1024];
    scanf("%s", str);
    size_t length = strlen(str);
    uint1024_t uint1024;
    if (length % 9 == 0) {
        uint1024.size = length / 9;
    } else {
        uint1024.size = length / 9 + 1;
    }
    uint1024.number = malloc(uint1024.size * sizeof(uint32_t));

    for (int i = length, j = 0; i > 0; i -= 9, j++) {
        str[i] = '\0';
        if (i >= 9) {
            uint1024.number[j] = atoi(str + i - 9);
        } else {
            uint1024.number[j] = atoi(str);
        }
    }

    *x = uint1024;
}

uint1024_t add_op(uint1024_t x, uint1024_t y) {
    int carry = 0, size;
    if (x.size > y.size) {
        size = x.size;
    } else {
        size = y.size;
    }
    uint1024_t result;
    result.size = size;
    result.number = malloc(result.size * sizeof(uint32_t));
    for (int i = 0; i < size; ++i) {
        result.number[i] = x.number[i] + carry;
        if (i < y.size) {
            result.number[i] = x.number[i] + y.number[i];
        }
        carry = result.number[i] / BASE;
        if (carry) {
            result.number[i] -= BASE;
        }
    }
    return result;
}

uint1024_t subtr_op(uint1024_t x, uint1024_t y) {
    uint1024_t result;
    result.size = x.size;
    result.number = malloc(result.size * sizeof(uint32_t));
    uint8_t carry = 0;
    for (int i = 0; i < y.size; i++) {
        result.number[i] = x.number[i] - carry;
        if (i < y.size) {
            result.number[i] -= y.number[i];
        }
        carry = result.number[i] < 0;
        if (carry) {
            result.number[i] += BASE;
        }
    }
    while (result.number[result.size - 1] == 0 && result.size > 1) {
        result.size--;
    }
    result.number = realloc(result.number, result.size * sizeof(uint32_t));
    return result;
}

uint1024_t mult_op(uint1024_t x, uint1024_t y) {
    uint1024_t result;
    uint32_t size = x.size + y.size;
    result.number = calloc(size, sizeof(uint32_t));
    result.size = size;
    int carry = 0;
    for (int i = 0; i < x.size; i++)
        for (int j = 0; j < y.size; j++) {
            uint64_t cur = result.number[i + j];
            if (i < y.size) {
                cur += x.number[i] * (uint64_t) y.number[j];
            }
            cur += carry;
            result.number[i + j] = cur % BASE;
            carry =  cur / BASE;
        }

    while (result.number[result.size - 1] == 0 && result.size > 1) {
        result.size--;
    }
    result.number = realloc(result.number, result.size * sizeof(uint32_t));
    return result;
}

void printf_value(uint1024_t x) {
    if (x.size == 0) {
        printf("null\n");
        return;
    }
    int i = x.size - 1;
    for (i; i > 0; i--) {
        if (x.number[i] != 0) {
            break;
        }
    }
    printf("%u", x.number[i]);
    for (i--; i != -1; i--) {
        printf("%.9u", x.number[i]);
    }
    putchar('\n');
}

int main() {
    uint1024_t x, y, c;
    printf("Enter number 1\n");
    scanf_value(&x);
    printf("Enter number 2\n");
    scanf_value(&y);

    printf("add: ");
    c = add_op(x, y);
    printf_value(c);

    printf("subtraction: ");
    c = subtr_op(x, y);
    printf_value(c);

    printf("multiplication: ");
    c = mult_op(x, y);
    printf_value(c);
    return 0;
}

// 999999999999
// 9999999