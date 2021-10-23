#include <stdio.h>
#include <math.h>
#include <string.h>
static int maximum = 1e9;
static int base = 9;
static int max_index = 34;
typedef struct {
    int array[35];
    int cells;
} uint1024_t;
void init(uint1024_t* x)
{
    for (int i = 0; i < max_index + 1; i++)
        x -> array[i] = 0;
}
uint1024_t from_uint(unsigned int x)
{
    uint1024_t res;
    int i;
    init (&res);
    if (x < maximum)
    {
        res.cells = 1;
        res.array[0] = x;
    }
    else
    {
        res.cells = 2;
        res.array[0] = x % maximum;
        res.array[1] = x / maximum;
    }
    return res;
}
void scanf_value(uint1024_t *x)
{
    char input[310];
    int i, numbers = 0;
    for (i = 0; i <= max_index; i++)
        x->array[i] = 0;
    scanf("%309s", input);
    i = strlen(input) - 1;
    while (i >= 0)
    {
        x->array[numbers / base] += (input[i] - '0') * pow (10, numbers % base);
        i--;
        numbers++;
    }
    x->cells = (numbers - 1) / base + 1;
}
void printf_value(uint1024_t x)
{
    int i;
    for (i = x.cells - 1; i >= 0; i--)
    {
        printf("%i", x.array[i]);
    }
    printf("\n");
}
uint1024_t add_op(uint1024_t x, uint1024_t y)
{
    uint1024_t sum;
    long long temp = 0;
    int cells_sum = 1, i;
    for (i = 0; i <= max_index; i++)
    {
        temp = (long long)(x.array[i] + y.array[i]) + temp;
        sum.array[i] = temp % maximum;
        temp /= maximum;
    }
    for (i = max_index; i >= 0; i--)
        if (sum.array[i] != 0)
        {
            cells_sum = i + 1;
            break;
        }
    sum.cells = cells_sum;
    return sum;
}
uint1024_t subtr_op(uint1024_t x, uint1024_t y)
{
    uint1024_t diff;
    long long temp = 0;
    int cells_subtr = 1, i;
    for (i = 0; i <= max_index; i++)
    {
        temp = (long long)(x.array[i] - y.array[i]) - temp;
        if (temp >= 0)
        {
            diff.array[i] = temp;
            temp = 0;
        }
        else
        {
            diff.array[i] = maximum + temp;
            temp = 1;
        }
    }
    for (i = max_index; i >= 0; i--)
        if (diff.array[i] != 0)
        {
            cells_subtr = i + 1;
            break;
        }
    diff.cells = cells_subtr;
    return diff;
}
uint1024_t mult_op(uint1024_t x, uint1024_t y)
{
    uint1024_t prod;
    long long temp = 0;
    int cells_mult = 1, i, j;
    for (i = 34; i >= 0; i--)
        prod.array[i] = 0;
    for (i = 0; i < 35; i++)
        for (j = 0; i + j < 35; j++)
        {
            temp = (long long)(x.array[i]) * (long long)y.array[j] + temp + prod.array[i + j];
            prod.array[i + j] = temp % maximum;
            temp = temp / maximum;
        }
    for (i = 34; i >= 0; i--)
        if (prod.array[i] != 0)
        {
            cells_mult = i + 1;
            break;
        }
    prod.cells = cells_mult;
    return prod;
}

int main()
{
    uint1024_t uint1024_1, uint1024_2, x, sum, diff, prod;
    char a;
    printf("First number ");
    scanf_value(&uint1024_1);
    printf("Second number ");
    scanf_value(&uint1024_2);
    printf("Operation(+, -, *) ");

    
    sum = add_op(uint1024_1, uint1024_2);
    diff = subtr_op(uint1024_1, uint1024_2);
    prod = mult_op(uint1024_1, uint1024_2);
    scanf("%s",&a); 
    printf_value(uint1024_2);
    if (a == '+')
    {
        printf("Sum of numbers ");
        printf_value(sum);
    }
    if (a == '-')
    {
        printf("Difference of numbers ");
        printf_value(diff);
    }
    if (a == '*')
    {
        printf("Product of numbers ");
        printf_value(prod);
    }
    return 0;
    getchar();
    getchar();
}
