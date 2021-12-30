#include <stdio.h>
#include <string.h>

unsigned long long INT_MAX = 4294967296;
typedef struct
{
    unsigned int list[32];
} uint1024_t;

uint1024_t uint1024_t_from_uint(unsigned int x)
{
    uint1024_t result;
    for (int i = 0; i < 31; i++)
    {
        result.list[i] = 0;
    }
    result.list[31] = x;
    return result;
}
uint1024_t add_op(uint1024_t x, uint1024_t y)
{
    uint1024_t result = uint1024_t_from_uint(0);
    for (int i = 31; i > 0; i--)
    {
        result.list[i] += x.list[i] + y.list[i];
        if ((result.list[i] < x.list[i]) && (result.list[i] < y.list[i]))
        {
            result.list[i - 1]++;
        }
    }
    result.list[0] += x.list[0] + y.list[0];
    return result;
}
uint1024_t subtr_op(uint1024_t x, uint1024_t y)
{
    uint1024_t result = uint1024_t_from_uint(0);
    for (int i = 31; i > 0; i--)
    {
        result.list[i] += x.list[i] - y.list[i];
        if (result.list[i] > x.list[i])
        {
            result.list[i - 1]--;
        }
    }
    result.list[0] += x.list[0] - y.list[0];
    return result;
}
uint1024_t mult_at_uint(uint1024_t x, unsigned y)
{
    uint1024_t result = uint1024_t_from_uint(0);
    for (int i = 31; i >= 0; i--)
    {
        unsigned long long xlong = x.list[i];
        unsigned long long ylong = y;
        unsigned long long multiply = xlong * ylong;
        result.list[i] += multiply % INT_MAX;
        if (i != 0)
        {
            result.list[i - 1] += multiply / INT_MAX;
        }
    }
    return result;
}
uint1024_t div_at_uint(uint1024_t x, unsigned y)
{
    uint1024_t result = uint1024_t_from_uint(0);
    unsigned long long Reml = 0;
    for (int i = 0; i < 32; i++)
    {
        result.list[i] = (Reml * INT_MAX + x.list[i]) / y;
        Reml = (Reml * INT_MAX + x.list[i]) % y;
    }
    return result;
}
unsigned mod_at_uint(uint1024_t x, unsigned y)
{
    uint1024_t result = uint1024_t_from_uint(0);
    unsigned long long Reml = 0;
    for (int i = 0; i < 32; i++)
    {
        result.list[i] = (Reml * INT_MAX + x.list[i]) / y;
        Reml = (Reml * INT_MAX + x.list[i]) % y;
    }
    return Reml;
}
uint1024_t mult_op(uint1024_t x, uint1024_t y)
{
    uint1024_t result = uint1024_t_from_uint(0);
    for (int i = 31; i >= 0; i--)
    {
        if (y.list[i] != 0)
        {
            uint1024_t Tempelate = mult_at_uint(x, y.list[i]);
            int k = 31 - i;
            for (int j = k; j < 32; j++)
            {
                Tempelate.list[j - k] = Tempelate.list[j];
            }
            for (int j = 32 - k; j < 32; j++)
            {
                Tempelate.list[j] = 0;
            }
            result = add_op(result, Tempelate);
        }
    }
    return result;
}
void printf_value(uint1024_t x)
{
    char arr[1024] = {0};
    for (int i = 0; i < 1024; i++)
    {
        unsigned mod = mod_at_uint(x, 10);
        arr[1023 - i] = mod + '0';
        x = div_at_uint(x, 10);
    }
    unsigned begin = 0;
    while (arr[begin] == '0')
    {
        begin++;
    }
    if (begin == 1024)
    {
        printf("0");
    }
    for (int i = begin; i < 1024; i++)
    {
        printf("%c", arr[i]);
    }
}
void scanf_value(uint1024_t *x)
{
    char arr[1024];
    scanf("%s", arr);
    *x = uint1024_t_from_uint(0);

    for (int i = 0; i < 1024 && arr[i] != '\0'; i++)
    {
        *x = mult_at_uint(*x, 10);
        uint1024_t Tempelate = uint1024_t_from_uint(arr[i] - '0');
        *x = add_op(*x, Tempelate);
    }
}
int main()
{
    uint1024_t a, b;
    printf("First number: ");

    scanf_value(&a);

    printf("Second number: ");

    scanf_value(&b);

    printf("\nSum is: ");

    printf_value(add_op(a, b));

    printf("\nSub is: ");

    printf_value(subtr_op(a, b));

    printf("\nMul is: ");

    printf_value(mult_op(a, b));
    return 0;
}