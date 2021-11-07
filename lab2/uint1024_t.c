//
//  uint1024_t.c
//  BigInteger
//
//  Created by Антон Чемодуров on 02.10.2021.
//

#include "uint1024_t.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <sys/types.h>

uint1024_t from_uint(uint64_t x) {
    uint1024_t num;
    num.nums[0] = x;
    for (int i = 1; i < MAX_SIZE; i++) {
        num.nums[i] = 0;
    }
    num.size = 1;
    return num;
}

uint1024_t from_uint1024(uint1024_t x) {
    uint1024_t new;

    for (int i = 0; i < MAX_SIZE; i++) {
        new.nums[i] = x.nums[i];
    }
    new.size = x.size;
    return new;
}


uint1024_t from_str_len(const char* str, size_t len) {
    uint1024_t x = from_uint(0);
    uint1024_t base = from_uint(1);
    uint1024_t ten = from_uint(10);
    for (int i = (int)len -1; i >= 0; i--) {
        uint1024_t num = from_uint(str[i] - 48);
        uint1024_t tmp = mult_op(num, base);
        x = add_op(x, tmp);
        base = mult_op(base, ten);
    }
    return x;
}

uint1024_t from_str(const char* str) {
    size_t len = strlen(str);
    return from_str_len(str, len);
}

uint1024_t mult_op(uint1024_t x, uint1024_t y) {
    uint1024_t x_cpy = from_uint(0);
    unsigned int shift = 0;
    for (int i = 0; i < y.size; i++) {
        uint64_t tmp = y.nums[i];
        while (tmp) {
            if (tmp % 2 != 0) {
                uint1024_t sh = l_shift(&x, shift);
                x_cpy = add_op(x_cpy, sh);
            }
            tmp >>= 1;
            shift++;
        }
        shift = (i+1) * 64;
    }
    return x_cpy;
}


/**
 * Сдвиг в лево на t позиций (x << t)
 *  ВХОДНЫЕ ДАННЫЕ:
 *      uint1024_t x -- сдвигаемое число
 *      uint t -- величина сдввига
 *  ВЫХОДНЫЕ ДАННЫЕ:
 *      сдвинутое на t бит число x
 *
 */

uint1024_t l_shift(const uint1024_t *x, unsigned int t) {
    uint1024_t ans = from_uint(0);
    unsigned int k = t / 64, i,
            lsh = t % 64,
            rsh = 64 - lsh;
    uint64_t carry = 0;

    for (i = 0; (i < x->size || carry != 0) && i + k < MAX_SIZE; ++i) {
        uint64_t shift = x->nums[i] << lsh;
        shift |= carry;
        carry = rsh == 64 ? 0 : x->nums[i] >> rsh; // обработка неопределенного поведения
        ans.nums[i + k] = shift;
    }

    if (i + k > MAX_SIZE || (i + k == MAX_SIZE && ans.nums[MAX_SIZE - 1] == 0)) {
        ans.size = 1;
    } else {
        ans.size = i+k;
    }

    return ans;
}

/**
 * Складывает два числа типа uint1024_t
 * @return x + y
 */
uint1024_t add_op(uint1024_t x, uint1024_t y) {
    uint1024_t x_cpy;
    if (x.size < y.size) {
        x_cpy = from_uint1024(y);
        add_op_ptr(&x_cpy, &x);
    } else {
        x_cpy = from_uint1024(x);
        add_op_ptr(&x_cpy, &y);
    }


    return x_cpy;
}


/**
 *  складывает два числа результат записвает в x
 *  требуется x.size >= y.size
 */
void add_op_ptr(uint1024_t *x, uint1024_t *y) {
    assert(x->size >= y->size);

    unsigned int carry = 0, i;

    for (i = 0; i < y->size; ++i) {
        uint64_t xcpy = x->nums[i];
        x->nums[i] += y->nums[i] + carry;
        carry = x->nums[i] < xcpy && x->nums[i] < y->nums[i]; // проверям произошло ли переполнение
    }

    for (; carry && i < MAX_SIZE; ++i) {
        if (i >= x->size) x->size++;
        carry = (x->nums[i] += carry) == 0;
    }
}


/**
 *
 * находит разность двух числе
 *
 * ВХОДНЫЕ ДАННЫЕ:
 *  uint1024_t *x -- уменьшаемое
 *  uint1024_t *y -- вычетаемое
 *
 * ВЫХОДНЫЕ ДАННЫЕ:
 *  uint1024_t равзность двух числе x - y
 *
 * требуется: x >= y
 *
 */

uint1024_t subtr_op(uint1024_t x, uint1024_t y) {
    uint1024_t x_cpy = from_uint1024(x);
    subtr_op_ptr(&x_cpy, &y);
    return x_cpy;
}


