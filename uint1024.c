#include <stdio.h>
#include <malloc.h>
#include <wchar.h>

typedef struct uint1024
{
    char *string;
    short UndefinedBehavior;

} uint1024;

uint1024 from_uint(unsigned int x)
{
    uint1024 generated;

    generated.string = (char *)(malloc(308 * sizeof(char)));

    char *str = (char *)(malloc(10 * sizeof(char)));

    for (int i = 9; i >= 0; i++)
    {
        str[i] = '0' + x % 10;
        x /= 10;
    }

    int detnull = 0;

    for (detnull; generated.string[detnull] != '\0'; detnull++)
        ;
    detnull--;

    for (int i = 0; detnull >= i; i++)
    {
        generated.string[307 - i] = generated.string[detnull - i];
    }

    for (int i = 307 - detnull - 1; i >= 0; i--)
    {
        generated.string[i] = '0';
    }

    return generated;
}

int length(uint1024 x)
{
    int counter = 0;

    for (int i = 0; x.string[i] != '\0'; i++)
        counter++;

    return counter;
}

void printf_value(uint1024 x)
{
    int idx = 0;

    int IsZero = 1;

    int IsError = 1;

    for (int i = 0; i < length(x); i++)
    {
        if (x.string[i] != '0' && IsZero == 1)
            IsZero = 0;

        if (x.string[i] != 'E' && IsError == 1)
            IsError = 0;
    }

    for (idx; x.string[idx] == '0' && idx < length(x); idx++)
        ;

    if (IsZero && !IsError)
        printf("%c", x.string[0]);
    else
        for (int i = idx; i < length(x); i++)
            printf("%c", x.string[i]);
}

int CheckIfNumber(uint1024 *x)
{
    int flag = 1;

    for (int i = 0; i < 308; i++)
        if (!(x->string[i] >= '0' & x->string[i] <= '9'))
        {
            flag = 0;
            break;
        }

    if (flag)
        return 1;
    else
    {
        return 0;
    }
}

void scanf_value(uint1024 *x)
{
    x->UndefinedBehavior = 0;

    x->string = (char *)(malloc(309 * sizeof(char)));

    scanf("%s", x->string);

    int detnull = 0;

    for (detnull; x->string[detnull] != '\0'; detnull++)
        ;
    detnull--;

    if (detnull != 307)
    {
        for (int i = 0; detnull >= i; i++)
        {
            x->string[307 - i] = x->string[detnull - i];
        }

        for (int i = 307 - detnull - 1; i >= 0; i--)
        {
            x->string[i] = '0';
        }

        x->string = (char *)(realloc(x->string, 308 * sizeof(char)));

        if (!CheckIfNumber(x))
        {
            x->UndefinedBehavior = 1;
            printf("\nIt's not a number\nUndefined behavior");
            return;
        }
    }

    else if (detnull == 308)
    {
        x->UndefinedBehavior = 1;
        printf("\nUndefined Behavior");
    }
    else
    {
        if (!CheckIfNumber(x))
        {
            x->UndefinedBehavior = 1;
            printf("\nIt's not a number\nUndefined behavior");
            return;
        }
        x->string = (char *)(realloc(x->string, 308 * sizeof(char)));
    }
}

uint1024 add_op(uint1024 *num1, uint1024 *num2)
{
    int DigitShift = 0;
    for (int i = 0; i < 308; i++)
    {
        int CurDigit = (num1->string[307 - i] - '0' + DigitShift + num2->string[307 - i] - '0');
        if (i != 307)
        {
            DigitShift = CurDigit / 10;
            num1->string[307 - i] = '0' + CurDigit % 10;
        }
        else if (CurDigit >= 10 && i == 307)
        {
            printf("%s", "\nUndefined Behavior");
            num1->UndefinedBehavior = 1;
        }
        else
        {
            num1->string[307 - i] = '0' + CurDigit % 10;
        }
    }
    if (num1->UndefinedBehavior == 0)
        return *num1;
    else
    {
        memcpy(num1->string, "\nError\n", 307 * sizeof(char));
        return *num1;
    }
}

uint1024 subtr_op(uint1024 *num1, uint1024 *num2)
{
    for (int i = 0; i < 308; i++)
    {
        int CurDigit = (num1->string[307 - i] - num2->string[307 - i]);
        if (i != 307)
        {
            if (CurDigit < 0)
            {
                CurDigit *= -1;
                num1->string[307 - i - 1] -= 1;
                num1->string[307 - i] = '0' + CurDigit;
            }
            else
            {
                num1->string[307 - i] = '0' + CurDigit;
            }
        }
        else if (CurDigit < 0)
        {
            printf("%s", "\nUndefined Behavior");
            num1->UndefinedBehavior = 1;
        }
    }
    if (num1->UndefinedBehavior == 0)
    {
        return *num1;
    }
    else
    {
        memcpy(num1->string, "\nError\n", 307 * sizeof(char));
        return *num1;
    }
}

uint1024 mult_op(uint1024 *num1, uint1024 *num2)
{
    int DigitShift = 0;

    int CurDigit = 0, SectionDigitShift = 0;

    char *result = (char *)(calloc(615, sizeof(char))); // 64 * 15

    for (int i = 307; i >= 0; i--)
    {
        if (num1->UndefinedBehavior == 1)
            break;

        for (int j = 307; j >= 0; j--)
            if (i + j != 307)
            {
                CurDigit = (int)((num1->string[i] - '0') * (num2->string[j] - '0'));

                if (result[i + j] >= '0' && result[i + j] <= '9')
                    SectionDigitShift = (int)(result[i + j] - '0') + CurDigit % 10 + DigitShift;
                else
                    SectionDigitShift = CurDigit % 10 + DigitShift;

                result[i + j] = '0' + (SectionDigitShift) % 10;

                DigitShift = CurDigit / 10 + SectionDigitShift / 10;

                SectionDigitShift = 0;
            }
            else
            {
                CurDigit = (int)((num1->string[i] - '0') * (num2->string[j] - '0'));

                if (result[i + j] >= '0' && result[i + j] <= '9')
                    SectionDigitShift = (int)(result[i + j] - '0') + CurDigit % 10 + DigitShift;
                else
                    SectionDigitShift = CurDigit % 10 + DigitShift;

                if (SectionDigitShift > 10)
                {
                    printf("\nUndefined Behavior");
                    num1->UndefinedBehavior = 1;
                    break;
                }
                else
                    result[i + j] = '0' + (SectionDigitShift) % 10;

                DigitShift = CurDigit / 10 + SectionDigitShift / 10;

                SectionDigitShift = 0;
            }
    }

    for (int i = 0; result[i] < '0' || result[i] > '9'; i++)
        result[i] = '0';

    free(num1->string);
    num1->string = result;
    result = NULL;

    if (num1->UndefinedBehavior == 0)
    {
        return *num1;
    }
    else
    {
        memcpy(num1->string, "\nError\n", 307 * sizeof(char));
        return *num1;
    }
}

int main()
{
    uint1024 number1;
    uint1024 number2;
    char sign;

    scanf_value(&number1);
    scanf("%s", &sign);
    scanf_value(&number2);

    switch (sign)
    {
    case '+':
        printf_value(add_op(&number1, &number2));
        break;

    case '-':
        printf_value(subtr_op(&number1, &number2));
        break;

    case '*':
        printf_value(mult_op(&number1, &number2));
        break;
    }

    return 0;
}

// 24
// 64

//     24    16
// 12  8
// ___________
// 15  3    6