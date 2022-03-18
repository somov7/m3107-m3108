#include <stdio.h>
#include <math.h>

const int MAX_INT = 1e9; // pow(10, SIZE)
const short NUM_DIGITS = 315; // ceil(308 / 9) * 9
const short SIZE = 9; // maximum digits of "unsigned int" - 1
#define INT_BIT_SIZE 35 // NUM_DIGITS / SIZE

typedef struct uint1024 {
    unsigned int part[INT_BIT_SIZE];
} uint1024_t;

uint1024_t from_uint(unsigned int x) {

    uint1024_t finish;

    for (int i = 0; i < INT_BIT_SIZE - 2; i++) {
        finish.part[i] = 0;
    }

    finish.part[INT_BIT_SIZE - 2] = x / MAX_INT;
    finish.part[INT_BIT_SIZE - 1] = x % MAX_INT;

    return finish;
}

uint1024_t add_op(uint1024_t x, uint1024_t y) {

    uint1024_t finish = from_uint(0);

    for (int i = INT_BIT_SIZE - 1; i > 0; i--) {
        finish.part[i] += (x.part[i] + y.part[i]) % MAX_INT;
        finish.part[i - 1] += (x.part[i] + y.part[i]) / MAX_INT;
    }

    finish.part[0] += (x.part[0] + y.part[0]) % MAX_INT;

    for (int i = INT_BIT_SIZE - 1; i > 0; i--) {
        finish.part[i - 1] += finish.part[i] / MAX_INT;
        finish.part[i] %= MAX_INT;
    }

    finish.part[0] %= MAX_INT;

    return finish;
}

uint1024_t subtr_op(uint1024_t x, uint1024_t y) {

    uint1024_t finish = from_uint(0);

    for (int i = 0; i < INT_BIT_SIZE; i++) {
        finish.part[i] = MAX_INT - 1 - y.part[i];
    }

    finish = add_op(finish, from_uint(1));
    finish = add_op(finish, x);

    return finish;
}

uint1024_t mult_op(uint1024_t x, uint1024_t y) {

    unsigned long long buffer;
    uint1024_t finish = from_uint(0);

    for (int i = 0; i < INT_BIT_SIZE; i++) {
        for (int j = 0; j < INT_BIT_SIZE; j++) {

            buffer = x.part[i] * y.part[j];

            int place = i + j - INT_BIT_SIZE + 1;

            finish.part[place] += buffer % MAX_INT;

            if (place >= 1) finish.part[place - 1] += buffer / MAX_INT;
            if (place >= 2) finish.part[place - 2] += buffer / MAX_INT / MAX_INT;
        }
    }

    for (int i = INT_BIT_SIZE - 1; i > 0; i--) {
        finish.part[i - 1] += finish.part[i] / MAX_INT;
        finish.part[i] %= MAX_INT;
    }

    finish.part[0] %= MAX_INT;

    return finish;
}

void printf_value(uint1024_t x) {

    short start_flag = 0;

    int i = 0;

    while (start_flag == 0) {

        if (x.part[i] != 0) start_flag++;

        i++;

        if (i == INT_BIT_SIZE) break;
    }

    printf("%d", x.part[i - 1]);

    for (; i < INT_BIT_SIZE; i++) printf("%09d", x.part[i]);

    printf("\n");
}

void scanf_value(uint1024_t* x) {

    char digits[NUM_DIGITS + 1];

    for (int i = 0; i < NUM_DIGITS + 1; i++) {
        digits[i] = '\0';
    }

    printf("Enter number:\n");

    scanf("%315s", digits);

    char now = '-';
    short int size = 0;

    while (now != '\0') {
        now = digits[size];
        size++;
    }

    size--;

    for (int i = NUM_DIGITS - size; i < NUM_DIGITS; i++) {
        short piece = INT_BIT_SIZE - (NUM_DIGITS - i - 1) / SIZE;
        x->part[piece - 1] += (digits[i + size - NUM_DIGITS] - '0') * (int)pow(10, (NUM_DIGITS - i - 1) % SIZE);
    }
}

int main() {

    uint1024_t a, b;
    unsigned int base = 1234567890;

    a = from_uint(0);
    b = from_uint(0);

    scanf_value(&a);

    b = from_uint(base);

    printf("a = ");
    printf_value(a);

    printf("b = ");
    printf_value(b);

    printf("a + b = ");
    printf_value(add_op(a, b));

    printf("a - b = ");
    printf_value(subtr_op(a, b));

    printf("b - a = ");
    printf_value(subtr_op(b, a));

    printf("a * b = ");
    printf_value(mult_op(a, b));

    return 0;
}