/**
 *
 * находит разность двух числе
 *
 * ВХОДНЫЕ ДАННЫЕ:
 *      uint1024_t *x -- уменьшаемое
 *      uint1024_t *y -- вычетаемое
 *
 * ВЫХОДНЫЕ ДАННЫЕ:
 *      результат записывется в x
 *
 * требуется x >= y
 *
 */
void subtr_op_ptr(uint1024_t *x, uint1024_t *y) {
    assert(x->size >= y->size);

    unsigned int borrow = 0, i;

    for (i = 0; i < y->size; i++) {
        x->nums[i] -= borrow;
        borrow = x->nums[i] < y->nums[i];
        x->nums[i] -= y->nums[i];
    }

    for(; borrow && i < x->size; i++) {
        uint tmp = x->nums[i] < borrow;
        x->nums[i] -= borrow;
        // TODO перенести измение size в цикл
        borrow = tmp;
    }

    // уменьшаем размер числа
    if (i == x->size) {
        while (i > 0 && !x->nums[i - 1]) i--;
        x->size = i;
    }
}

/**
 * целочисленное деление двух чисел
 *  ВХОДНЫЕ ДАННЫЕ:
 * uint1024_t x -- делимое
 * uint64_t y -- делитель
 * uint64_t mod -- остаток от деления, если не используется передать NULL
 *
 *  ВЫХОДНЫЕ ДАННЫЕ:
 * uint1024_t ans -- результат деления
 * uint64_t *mod -- остаток от деления
 *
 * требуется y != 0
 *
 */
uint1024_t div_uint1024_uint64(uint1024_t x, uint64_t y, uint64_t *mod) {
    assert(y != 0);

    uint1024_t ans = from_uint(0);
    ans.size = x.size;
    uint64_t rem = 0;
    uint32_t *px = (uint32_t *)(x.nums + x.size);
    uint32_t *pans = (uint32_t *)(ans.nums + ans.size);
    int i = (int)x.size * 2;

    while (--i >= 0) {
        rem = (rem << 32) | *--px;
        uint32_t tmp;
        *--pans = tmp = (uint32_t)(rem / y);
        rem -= (uint64_t)tmp * y;
    }

    if (mod) *mod = rem;

    return ans;
}


/**
 * Функция сравнивает два числа типа uint1024_t
 * @return
 * 0 -- числа равны\n
 *  1 -- x > y\n
 *  -1 -- x < y
 */
int uint1024_cmp(uint1024_t *x, uint1024_t *y) {
    if (x->size > y->size) {
        return 1;
    } else if (x->size < y->size) {
        return -1;
    }

    for (int i = 0; i < x->size; ++i) {
        uint64_t x_uint = x->nums[i];
        uint64_t y_unit = y->nums[i];
        if (x_uint < y_unit)
            return -1;
        else if (x_uint > y_unit)
            return 1;
    }
    return 0;
}

char iszero(uint1024_t *x) {
    for (int i = 0; i < x->size; i++) {
        if (x->nums[i] != 0) {
            return 0;
        }
    }
    return 1;
}

void swap(char *i, char *j) {
    char tmp = *i;
    *i = *j;
    *j = tmp;
}


char *toString(uint1024_t x) {
    const char nums_table[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    char *buff = calloc(1024, sizeof(char));
    int i = 0;

    // перевод двоичного числа в десятичное
    do {
        uint64_t mod = 0;
        x = div_uint1024_uint64(x, 10, &mod);
        buff[i++] = nums_table[mod];
    }  while (!iszero(&x) && i < 1024);
    //i--;

    // разворачиваем массив
    for (int j = 0; j < i / 2; j++) {
        swap(buff + j, buff + (i-1) - j);
    }

    buff[i] = 0; // конец строки

    return buff;
}

/**
 * Выводит число в 10CC в stdin
 */
void printf_value(uint1024_t x) {
    char *num_str = toString(x);
    puts(num_str);
    free(num_str);
}

/**
 * Выводит hex число в stdin
 * @param x
 */
void print(uint1024_t x) {
    printf("0x");
    for (int i = (int)x.size - 1; i >= 0; --i) {
        printf("%016llx", x.nums[i]);
    }
    printf("\n");
}

/**
 *
 * Читает значение из stdin,
 * результат записывая в x
 *
 */

void scanf_value(uint1024_t* x) {
    char *line = NULL;
    size_t len;
    if ((len = getline(&line, &len, stdin)) == -1) {
        perror("error file reading");
        exit(0);
    }
    line[--len] = 0; // удаляем символ перевода строки

    *x = from_str_len(line, len);

    if (line) {
        free(line);
    }
}
