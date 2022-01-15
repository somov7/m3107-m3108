#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define SIZE 32
#define BASE 1000000000

typedef struct {
    uint32_t digits[SIZE];
}uint1024_t;

uint1024_t from_uint(unsigned int x) {
    uint1024_t new = {0};

    // переводим x в систему исчисления по основанию BASE
    new.digits[0] = x % BASE; // первая цифра
    new.digits[1] = x / BASE; // вторая цифра
    return new;
}

uint1024_t add_op(uint1024_t x, uint1024_t y)
{
    uint1024_t new = {0};
    uint64_t carry = 0; // перенос

    for (int i = 0; i < SIZE; ++i)
    {
        new.digits[i] = (x.digits[i] + y.digits[i] + carry) % BASE;
        carry = (x.digits[i] + y.digits[i]) / BASE;
    }
    return new;
}

uint1024_t sub_op(uint1024_t x, uint1024_t y)
{
    uint1024_t new = {0};
    uint64_t borrow = 0;

    for (int i = 0; i < SIZE; ++i)
    {
        new.digits[i] = (x.digits[i] - y.digits[i] - borrow) % BASE;
        borrow = x.digits[i] < y.digits[i];
    }
    return new;
}

uint1024_t mult_op(uint1024_t x, uint1024_t y)
{
    uint1024_t res = {0};
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0, over = 0; j < SIZE || over; j++)
        {
            uint64_t mult = res.digits[i + j] + (uint64_t)x.digits[i] * y.digits[j] + over;
            res.digits[i + j] = mult % BASE;
            over = mult / BASE;
        }
    }
    return res;
}

void printf_value(const uint1024_t x)
{
    int j = 31;
    while (j > 0 && x.digits[j] == 0) j--;
    printf("%u", x.digits[j]);

    for (int i = j - 1; i >= 0; i--)
    {
        printf("%09u", x.digits[i]);
    }
    printf("\n");
}

void scanf_value(uint1024_t *x)
{
    char num[310] = {0};
    scanf("%309s", num);
    char digit[10] = {0};

    //strlen определяет длину строки
    int length = strlen(num) - 1;
    int k = 0;

    while (length >= 0)
    {
        for (int j = 8; j >= 0; --j)
        {
            digit[j] = '0';
            if (length >= 0 ) digit[j] = num[length--];
        }
        digit[9] = 0;
        x->digits[k++] = atol(digit);
    }
}

int main()
{
    uint1024_t x = from_uint(0);
    scanf_value(&x);
    uint1024_t y = from_uint(0);
    scanf_value(&y);

    printf("Sum: ");
    uint1024_t add = add_op(x, y);
    printf_value(add);

    printf("Difference: ");
    uint1024_t sub = sub_op(x, y);
    printf_value(sub);

    printf("Multiplication: ");
    uint1024_t multiplcation = mult_op(x, y);
    printf_value(multiplcation);

}
