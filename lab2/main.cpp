#include <stdio.h>
#include <math.h>

#define BASE 1000000000
#define SIZE 35
#define STRLEN 310

typedef struct {
    int a[SIZE];
    int rank;
} uint1024;

void Init(uint1024* x)
{
    for (int i = 0; i < SIZE; i++)
        x->a[i] = 0;
}

void Rank(uint1024* r)
{
    int rank;

    for (int i = SIZE - 1; i >= 0; i--)
        if (r->a[i] != 0)
        {
            rank = i + 1;
            break;
        }

    r->rank = rank;
}

void Scan(uint1024* x)
{
    char tstr[STRLEN];
    int i, digits = 0;

    Init(x);

    for (i = 0; i < STRLEN; i++)
        tstr[i] = '\0';

    scanf("%309s", tstr);

    --i;

    while (tstr[i] == '\0')
        --i;

    while (i >= 0)
    {
        x->a[digits / 9] += (tstr[i] - '0') * pow(10, digits % 9);
        --i;
        ++digits;
    }

    x->rank = (digits - 1) / 9 + 1;
}

uint1024 Fromuint1024(unsigned int x)
{
    uint1024 val;
    int digits = 0;

    Init(&val);

    while (x > 0)
    {
        val.a[digits / 9] += (x % 10) * pow(10, digits % 9);
        x /= 10;
        ++digits;
    }

    val.rank = (digits - 1) / 9 + 1;

    return val;
}

void Print(uint1024 x)
{
    int i, j;

    for (i = x.rank - 1; i >= 0; i--)
    {
        j = 8;

        while (i != x.rank - 1 && pow(10, j--) - x.a[i] > 0 && j >= 0)
            printf("0");

        printf("%i", x.a[i]);
    }
}

uint1024 Plus(uint1024 x, uint1024 y)
{
    uint1024 r;
    long long buff = 0;

    Init(&r);

    for (int i = 0; i < SIZE; i++)
    {
        buff = (long long)x.a[i] + (long long)y.a[i] + buff;
        r.a[i] = buff % BASE;
        buff /= BASE;
    }

    Rank(&r);

    return r;
}

uint1024 Minus(uint1024 x, uint1024 y)
{
    uint1024 r;
    long long buff = 0;

    Init(&r);

    for (int i = 0; i < SIZE; i++)
    {
        buff = (long long)x.a[i] - (long long)y.a[i] - buff;
        r.a[i] = buff >= 0 ? buff : BASE + buff;

        if (buff >= 0)
            buff = 0;
        else
            buff = 1;
    }

    Rank(&r);

    return r;
}

uint1024 Mult(uint1024 x, uint1024 y)
{
    uint1024 r;
    long long buff = 0, c = 0;

    Init(&r);

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; i + j < SIZE; j++)
        {
            buff = r.a[i + j] + (long long)x.a[i] * (long long)y.a[j] + c;
            r.a[i + j] = buff % BASE;
            c = buff / BASE;
        }

    Rank(&r);

    return r;
}

int main(void)
{
    uint1024 x, y;

    Scan(&x);
    Scan(&y);

    printf("\n\n|+| =\n");
    Print(Plus(x, y));
    printf("\n\n|-| =\n");
    Print(Minus(x, y));
    printf("\n\n|*| = \n");
    Print(Mult(x, y));
    return 0;
}