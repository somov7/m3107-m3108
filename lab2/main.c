#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef struct uint1024_t
{
    int *t;
    size_t size;
} uint1024_t;

const int notUndefined = 1000000000;

uint1024_t from_uint(unsigned int x)
{
    int size = 1;
    uint1024_t ans;

    if (x >= notUndefined)
    {
        size = 2;
    }

    ans.t = malloc(size * sizeof(int32_t));
    ans.size = size;
    ans.t[0] = x % notUndefined;

    if (size > 1)
    {
        ans.t[1] = x / notUndefined;
    }

    return ans;
}

uint1024_t add_op(uint1024_t x, uint1024_t y)
{
    int size;
    uint1024_t ans;

    if(x.size > y.size)
    {
        size = x.size;
    }   else
    {
        size = y.size;
    }

    ans.t = calloc(size, sizeof(int32_t));
    ans.size = size;

    int32_t buf = 0;

    for (int i = 0;  i < size || buf; i++)
    {
        if (i == size)
        {
            ans.t = realloc(ans.t, (size + 1) * sizeof(int32_t));
            ans.t[size] = 0;
            ans.size++;
        }

        ans.t[i] += buf;

        if (i < x.size)
        {
            ans.t[i] += x.t[i];
        }
        if (i < y.size)
        {
            ans.t[i] += y.t[i];
        }

        if (ans.t[i] >= notUndefined)
        {
            buf = 1;
            ans.t[i] -= notUndefined;
        }   else
        {
            buf = 0;
        }
    }

    return ans;
}

uint1024_t subtr_op(uint1024_t x, uint1024_t y)
{
    uint1024_t ans;
    int size = x.size;

    if (y.size > x.size)
    {
        printf("Undefined Behavior");
        exit(1);
    }

    ans.t = calloc(size, sizeof(int32_t));
    ans.size = size;

    int buf = 0;
    for (int i = 0;  i < size || buf; i++)
    {

        if (i == size)
        {
            break;
        }

        ans.t[i] = x.t[i] - buf;

        if (i < y.size)
        {
            ans.t[i] -= y.t[i];
        }
        if (ans.t < 0)
        {
            ans.t += notUndefined;
            buf = 1;
        }   else
        {
            buf = 0;
        }
    }

    while (ans.t[ans.size - 1] == 0 && ans.size > 1)
    {
        ans.size--;
    }
    ans.t = realloc(ans.t, ans.size * sizeof(int32_t));

    return ans;
}

uint1024_t mult_op(uint1024_t x, uint1024_t y)
{
    uint1024_t ans;
    int size = x.size + y.size;

    ans.t = calloc(size, sizeof(int32_t));
    ans.size = size;

    int buf;
    long long forResult;
    for (int i = 0; i < x.size; i++)
    {
        buf = 0;
        for (int j = 0; j < y.size || buf; j++)
        {
            forResult = ans.t[i + j];

            if (j < y.size)
            {
                forResult += 1ll * x.t[i] * y.t[j];
            }

            forResult += buf;
            ans.t[i + j] = forResult % notUndefined;
            buf = forResult / notUndefined;
        }
    }

    while (ans.t[size - 1] == 0 && size > 1) {
        size--;
    }

    ans.t = realloc(ans.t, size * sizeof(int32_t));
    ans.size = size;

    return ans;
}

void printf_value(uint1024_t x)
{
    int size = x.size;

    if (!size)
    {
        printf(0);
    }   else
    {
        for (int i = size - 1; i >= 0; i--)
        {
            printf("%d", x.t[i]);
        }
    }
}

void scanf_value(uint1024_t* x)
{
    char str[1024];
    scanf("%s", str);

    uint1024_t ans;
    int size;

    if (strlen(str) % 9 == 0)
    {
        size = strlen(str) / 9;
    }   else
    {
        size = strlen(str) / 9 + 1;
    }

    ans.t = malloc(size + sizeof(int32_t));
    ans.size = size;

    int i = sizeof(str);
    int j = 0;

    while (i > 0)
    {
        str[i] = '\0';
        
        if (i >= 9)
        {
            ans.t[j] = atoi(str + i - 9);
        }   else
        {
            ans.t[j] = atoi(str);
        }

        i -= 9;
        j++;
    }

    *x = ans;
}

int main(int argc, char* argv[])
{
    printf_value(from_uint(1111111111));
    printf("\n");
    uint1024_t x;
    uint1024_t y;
    scanf_value(&x);
    printf_value(x);
    scanf_value(&y);
    printf("\n");
    printf_value( add_op(from_uint(121111111), from_uint(1111111111)) );
    printf("\n");
    printf_value( subtr_op(from_uint(1211111111), from_uint(1111111111)) );
    printf("\n");
    printf_value( mult_op(x, y) );

    return 0;
}