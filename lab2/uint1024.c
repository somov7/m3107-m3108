#include "uint1024.h"

const int BASE = 1000000000;

uint1024_t from_uint(unsigned int x) {
    int size;
    uint1024_t res;

    if (x >= BASE) {
        size = 2;
    } else {
        size = 1;
    }

    res.t = malloc(size * sizeof(int32_t));
    res.size = size;
    res.t[0] = x % BASE;

    if (size > 1) {
        res.t[1] = x / BASE;
    }

    return res;
}

uint1024_t add_op(uint1024_t x, uint1024_t y) {
    int size;
    uint1024_t res;
    int32_t carry = 0;

    if (x.size > y.size) {
        size = x.size;
    } else {
        size = y.size;
    }

    res.t = calloc(size, sizeof(int32_t));
    res.size = size;

    int i = 0;

    while (i < size || carry) {
        if (i == size) {
            res.t = realloc(res.t, (size + 1) * sizeof(int32_t));
            res.t[size] = 0;
            res.size++;
        }

        res.t[i] += carry;

        if (i < x.size) {
            res.t[i] += x.t[i];
        }
        if (i < y.size) {
            res.t[i] += y.t[i];
        }

        if (res.t[i] >= BASE) {
            carry = 1;
            res.t[i] -= BASE;
        } else {
            carry = 0;
        }

        i++;
    }

    return res;

}

uint1024_t subtr_op(uint1024_t x, uint1024_t y) {
    uint1024_t res;
    int size = x.size;
    int carry = 0;


    res.t = calloc(size, sizeof(int32_t));
    res.size = size;

    int i = 0;

    while (i < size || carry) {

        res.t[i] = x.t[i] - carry;

        if (i < y.size) {
            res.t[i] -= y.t[i];
        }

        if (res.t[i] < 0) {
            carry = 1;
            res.t[i] += BASE;
        } else {
            carry = 0;
        }

        i++;
    }

    while (res.t[res.size - 1] == 0 && res.size > 1) {
        res.size--;
    }

    res.t = realloc(res.t, res.size * sizeof(int32_t));
    return res;
}

uint1024_t mult_op(uint1024_t x, uint1024_t y) {
    int carry;
    int size = x.size + y.size;
    long long t;
    uint1024_t res;

    res.t = calloc(size, sizeof(int32_t));
    res.size = size;

    for (int i = 0; i < x.size; i++) {
        int j = 0;
        carry = 0;

        while (j < y.size || carry) {
            t = res.t[i + j];

            if (j < y.size) {
                t += (long long) x.t[i] * y.t[j];
            }

            t += carry;
            res.t[i + j] = t % BASE;
            carry = t / BASE;

            j++;
        }
    }

    while (res.t[size - 1] == 0 && size > 1) {
        size--;
    }

    res.t = realloc(res.t, size * sizeof(int32_t));
    res.size = size;

    return res;
}


void scanf_value(uint1024_t *x) {
    char str[1024];
    int size;
    int len;
    uint1024_t res;

    scanf("%309s", str);
    len = strlen(str);

    if (len % 9 == 0) {
        size = len / 9;
    } else {
        size = len / 9 + 1;
    }


    res.t = malloc(size * sizeof(int32_t));
    res.size = size;

    for (int i = len, j = 0; i > 0; i -= 9, j++) {
        str[i] = '\0';

        if (i >= 9) {
            res.t[j] = atoi(str + i - 9);
        } else {
            res.t[j] = atoi(str);
        }
    }

    *x = res;
}


void printf_value(uint1024_t x) {
    int size = x.size;

    if (size) {
        printf("%d", x.t[size - 1]);
    } else {
        printf(0);
    }

    for (int i = size - 2; i >= 0; --i) {
        printf("%09d", x.t[i]);
    }
}