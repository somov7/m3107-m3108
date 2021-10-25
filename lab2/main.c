#include <stdio.h>
#include <math.h>
#include <string.h>

typedef struct
{
    unsigned int nums[35];
    int ranks;
} uint1024_t;

uint1024_t from_uint(unsigned int x)
{
    uint1024_t element;
    int j = 0;
    while (j < 35)
    {
        element.nums[j] = 0;
        j++;
    }
    if (x < 1000000000)
    {
        element.nums[0] = x;
        element.ranks = 1;
    }
    else
    {
        element.nums[0] = x % 1000000000;
        element.nums[1] = x / 1000000000;
        element.ranks = 2;
    }
    return element;
}

uint1024_t add_op(uint1024_t x, uint1024_t y)
{
    uint1024_t res;
    long long temp = 0;
    int elem = 1;
    for (int i = 0; i < 35; i++)
    {
        temp = (long long)x.nums[i] + (long long)y.nums[i] + temp;
        res.nums[i] = temp % 2147483647;
        temp = temp / 2147483647;
    }
    for (int k = 34; k >= 0; k--)
    {
        if (res.nums[k] != 0)
        {
            elem = k + 1;
            break;
        }
    }
    res.ranks = elem;
    return res;
}

uint1024_t subtr_op(uint1024_t x, uint1024_t y)
{
    uint1024_t res;
    long long temp = 0;
    int elem = 1;
    for (int i = 0; i < 35; i++)
    {
        temp = (long long)x.nums[i] - (long long)y.nums[i] - temp;
        if (temp < 0) {
            temp += 1000000000;
            res.nums[i] = temp;
            temp = 1;
        }
        else
        {
            res.nums[i] = temp;
            temp = 0;
        }
    }
    for (int k = 34; k >= 0; k--)
    {
        if (res.nums[k] != 0)
        {
            elem = k + 1;
            break;
        }
    }
    res.ranks = elem;
    return res;
}

uint1024_t mult_op(uint1024_t x, uint1024_t y) {
    uint1024_t res;
    int elem = 1;

    return res;
}

void printf_value(uint1024_t x)
{
    for (int i = x.ranks - 1; i >= 0; i--)
    {
        int j = 8;
        while (i != x.ranks - 1 && pow(10, j--) - x.nums[i] > 0 && j >= 0) {
            printf("0");
        }
        printf("%i", x.nums[i]);

    }
}

void scanf_value(uint1024_t* x)
{
    char str[310];
    int i, rank = 0;
    for (i = 0; i < 35; i++) // заполняем нулями
    {
        x->nums[i] = 0;
    }
    scanf("%s", str);
    i = strlen(str) - 1; // длина числа, видимо
    while (i >= 0)
    {
        x->nums[rank / 9] += (str[i] - '0') * pow (10, rank % 9); // не особо понял, но вроде просто проходимся
        // по массиву по каждому числу
        i--;
        rank++;
    }
    x->ranks = (rank - 1) / 9 + 1; // количество разрядов
}

int main()
{
    char sign;
    uint1024_t x, y;
    printf("Enter first number: \n");
    scanf_value(&x);
    printf("Enter second number: \n");
    scanf_value(&y);
    printf("Enter action you would like to use: (+, -, *) \n");
    scanf("%s", &sign);
    if (sign == '+')
    {
        printf("Answer: ");
        printf_value(add_op(x, y));
    }
    else if (sign == '-')
    {
        printf("Answer: ");
        printf_value(subtr_op(x, y));
    }
    else if (sign == '*')
    {
        printf("Answer: ");
        printf_value(mult_op(x, y));
    }
    return 0;
}



