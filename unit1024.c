#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

const uint32_t MASK832 = 0x000000ff;
const uint32_t MASK816 = 0x00ff;
const uint16_t DECLENGTH = 3277; // HEXLENGTH * 256 / 10 = кол-во десятичных разрядов числа
const uint8_t HEXLENGTH = 128; // uint8_t == 2**8; 1024 / 8 = HEXLENGTH

typedef uint8_t uint1024_t[128];

void initu(uint1024_t r) {
    memset(r, 0, HEXLENGTH); //делаем число не мусором
}

void initc(char *r) {
    memset(r, '\0', DECLENGTH); //делаем число не мусором
}

bool from_uint(unsigned int x, uint1024_t r) {
    // unsigned int === uint32_t
    initu(r);
    for (int i = 0; i < 4; i++) {
        r[i] = x & MASK832;
        x >>= 8;
    }
    return true;
}

void copy(uint1024_t src, uint1024_t dst) {
    initu(dst);
    memcpy(dst, src, HEXLENGTH);
}

bool is_zero_op(uint1024_t x) {
    for (int i = 0; i < HEXLENGTH; i++){
        if (x[i]){
            return false;
        }
    }
    return true;
}

uint8_t add_rank(uint8_t x, uint8_t y, uint8_t d, uint8_t *r) {
    uint16_t t = x;
    t += y;
    t += d;
    *r = (uint8_t)(t & MASK816);
    return (uint8_t)(t >> 8); // происходит обрезание
}

bool add_op(uint1024_t x, uint1024_t y, uint1024_t r) {
    uint8_t d = 0; // переполнение лежит тут

    initu(r);
    for (int i = 0; i < HEXLENGTH; i++)
        d = add_rank(x[i], y[i], d, r+i);
    return d == 0; // самого старшего переполнения быть не должно
}

uint8_t subtr_rank(uint8_t x, uint8_t y, uint8_t d, uint8_t *r) {
    uint16_t t = x;
    uint8_t dd = 0;

    if (x < y + d) {
        t += 0x0100; // заняли разряд
        dd = 1;
    }
    t -= y;
    t -= d;
    *r = (uint8_t)t;
    return dd;
}

bool subtr_op(uint1024_t x, uint1024_t y, uint1024_t r) {
    uint8_t d = 0; // тут занимание

    initu(r);
    for (int i = 0; i < HEXLENGTH; i++)
        d = subtr_rank(x[i], y[i], d, r+i);
    return d == 0; // самого старшего занимания быть не должно
}

bool mult_rank(uint8_t x, uint8_t y, uint1024_t r, int i) {
    uint32_t t = x * y;
    uint16_t d = 0;

    if (i >= HEXLENGTH)
        return false;
    for (int j = i; j < HEXLENGTH; j++) {
        t += d + (r[j] & MASK832);
        r[j] = (uint8_t)(t & MASK832);
        d = (uint16_t)(t >> 8);
        if (j == HEXLENGTH-1 && d){
            return false; // самого старшего переполнения быть не должно
        }
        if (!d){
            break; // всплывание переполнения закончилось
        }
        t = 0;
    }
    return true;
}

bool mult_op8(uint1024_t x, uint8_t y, uint1024_t r) {
    initu(r);
    for (int i = 0; i < HEXLENGTH; i++) {
        if (x[i] && y) {
            if (!mult_rank(x[i], y, r, i))
                return false;
        }
    }
    return true;
}

bool mult_op(uint1024_t x, uint1024_t y, uint1024_t r) {
    initu(r);
    for (int j = 0; j < HEXLENGTH; j++) {
        for (int i = 0; i < HEXLENGTH; i++) {
            if (x[i] && y[j]) {
                if (i+j < HEXLENGTH) {
                    if (!mult_rank(x[i], y[j], r, i+j))
                        return false;
                }
            }
        }
    }
    return true;
}

int compare_op(uint1024_t x, uint1024_t y) {
    for (int i = HEXLENGTH-1; i >= 0; i--) {
        if (x[i] > y[i])
            return 1;
        else if (x[i] < y[i])
            return -1;
    }
    return 0;
}

