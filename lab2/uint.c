#include <stdio.h>
#include <math.h>
#include <string.h>

static int max_numb = 1000000000;
static int base = 9;

typedef struct {
    int arr[35];
    int impcells;
} uint1024_t;

uint1024_t from_uint(unsigned int x)
{
    uint1024_t res;
    int i;
    for (i = 0; i < 35; i++)
        res.arr[i] = 0;

    if (x < max_numb)
    {
        res.impcells = 1;
        res.arr[0] = x;
    }
    else
    {
        res.impcells = 2;
        res.arr[0] = x % max_numb;
        res.arr[1] = x / max_numb;
    }
    return res;
}

void printf_uint(uint1024_t x)
{
    int i;

    for (i = x.impcells - 1; i >= 0; i--)
    {
        int j = base - 1;

        while (i != x.impcells - 1 && pow(10, j--) - x.arr[i] > 0 && j >= 0)
            printf("0");

        printf("%i", x.arr[i]);
    }

    printf("\n");
}

uint1024_t add_op(uint1024_t x, uint1024_t y)
{
    uint1024_t sum;
    long long cur = 0;
    int impcells_sum = 1, i;

    for (i = 0; i < 35; i++)
    {
        cur = (long long)(x.arr[i] + y.arr[i]) + cur;
        sum.arr[i] = cur % max_numb;
        cur /= max_numb;
    }

    for (i = 34; i > -1; i--)
        if (sum.arr[i] != 0)
        {
            impcells_sum = i + 1;
            break;
        }

    sum.impcells = impcells_sum;

    return sum;
}

uint1024_t subtr_op(uint1024_t x, uint1024_t y)
{
    uint1024_t diff;
    long long cur = 0;
    int impcells_subtr = 1, i;

    for (i = 0; i < 35; i++)
    {
        cur = (long long)(x.arr[i] - y.arr[i]) - cur;
        if (cur > -1)
        {
            diff.arr[i] = cur;
            cur = 0;
        }
        else
        {
            diff.arr[i] = max_numb + cur;
            cur = 1;
        }

    }

    for (i = 34; i > -1; i--)
        if (diff.arr[i] != 0)
        {
            impcells_subtr = i + 1;
            break;
        }

    diff.impcells = impcells_subtr;

    return diff;
}

uint1024_t mult_op(uint1024_t x, uint1024_t y)
{
    uint1024_t prod;
    long long cur = 0;
    int impcells_mult = 1, i, j;

    for (i = 34; i > -1; i--)
        prod.arr[i] = 0;

    for (i = 0; i < 35; i++)
        for (j = 0; i + j < 35; j++)
        {
            cur = (long long)(x.arr[i]) * (long long)y.arr[j] + cur + prod.arr[i + j];
            prod.arr[i + j] = cur % max_numb;
            cur = cur / max_numb;
        }

    for (i = 34; i > -1; i--)
        if (prod.arr[i] != 0)
        {
            impcells_mult = i + 1;
            break;
        }

    prod.impcells = impcells_mult;

    return prod;
}

void scanf_value(uint1024_t *x)
{
    char tmp[310];

    int i, digits = 0;
    for (i = 0; i < 35; i++)
        x->arr[i] = 0;
    scanf("%309s", tmp);

    i = strlen(tmp) - 1;

    while (i > -1)
    {
        x->arr[digits / base] += (tmp[i] - '0') * pow (10, digits % base);
        i--;
        digits++;
    }
    x->impcells = (digits - 1) / base + 1;
}

int main()
{
    uint1024_t uint1, uint2, x, sum, diff, prod;
    unsigned int b = 1000000000;

    x = from_uint(b);
    scanf_value(&uint1);
    scanf_value(&uint2);
    sum = add_op(uint1, uint2);
    diff = subtr_op(uint1, uint2);
    prod = mult_op(uint1, uint2);
    printf("Generated from unsigned int: ");
    printf_uint(x);
    printf("First uint1024_t generated from string: ");
    printf_uint(uint1);
    printf("Second uint1024_t generated from string: ");
    printf_uint(uint2);
    printf("1st_uint1024_t + 2nd_uint1024_t: ");
    printf_uint(sum);
    printf("1st_uint1024_t - 2nd_uint1024_t: ");
    printf_uint(diff);
    printf("1st_uint1024_t * 2nd_uint1024_t: ");
    printf_uint(prod);
}

