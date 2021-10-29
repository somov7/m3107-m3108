#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <iso646.h>

#define y_or_zero (i < y.len ? y.num[i] : 0)
#define x_or_zero (i < x.len ? x.num[i] : 0)


const int base = 1000 * 1000 * 1000;
const int max_uint1024_len = 309;

typedef struct {
    uint32_t *num;
    int32_t len;
} uint1024_t;

int max(int a, int b) {
    if (a > b)
        return a;
    else
        return b;
}

int x_less_y(uint1024_t x, uint1024_t y){
    if (x.len < y.len)
        return 1;
    if (x.len == y.len and x.num[x.len-1] < y.num[y.len-1])
        return 1;
    return 0;
}
uint1024_t init(int size) {
    uint1024_t result;
    result.len = size;
    result.num = calloc(size, sizeof(uint32_t));
    return result;
}

void removing_zeros(uint1024_t *x) {
    while (x->num[x->len - 1] == 0 and x->len > 1)
        x->len--;
    x->num = realloc(x->num, x->len * sizeof(uint32_t));
}

void scanf_value(uint1024_t *x) {

    char str[max_uint1024_len];
    scanf("%309s", str);
    int str_len = strlen(str);
    int num_len;

    if (str_len % 9 == 0)
        num_len = str_len / 9;
    else
        num_len = (str_len / 9) + 1;

    uint1024_t result = init(num_len);

    for (int i = str_len, j = -1; i > 0; i -= 9) {
        j++;
        str[i] = '\0';
        // Устанавливаем в i-ый элемент завершающий ноль,
        // записываем в массив число = str[i-9:i], если i >= 9
        // иначе записываем всю оставшующся строку
        if (i >= 9)
            result.num[j] = atoi(str + i - 9);
        else
            result.num[j] = atoi(str);
    }
    *x = result;
}

void printf_value(uint1024_t x) {
    // Проверка на то, что первый элемент - 0
    // последующий вывод первого элемента
    if (x.num[x.len - 1] == 0 and x.len == 1)
        printf("0");
    else
        printf("%d", x.num[x.len - 1]);

    for (int i = x.len - 2; i >= 0; --i)
        printf("%09d", x.num[i]);
}

uint1024_t from_uint(unsigned int x) {
    uint1024_t result;
    // Если x < взятого основания, то записываем число в массив размерность которого 1,
    // иначе размерность - 2
    if (x < base) {
        result.num = calloc(1, sizeof(uint32_t));
        result.num[0] = x;
        result.len = 1;
    } else {
        result.num = calloc(2, sizeof(uint32_t));
        result.len = 2;
        result.num[0] = x % base;
        result.num[1] = x / base;
    }
    return result;
}

uint1024_t add_op(uint1024_t x, uint1024_t y) {

    int carry = 0;
    int arr_size = max(x.len, y.len);
    uint1024_t result = init(arr_size);

    // Вычисление выражения x + y в result.num
    for (int i = 0; i < arr_size or carry; i++) {
        // увеличиваем размер массива на один, если произошёл перенос при i = arr_size - 1;
        if (i == arr_size) {
            result.num = realloc(result.num, (arr_size + 1) * sizeof(int32_t));
            result.num[arr_size] = 0;
            result.len++;
        }
        result.num[i] = carry + y_or_zero + x_or_zero;
        carry = result.num[i] >= base;
        if (carry)
            result.num[i] -= base;
    }
    return result;
}

uint1024_t subtr_op(uint1024_t x, uint1024_t y) {
    if (x_less_y(x, y))
        return init(1); // undefined behavior
    int carry = 0;
    int arr_size = max(x.len, y.len);
    uint1024_t result = init(arr_size);

    // Вычисление выражения x - y в result.num
    for (int i = 0; i < arr_size or carry; ++i) {
        if (x.num[i] < y_or_zero + carry) {
            result.num[i] = base + (x_or_zero - y_or_zero - carry);
            carry = 1;
        } else {
            result.num[i] = x_or_zero - y_or_zero - carry;
            carry = 0;
        }
    }

    removing_zeros(&result);
    return result;
}

uint1024_t mult_op(uint1024_t x, uint1024_t y) {
    if (x.len*9 + y.len*9 - 1 > max_uint1024_len)
        return init(1); // undefined behavior
    int arr_size = x.len + y.len;
    uint1024_t result = init(arr_size);

    for (int i = 0; i < x.len; ++i)
        for (int j = 0, carry = 0; j < y.len or carry; ++j) {
            long long cur = result.num[i + j] + x.num[i] * 1ll * (j < y.len ? y.num[j] : 0) + carry;
            result.num[i + j] = (uint32_t) (cur % base);
            carry = (int) (cur / base);
        }

    removing_zeros(&result);
    return result;
}

int main() {
    uint1024_t x, y;
    printf("Please, enter two uint1024_t numbers\n");
    scanf_value(&x);
    scanf_value(&y);

    printf("\n x = ");
    printf_value(x);
    printf("\n y = ");
    printf_value(y);

    printf("\n x + y = ");
    printf_value(add_op(x, y));
    printf("\n x - y = ");
    printf_value(subtr_op(x, y));
    printf("\n x * y = ");
    printf_value(mult_op(x, y));

    printf("\n\nPlease, enter two uint32_t numbers\n");
    unsigned int x1, y1;
    scanf("%u", &x1);
    scanf("%u", &y1);
    x = from_uint(x1);
    y = from_uint(y1);

    printf("\n x = ");
    printf_value(x);
    printf("\n y = ");
    printf_value(y);

    printf("\n x + y = ");
    printf_value(add_op(x, y));
    printf("\n x - y = ");
    printf_value(subtr_op(x, y));
    printf("\n x * y = ");
    printf_value(mult_op(x, y));

    return 0;
}