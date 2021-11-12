#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define BASE 1000000000

typedef struct uint1024_t {
    uint32_t *number;
    size_t count;
    size_t size;
} uint1024_t;

static void swap(uint8_t *array, size_t size) {
    uint8_t temp;
    for (size_t i = 0, j = size - 1; i < size / 2; i++, j--) {
        temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

static void to_one_size(uint1024_t *x, uint1024_t *y) {
    if (x->size < y->size) {
        x->size = y->size;
        x->number = (uint32_t *) realloc(x->number, x->size * sizeof(uint32_t));
        memset(x->number + x->count, 0, (x->size - x->count) * sizeof(uint32_t));
    }
    if (y->size < x->size) {
        y->size = x->size;
        y->number = (uint32_t *) realloc(y->number, y->size * sizeof(uint32_t));
        memset(y->number + y->count, 0, (y->size - y->count) * sizeof(uint32_t));
    }
    if (x->count < y->count) {
        x->count = y->count;
    }
}

void printf_value(uint1024_t x) {
    ssize_t i = x.count - 1;
    for (; i > 0; i--) {
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

uint1024_t add_op(uint1024_t x, uint1024_t y) {
    to_one_size(&x, &y);
    uint1024_t result;
    result.number = calloc(x.size, sizeof(uint32_t));
    result.count = x.count;
    result.size = x.size;
    uint8_t carry = 0;
    for (size_t i = 0; i < x.count; i++) {
        uint64_t temp = carry + x.number[i] + y.number[i];
        result.number[i] = temp % BASE;
        carry = temp / BASE;
    }
    if (carry) {
        result.number[result.count] = carry;
        result.count += 1;
    }
    return result;
}

uint1024_t subtr_op(uint1024_t x, uint1024_t y) {
    to_one_size(&x, &y);
    uint1024_t result;
    result.number = calloc(x.size, sizeof(uint32_t));
    result.count = x.count;
    result.size = x.size;
    uint8_t carry = 0;
    for (size_t i = 0; i < x.count; i++) {
        uint64_t temp = (BASE + x.number[i]) - (carry + y.number[i]);
        result.number[i] = temp % BASE;
        carry = (temp >= BASE) ? 0 : 1;
    }
    return result;
}

uint1024_t mult_op(uint1024_t x, uint1024_t y) {
    to_one_size(&x, &y);
    uint1024_t result;
    result.number = calloc(x.size * 2, sizeof(uint32_t));
    result.count = x.count * 2;
    result.size = x.size * 2;
    int carry = 0;
    for (size_t i = 0; i < x.count; i++) {
        for (size_t j = 0; j < x.count; j++) {
            uint64_t temp = result.number[i + j];
            temp += x.number[i] * (uint64_t) y.number[j];
            temp += carry;
            result.number[i + j] = temp % BASE;
            carry = temp / BASE;
        }
    }
    if (carry) {
        result.number[result.count - 1] = carry;
    }
    return result;
}

void scanf_value(uint1024_t *x) {
    const size_t SIZE = 9;
    uint8_t str[1024];
    scanf("%s", str);
    uint1024_t uint1024;
    uint1024.size = strlen(str);
    uint1024.count = 0;
    uint1024.number = malloc(uint1024.size * sizeof(uint32_t));

    uint8_t chunck[SIZE + 1];
    memset(chunck, 0, (SIZE + 1) * sizeof(uint8_t));

    size_t next = 0;
    size_t index = 0;
    size_t length = strlen(str);

    for (ssize_t i = length - 1; i != -1; i--) {
        chunck[index++] = str[i];
        if (index == SIZE) {
            index = 0;
            swap(chunck, SIZE);
            uint1024.number[next++] = atoi(chunck);
            uint1024.count += 1;
        }
    }
    if (index != 0) {
        chunck[index] = '\0';
        swap(chunck, index);
        uint1024.number[next++] = atoi(chunck);
        uint1024.count += 1;
    }

    *x = uint1024;

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

/*
999999999
1

1000000000000
999999999999
*/