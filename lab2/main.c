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
    }   
    else
    {
        printf("%d", x.t[size-1]);
        for (int i = size - 2; i >= 0; i--)
        {
            printf("%09d", x.t[i]);
        }
    }
}

void scanf_value(uint1024_t* x)
{
    char str[1024];
    scanf("%s", str);

    int len = strlen(str);
    int size;

    if (len % 9 ==0)
    {
        size = len / 9;
    }   else
    {
        size = len / 9 + 1;
    }

    uint1024_t res;
    res.t = malloc(size*sizeof(int32_t));
    res.size = size;

    for (int i = len, j = 0; i > 0; i -= 9, j++) 
    {
	    str[i] = '\0';
	    res.t[j] = (atoi (i>=9 ? str+i-9 : str));
    }

    *x = res;
}

int main(int argc, char* argv[])
{
    uint1024_t t1, t2;
    char buf[310];
    memset(buf, 0, 310);
    printf("Enter number 1\n");
    scanf_value(&t1);
    printf("Enter number 2\n");
    scanf_value(&t2);
    fseek(stdin,0,SEEK_END);
    printf("Choose operation: + - * \n");
    scanf("%c", buf);
    if (!strcmp(buf, "*")) t1 = mult_op(t1, t2);
    else if (!strcmp(buf, "-")) t1 = subtr_op(t1, t2);
    else if (!strcmp(buf, "+")) t1 = add_op(t1, t2);
    printf_value(t1);
    fseek(stdin,0,SEEK_END);
 
 
    return 0;
}