bool scanf_value(uint1024_t r) {
    char str[DECLENGTH];
    uint1024_t dec10; // число текущей степени 10
    uint32_t a;
    uint1024_t tmp;
    uint1024_t anew;

    initu(r);
    from_uint(1, dec10);

    gets(str);
    for (int i = (int)strlen(str)-1; i >= 0; i--) {
        a = str[i] - 48;
        from_uint(a, anew);

        // умножаем anew на dec10
        if (!mult_op(dec10, anew, tmp))
            return false;
        copy(tmp, anew);

        // прибавляем anew к r
        if (!add_op(anew, r, tmp))
            return false;
        copy(tmp, r);

        // умножаем dec10 на 10 кроме последнего разряда
        if (i > 0) {
            if (!mult_op8(dec10, 10, tmp))
                return false;
            copy(tmp, dec10);
        }
    }
    return true;
}

bool printf_value_hex(uint1024_t x) {

    bool isleading = true;
    for (int i = 127; i >= 0; i--) {
        if (x[i] > 0 || !isleading) {
            if (isleading)
                printf("0x%X", x[i] & 0xff);
            else
                printf("%02X", x[i] & 0xff);
            isleading = false;
        }
    }
    printf("\n");
    return true;
}

bool printf_value(uint1024_t x) {

    char dec[DECLENGTH];
    uint1024_t y; // копия входного числа (будет модифицирована)
    uint1024_t dec10; // число текущей степени 10
    uint1024_t tmp, tmp1 ; // промежуточные для вычислений
    int rank; // текущий десятичный разряда
    uint8_t n; // значение текущего десятичного разряда

    copy(x, y);
    initc(dec); // инициализация нулями
    from_uint(1, tmp);

    // находим максимальную степень 10 (dec10) меньшую исходного числа
    for (rank = 0; compare_op(y, tmp) != -1; rank++) {
        if (!mult_op8(tmp, 10, tmp1))
            return false;
        copy(tmp1, tmp);
    }

    for (uint8_t i = 0; i < DECLENGTH; i++, rank--) {
        if (is_zero_op(y)) {
            if (rank > 0) {
                dec[i] = 48; // завершающие нули
                continue;
            } else {
                break;
            }
        }
        from_uint(1, dec10);
        for (uint8_t j = 0; j < rank-1; j++) {
            if (!mult_op8(dec10, 10, tmp))
                return false;
            copy(tmp, dec10);
        }
        // на данный момент в dec10 лежит на разряд меньше dectmp = 1 * 10 ** i
        copy(dec10, tmp);
        copy(dec10, tmp1);
        for (n = 0; compare_op(y, tmp) != -1; n++) {
            copy(tmp, dec10);
            if (!add_op(dec10, tmp1, tmp))
                return false;
        }
        dec[i] = n + 48; // сохраняем значение разряда как символ цифры
        if (!n)
            continue;
        // на данный момент в dec10 лежит n * 10 ** i
        // вычитаем из y найденное десятичное число
        if (subtr_op(y, dec10, tmp)) {
            copy(tmp, y); // если получилось, переходим к более младшему разряду
        } else {
            dec[i+1] = n + 48; // остался остаток от деления на 10 == младшая десятичная цифра
            break;
        }
    }
    printf("%s\n", dec);
    return true;
}

int main() {
    char a;
    uint1024_t x;
    uint1024_t y;
    uint1024_t r;

    printf("Enter first number:\n");
    if (!scanf_value(x))
        printf("scanf_value failed\n");
    printf("Entered x=");
    if (!printf_value(x))
        return false;
    printf("\n");
    printf("Enter second number:\n");
    if (!scanf_value(y))
        printf("scanf_value failed\n");
    printf("Entered y=");
    if (!printf_value(y))
        return false;
    printf("\n");
    printf("Enter operation you would like to perform: (+, -, *)\n");
    scanf("%s", &a);
    printf("Chosen operation %c\n", a);
    if (a == '+') {
        printf("Answer: ");
        if (add_op(x, y, r))
            printf_value(r);
        else
            printf("переполнение");
        printf("\n");
    } else if (a == '-') {
        printf("Answer: ");
        if (subtr_op(x, y, r))
            printf_value(r);
        else {
            printf("переполнение");
        }
        printf("\n");
    } else if (a == '*') {
        printf("Answer: ");
        if (mult_op(x, y, r))
            printf_value(r);
        else {
            printf("переполнение");
        }
        printf("\n");
    }

    return 0;
}