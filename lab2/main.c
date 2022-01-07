#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define maximum 1000000000

typedef struct uint1024_t
{
    int *arr;
    size_t size;
} uint1024_t;

void scanf_value(uint1024_t *x)
{
    char str[311];
    int numb_length;
    scanf("%309s", str);
    numb_length = strlen(str);
    x->size = numb_length / 9 + (numb_length % 9 != 0);
    x->arr = malloc(x->size * sizeof(int32_t));
    int j = 0;
    for (int i = numb_length; i > 0; i -= 9)
    {
        str[i] = '\0';
        if (i >= 9)
        {
            x->arr[j] = atoi(str + i - 9);
        }
        else
        {
            x->arr[j] = atoi(str);
        }
        j++;
    }
}

uint1024_t from_uint(unsigned int x)
{
    uint1024_t result;
    if (x >= maximum)
    {
        result.arr = malloc(2 * sizeof(int32_t));
        result.arr[0] = x % maximum;
        result.size = 2;
        result.arr[1] = x / maximum;
    }
    else
    {
        result.arr = malloc(1 * sizeof(int32_t));
        result.size = 1;
        result.arr[0] = x % maximum;
    }
    return result;
}

uint1024_t add_op(uint1024_t x, uint1024_t y) {
    int size;
    uint1024_t result;
    int full = 0;
    if (x.size > y.size) {
        size = x.size;
    } else {
        size = y.size;
    }

    result.arr = calloc(size, sizeof(int32_t));
    result.size = size;
    for(int i = 0; i < size; i++)
    {
        if (i == size)
        {
            result.arr = realloc(result.arr, (size + 1) * sizeof(int32_t));
            result.arr[size] = 0;
            result.size++;
        }
        result.arr[i] += full;
        if (i < x.size)
        {
            result.arr[i] += x.arr[i];
        }
        if (i < y.size)
        {
            result.arr[i] += y.arr[i];
        }
        if (result.arr[i] >= maximum)
        {
            full = 1;
            result.arr[i] -= maximum;
        }
        else
        {
            full = 0;
        }
    }
    if(full)
    {
        result.arr = realloc(result.arr, (size + 1) * sizeof(int32_t));
        result.arr[size] = 0;
        result.size++;
        result.arr[size] += full;
    }
    return result;
}

uint1024_t subtr_op(uint1024_t x, uint1024_t y)
{
    uint1024_t result;
    int size = x.size;
    int full = 0;
    result.arr = calloc(size, sizeof(int32_t));
    result.size = size;
    for(int i = 0; i < size; i++)
    {
        result.arr[i] = x.arr[i] - full;
        if (i < y.size)
        {
            result.arr[i] -= y.arr[i];
        }
        if (result.arr[i] < 0)
        {
            full = 1;
            result.arr[i] += maximum;
        }
        else
        {
            full = 0;
        }
    }
    while (result.arr[result.size - 1] == 0 && result.size > 1)
    {
        result.size--;
    }
    if(result.size != size)
        result.arr = realloc(result.arr, result.size * sizeof(int32_t));
    return result;
}

uint1024_t mult_op(uint1024_t x, uint1024_t y) {
    int full, perenos_y;
    long long numb;
    uint1024_t result;
    result.size = x.size + y.size;
    result.arr = calloc(result.size, sizeof(int32_t));
    for (int i = 0; i < x.size; i++)
    {
        full = 0;
        perenos_y = y.size;
        for(int j = 0; j < y.size; j++ || full)
        {
            numb = result.arr[i + j];
            numb += (long long) x.arr[i] * y.arr[j];
            numb += full;
            result.arr[i + j] = numb % maximum;
            full = numb / maximum;
        }
        while(full != 0)
        {
            numb = result.arr[i + perenos_y];
            numb += full;
            result.arr[i + perenos_y] = numb % maximum;
            full = numb / maximum;
            perenos_y++;
        }
    }
    int size = result.size;
    while (result.arr[result.size - 1] == 0 && result.size > 1)
    {
        result.size--;
    }
    if(result.size != size)
        result.arr = realloc(result.arr, result.size * sizeof(int32_t));
    return result;
}
void printf_value(uint1024_t x)
{
    if(x.size)
    {
        printf("%d", x.arr[x.size - 1]);
    }
    else
    {
        printf(0);
    }
    for (int i = x.size - 2; i >= 0; --i)
    {
        printf("%09d", x.arr[i]);
    }
}

int main()
{
    printf("Helly evrybady \n");
    char op;
    uint1024_t numb1, numb2, result;
    printf("Enter first numb: \n");
    scanf_value(&numb1);
    printf("Enter second numb: \n");
    scanf_value(&numb2);
    while (1)
    {
        printf("Enter which operation we need:");
        scanf("%s", &op);
        if (op == '+')
        {
            result = add_op(numb1, numb2);
        }
        else if (op == '-')
        {
            result = subtr_op(numb1, numb2);
        }
        else if (op == '*')
        {
            result = mult_op(numb1, numb2);
        }
        else
            {
            return 0;
        }
        printf("Result: \n");
        printf_value(result);
        printf("\n\n");
    }
}
