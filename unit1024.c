#include <stdio.h>
#include <string.h>
#include <stdint.h>

const uint32_t mask32 = 0x0000ffff;

typedef struct uint1024_t {
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
    uint32_t r12;
    uint32_t r13;
    uint32_t r14;
    uint32_t r15;
    uint32_t r16;
    uint32_t r17;
    uint32_t r18;
    uint32_t r19;
    uint32_t r20;
    uint32_t r21;
    uint32_t r22;
    uint32_t r23;
    uint32_t r24;
    uint32_t r25;
    uint32_t r26;
    uint32_t r27;
    uint32_t r28;
    uint32_t r29;
    uint32_t r30;
    uint32_t r31;
    uint32_t error;
}uint1024_t;


uint1024_t from_uint(unsigned int x){
    uint1024_t r;
    r.error = 0;

    for (uint8_t i = 0; i < 32; i++) //делаем число не мусором
        *(&r.r0 + i) = 0x00000000;

    r.r0 = x;

    return r;
}


uint32_t add_segment(uint32_t x, uint32_t y, uint32_t d, uint32_t* r) {
    uint64_t t = x + y + d;
    *r = t & mask32;
    return t >> 32; // происходит обрезание
}

uint1024_t add_op(uint1024_t x, uint1024_t y) {
    uint1024_t r = from_uint(0);

    if (x.error > 0 || y.error > 0) {
        r.error = 1;
        return r;
    }

    uint32_t d = 0; // переполнение лежит тут
    for (uint8_t i = 0; i < 32; i++){
        d = add_segment(*(&x.r0 + i), *(&y.r0 + i), d, &r.r0 + i); //звездочкой выковыриваем значение
    }
    if (d > 0) // если переполнение все еще есть оно слишом переполнилось
        r.error = 1;

    return r;
}

uint32_t subtr_segment(uint32_t x, uint32_t y, uint32_t d, uint32_t* r) {
    uint64_t t = 0;
    if (x < y + d) {
        t = 1;
        x += 0x00010000; // заняли разряд
    }
    *r = x - y - d;
    return t;
}

uint1024_t subtr_op(uint1024_t x, uint1024_t y) {
    uint1024_t r = from_uint(0);

    if (x.error > 0 || y.error > 0) {
        r.error = 1;
        return r;
    }

    uint32_t d = 0; // тут занимание
    for (uint8_t i = 0; i < 32; i++)
        d = subtr_segment(*(&x.r0 + i), *(&y.r0 + i), d, &r.r0 + i);

    if (d > 0) // переполнилось занимание
        r.error = 1;

    return r;
}

uint32_t mult_segment(uint32_t x, uint32_t y, uint32_t d, uint32_t* r) {
    uint64_t t = x * y + d;
    *r += t & mask32;
    return t >> 32;
}

uint1024_t mult_op(uint1024_t x, uint1024_t y)
{
    uint1024_t r = from_uint(0);

    if (x.error > 0 || y.error > 0) {
        r.error = 1;
        return r;
    }


    uint32_t d = 0;
    for (uint8_t j = 0; j < 32; j++)
        for (uint8_t i = 0; i < 32-j; i++) {
            d = mult_segment(*(&x.r0 + i), *(&y.r0 + j), d, &r.r0 + (j+i));
            if (i+j == 31 && d > 0) {
                r.error = 1;
                return r;
            }
        }

    return r;
}
void printf_value(uint1024_t x) {
    for (uint8_t i = 0; i < 32; i++){
        printf("%d", *(&x.r31 - i));
    }
}
void scanf_value(uint1024_t* x){

    uint1024_t r = from_uint(0);

    char str[4000];
    gets(str);

    uint1024_t base = from_uint(1);
    uint1024_t ten = from_uint(10);


    for (int i = strlen(str) - 1; i >= 0; i--){
        int a = str[i] - 48;
        uint1024_t anew = from_uint(a);
        uint1024_t temp = mult_op(anew, base);
        r = add_op(r, temp);
        base = mult_op(base, ten);
    }
    *x = r;
}


int main() {
    char a;
    uint1024_t x, y;
    printf("Enter first number: \n");
    scanf_value(&x);
    printf("Enter second number: \n");
    scanf_value(&y);
    printf("Enter action you would like to use: (+, -, *) \n");
    scanf("%s", &a);
    if (a == '+') {
        printf("Answer: ");
        if (add_op(x, y).error == 1){
            printf("%s", "переполнение");
        } else {
            printf_value(add_op(x, y));
        }
    } else if (a == '-') {
        printf("Answer: ");
        if (subtr_op(x, y).error == 1){
            printf("%s", "переполнение");
        } else {
            printf_value(subtr_op(x, y));
        }
    } else if (a == '*') {
        printf("Answer: ");
        if (mult_op(x, y).error == 1){
            printf("%s", "переполнение");
        } else {
            printf_value(mult_op(x, y));
        }

    }

    return 0;
}

