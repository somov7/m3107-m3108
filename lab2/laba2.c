#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#define max(a,b) (((a) > (b)) ? (a) : (b))

const int base = 1e9;

typedef struct 
{
    int32_t* t;
    size_t size;
} uint1024_t;

uint1024_t from_uint(unsigned int x)
{
    int sz;
    if (x >= base) sz = 2;
    else sz = 1;
    uint1024_t t;
    t.t = malloc(sz*sizeof(int32_t));
    t.size = sz;
    t.t[0] = x % base;
    if (sz > 1) t.t[1] = x / base;
    return t;
}

uint1024_t add_op(uint1024_t x, uint1024_t y)
{
    uint1024_t res;
    int sz = max(x.size, y.size);
    res.t = calloc(sz, sizeof(int32_t));
    res.size = sz;
    int32_t over = 0;
    for (int i = 0 ; i < sz || over; i++)
    {
        if (i == sz) 
        {
            realloc(res.t, (sz+1)*sizeof(int32_t));
            res.t[sz] = 0;
            res.size++;
        }
        res.t[i] = over + (i < x.size ? x.t[i] : 0) + (i < y.size ? y.t[i] : 0);
        if (res.t[i] >= base) over = 1, res.t[i] -= base;
        else over = 0;
    }
    return res;
}

uint1024_t subtr_op(uint1024_t x, uint1024_t y)
{
    uint1024_t res;
    int sz = max(x.size, y.size);
    res.t = calloc(sz, sizeof(int32_t));
    res.size = sz;
    int over = 0;
    for (int i = 0; i < sz || over; i++)
    {
        if (i == sz)
        {
            realloc(res.t, (sz+1)*sizeof(int32_t));
            res.t[sz] = 0;
            res.size++;
        }
        res.t[i] = x.t[i] - (i < y.size ? y.t[i] : 0) - over;
        if (res.t[i] < 0) over = 1, res.t[i] += base;
        else over = 0;
    }
    while (res.t[sz-1] == 0) sz--;
    realloc(res.t, sz*sizeof(int32_t));
    res.size = sz;
    return res;
}

uint1024_t mult_op(uint1024_t x, uint1024_t y)
{
   uint1024_t res;
   int sz = x.size + y.size;
   res.t = calloc(sz,sizeof(int32_t));
   res.size = sz;
   for (int i = 0; i < x.size; i++)
        for (int j = 0, over = 0; j < y.size || over; j++)
        {
            long long t = res.t[i+j] + x.t[i] * 1ll * (j < y.size ? y.t[j] : 0) + over; //*1ll to convert x.t to long long (auto type conversion)
            res.t[i+j] = (int) (t % base);
            over = (int) (t / base);
        }
    
    while (res.t[sz-1] == 0 && sz > 1) sz--;
    realloc(res.t, sz*sizeof(int32_t));
    res.size = sz;
    return res;
}

void printf_value(uint1024_t x)
{
    int sz = x.size;
    printf ("%d", !sz ? 0 : x.t[sz-1]); //print the most significant digit if it exists to avoid 09 padding with zeros
    for (int i=sz-2; i>=0; --i) //now just print the rest most signifacnt digits are at the end of array
	    printf ("%09d", x.t[i]);
}

void scanf_value(uint1024_t* x)
{
    char str[309]; // >1024 bits? undefined behavior (max 309, one more for null terminating)
    scanf("%s", str);
    int len = strlen(str);
    int size = len % 9 == 0 ? len / 9 : len / 9 + 1;
    uint1024_t res;
    res.t = malloc(size*sizeof(int32_t));
    res.size = size;
    for (int i = len, j = 0; i > 0; i -= 9, j++) 
    {
	    str[i] = '\0'; //null terminator
	    res.t[j] = (atoi (i>=9 ? str+i-9 : str)); //get 9 digits at a time from the end, if i is less than 9, get the remaining string
    }
    *x = res;
}

int main()
{
    uint1024_t t1, t2;
    char buf[310];
    memset(buf, 0, 310);
    printf("Enter number 1\n");
    scanf_value(&t1);
    printf("Enter number 2\n");
    scanf_value(&t2);
    fseek(stdin,0,SEEK_END);
    while (1)
    {
        printf("Choose operation: + - * \nType any character to stop\n");
        scanf("%c", buf);
        if (!strcmp(buf, "*")) t1 = mult_op(t1, t2);
        else if (!strcmp(buf, "-")) t1 = subtr_op(t1, t2);
        else if (!strcmp(buf, "+")) t1 = add_op(t1, t2);
        else break;
        printf_value(t1);
        fseek(stdin,0,SEEK_END);
        printf("\nEnter number\n");
        scanf_value(&t2);
        fseek(stdin,0,SEEK_END);
    }

    return 0;
}
