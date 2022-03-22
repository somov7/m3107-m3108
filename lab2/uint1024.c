#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "uint1024.h"

const int BASE = 1000 * 1000 * 1000;

int get_max(int x, int y) {
    if (x > y) return x;
    return y;
}

uint1024_t constructor(int size) {
    uint1024_t number = {
            .a = calloc(size, sizeof(int)),
            .size = size,
    };
    return number;
}

uint1024_t from_uint(uint x) {
    int size = 1;
    if (x >= BASE) size = 2;
    uint1024_t number = constructor(size);
    if (size == 1) {
        number.a[0] = x;
    } else {
        number.a[0] = x % BASE;
        number.a[1] = x / BASE;
    }
    return number;
}


void printf_value(uint1024_t number) {
    int size = number.size;
    if (size == 0) printf("%d", 0);
    else printf("%d", number.a[size-1]);
    for (int i=size-2; i >= 0; --i) printf("%09d", number.a[i]);
}

void scanf_value(uint1024_t* number) {
    char line[309];
    scanf("%s", line);

    int len = (int)strlen(line);
    int size = len / 9;
    if (len % 9 != 0) size++;

    number->size = size;
    number->a = calloc(size, sizeof(int));

    for (int j = 0, i = len; i > 0; i -= 9, ++j) {
	    line[i] = '\0';
        if (i >= 9) {
            number->a[j] = atoi(line + i - 9);
        } else
            number->a[j] = atoi(line);
    }
}

uint1024_t add_op(uint1024_t x, uint1024_t y) {
    int carry = 0;
    int size = get_max(x.size, y.size);
    uint1024_t number = constructor(size);
    for (int i = 0 ; (i < size) || carry; ++i) {
        if (size == i) {
            number.a = realloc(number.a, (size + 1) * sizeof(int));
            number.a[number.size++] = 0;
        }
        number.a[i] = carry;
        if (y.size > i) number.a[i] += y.a[i];
        if (x.size > i) number.a[i] += x.a[i];
        carry = number.a[i] >= BASE;
        if (carry) number.a[i] -= BASE;
    }
    return number;
}

uint1024_t subtr_op(uint1024_t x, uint1024_t y) {
    if ((y.size > x.size) || ( (y.size == x.size) &&  y.a[y.size - 1] > x.a[x.size - 1])) {
        printf("undefined behavior");
        exit(1);
    }

    int carry = 0;
    int size = x.size;

    for (int i = 0; (i < size) || carry; ++i) {
        if (y.size > i)
            x.a[i] -= carry + y.a[i];
        else
            x.a[i] -= carry;
        carry = x.a[i] < 0;
        if (carry) x.a[i] += BASE;
    }

    while (x.a[x.size - 1] == 0 && x.size > 1)
        x.size--;

    x.a = realloc(x.a, x.size*sizeof(int));
    return x;
}
