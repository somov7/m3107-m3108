#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int BASE = 1000000000;
const int DIGIT_NUMBER = 9;

typedef struct uint1024 {
    int* number;
    int size;
} uint1024_t;

int max(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

//not working properly on x * y > 1000000000
uint1024_t mult_op(uint1024_t x, uint1024_t y) {
    int resSize = x.size + y.size;
    uint1024_t resultMult;
    resultMult.number = calloc(resSize, sizeof (int));
    resultMult.size = resSize;
    for (int i = 0; i < x.size; ++i) {
        for (int j = 0; j < x.size - i; ++j) {
            resultMult.number[i + j] += x.number[i] * y.number[j];
        }
    }
    for (int i = 0; i < x.size - 1; ++i) {
        resultMult.number[i + 1] += resultMult.number[i] / BASE;
        resultMult.number[i] %= BASE;
    }
    return resultMult;
}

uint1024_t subtr_op(uint1024_t x, uint1024_t y) {
    uint1024_t resultSubtr;
    resultSubtr.number = calloc(x.size, sizeof(int));
    resultSubtr.size = x.size;
    short underflow = 0;

    for (int i = 0; i < x.size || underflow; ++i) {
        if (i == x.size) {
            break;
        }
        resultSubtr.number[i] = x.number[i] - underflow;
        if (i < y.size) {
            resultSubtr.number[i] -= y.number[i];
        }
        if (resultSubtr.number[i] < 0) {
            underflow = 1;
            resultSubtr.number[i] += BASE;
        } else {
            underflow = 0;
        }
    }
    return resultSubtr;
}

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
    int resSize = max(x.size, y.size);

    uint1024_t resultAdd;
    resultAdd.number = calloc(resSize, sizeof (int));
    resultAdd.size = resSize;
    short overflow = 0;

    for (int i = 0; i < resSize || overflow; ++i) {
        if (i == resSize) {
            resultAdd.number = realloc(resultAdd.number, (resSize + 1) * sizeof (int));
            resultAdd.number[resSize] = 0;
            resultAdd.size += 1;
        }
        resultAdd.number[i] += overflow;
        if (i < x.size) {
            resultAdd.number[i] += x.number[i];
        }
        if (i < y.size) {
            resultAdd.number[i] += y.number[i];
        }
        if (resultAdd.number[i] >= BASE) {
            overflow = 1;
            resultAdd.number[i] -= BASE;
        } else {
            overflow = 0;
        }
    }
    return resultAdd;
}


void printf_value(uint1024_t x) {
    if (x.size == 0) {
        printf("%d", 0);
    } else {
        printf("%d", x.number[x.size - 1]);
    }
    for (int i = x.size - 2; i >= 0; --i) {
        printf("%09d", x.number[i]);
    }
}

void scanf_value(uint1024_t* x) {
    uint1024_t resValue;
    int resSize;
    char value[1024];
    scanf("%s", value);
    int valueLength = strlen(value);
    if (valueLength % DIGIT_NUMBER == 0) {
        resSize = valueLength / DIGIT_NUMBER;
    } else {
        resSize = valueLength / DIGIT_NUMBER + 1;
    }
    resValue.number = malloc(resSize * sizeof (int));
    resValue.size = resSize;
    for (int i = valueLength, j = 0; i > 0; i -= DIGIT_NUMBER, ++j) {
        value[i] = '\0';
        if (i >= DIGIT_NUMBER) {
            resValue.number[j] = atoi(value + (i - 9));
        } else {
            resValue.number[j] = atoi(value);
        }
    }
    *x = resValue;
}

int main() {
    uint1024_t x;
    uint1024_t y;
    printf("%s", "Enter uint1024 value n1\n");
    scanf_value(&x);
    printf("%s", "Enter uint1024 value n2\n");
    scanf_value(&y);

    printf("x + y = ");
    printf_value(add_op(x, y));
    printf("\nx - y = ");
    printf_value(subtr_op(x, y));
    printf("\nx * y = ");
    printf_value(mult_op(x, y));

    unsigned int a;
    uint1024_t z;
    printf("%s", "\nEnter int value\n");
    scanf("%d", &a);
    z = from_uint(a);
    printf("uint a = ");
    printf_value(z);
}