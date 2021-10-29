#include <stdio.h>
#include <math.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>

const unsigned long long restriction = 1e9;

/**
 * @brief
 * i'll keep dynamic array of int
 */
typedef struct
{
    int *array;

    int size_of_array;

    bool IsError;

} uint1024;

void init(uint1024 *x)
{
    x->size_of_array = 0;

    x->IsError = 0;
}

uint1024 from_uint(unsigned int x)
{
    uint1024 generated;

    init(&generated);

    (x >= restriction) ? (generated.array = (int *)malloc(2 * sizeof(int))) : (generated.array = (int *)malloc(sizeof(int)));

    if (generated.array == NULL)
    {
        printf("\nSome kinda error occured during memory allocating\n");
        generated.IsError = 1;
        memcpy(generated.array, "Error", 5);
    }
    else
    {
        generated.size_of_array = sizeof(generated.size_of_array) / sizeof(int);

        generated.array[0] = x % restriction;

        if (generated.size_of_array > 1)
            generated.array[1] = x / restriction;
    }

    return generated;
}

void scanf_value(uint1024 *x)
{
    init(x);

    char *str = (char *)calloc(309, sizeof(char));

    scanf("%s", str);

    if (strlen(str) == 309)
    {
        x->IsError = 1;

        printf("\nOut of range!\nUndefined Behavior\n");

        memcpy(x->array, "Error", 5);

        return;
    }

    if (x->IsError != 1)
    {

        str = (char *)realloc(str, 308 * sizeof(char));

        if (strlen(str) % 9 == 0)
            x->size_of_array = strlen(str) / 9;
        else
            x->size_of_array = strlen(str) / 9 + 1;

        x->array = (int *)calloc(x->size_of_array, sizeof(int));

        int idx = 0;
        for (int i = strlen(str); i >= 0; i -= 9)
        {
            str[i] = '\0';

            x->array[idx++] = (atoi((i >= 9) ? (str + i - 9) : str));
        }
    }
    free(str);
}

void printf_value(uint1024 x)
{
    if (x.IsError != 1)
        for (int i = x.size_of_array - 1; i >= 0; i--)
        {
            if (i == x.size_of_array - 1)
                printf("%d", x.array[i]);
            else
                printf("%09d", x.array[i]);
        }
}

uint1024 add_op(uint1024 x, uint1024 y)
{
    uint1024 sum;

    init(&sum);

    sum.size_of_array = fmax(x.size_of_array, y.size_of_array);

    sum.array = (int *)calloc(sum.size_of_array, sizeof(int));

    int DigitShift = 0;

    for (int i = 0; i < sum.size_of_array || DigitShift; i++)
    {
        if (i == sum.size_of_array)
        {
            sum.array = (int *)(realloc(sum.array, ++sum.size_of_array));

            sum.array[sum.size_of_array - 1] = 0;
        }

        sum.array[i] = DigitShift + ((i < x.size_of_array) ? (x.array[i]) : (0LL)) + ((i < y.size_of_array) ? (y.array[i]) : (0LL));

        if (sum.array[i] >= restriction)
        {
            DigitShift = sum.array[i] / restriction;

            sum.array[i] = sum.array[i] % restriction;
        }
        else
            DigitShift = 0;
    }

    return sum;
}

uint1024 subtr_op(uint1024 x, uint1024 y)
{
    uint1024 difference;

    init(&difference);

    difference.size_of_array = fmax(x.size_of_array, y.size_of_array);

    difference.array = (int *)calloc(difference.size_of_array, sizeof(int));

    int DigitShift = 0;

    for (int i = 0; i < difference.size_of_array || DigitShift; i++)
    {
        if (i == difference.size_of_array)
        {
            break;
        }

        difference.array[i] = ((i < x.size_of_array) ? (x.array[i]) : (0)) - ((i < y.size_of_array) ? (y.array[i]) : (0)) - DigitShift;

        if (difference.array[i] < 0)
        {
            DigitShift = 1;

            difference.array[i] += restriction;
        }
        else
            DigitShift = 0;
    }

    while (difference.array[difference.size_of_array - 1] == 0 && difference.size_of_array > 1)
        difference.size_of_array--;

    difference.array = (int *)(realloc(difference.array, difference.size_of_array * sizeof(int)));

    return difference;
}

uint1024 mult_op(uint1024 x, uint1024 y)
{
    uint1024 product;

    init(&product);

    product.size_of_array = x.size_of_array + y.size_of_array;

    product.array = (int *)calloc(product.size_of_array, sizeof(int));

    unsigned long long CurrentDigit = 0, DigitShift = 0;

    for (int i = 0; i < x.size_of_array; i++)
        for (int j = 0; j < y.size_of_array || DigitShift; j++)
        {
            CurrentDigit = product.array[i + j] + (unsigned long long)x.array[i] * ((j < y.size_of_array) ? (y.array[j]) : (0)) + DigitShift;

            product.array[i + j] = (int)(CurrentDigit % restriction);

            DigitShift = (int)(CurrentDigit / restriction);
        }

    while (product.array[product.size_of_array - 1] == 0 && product.size_of_array > 1)
        product.size_of_array--;

    product.array = (int *)realloc(product.array, product.size_of_array * sizeof(int));

    return product;
}

uint1024 div_by_small(uint1024 x, int y)
{
    if (y == 0)
    {
        printf("\nDivision by null!\n");

        return x;
    }

    uint1024 temp;

    temp.array = (int *)(calloc(x.size_of_array, sizeof(int)));

    for (int i = 0; i < x.size_of_array; i++)
    {
        temp.array[i] = x.array[i];
    }

    for (int i = x.size_of_array - 1; i >= 0; i--)
    {
        if (i)
            temp.array[i - 1] += (temp.array[i] % y) * restriction;

        temp.array[i] /= y;
    }

    while (temp.array[x.size_of_array - 1] == 0 && x.size_of_array > 1)
        x.size_of_array--;

    temp.array = (int *)realloc(temp.array, x.size_of_array * sizeof(int));
    temp.size_of_array = x.size_of_array;

    return temp;
}

short large_number_comparator(uint1024 x, uint1024 y)
{
    int sizex = 0, sizey = 0;

    int x_cell, y_cell;

    for (int i = 0; i < x.size_of_array; i++)
    {
        x_cell = x.array[i];

        while (x_cell > 0)
        {
            sizex++;
            x_cell /= 10;
        }
    }

    for (int i = 0; i < y.size_of_array; i++)
    {
        y_cell = y.array[i];

        while (y_cell > 0)
        {
            sizey++;
            y_cell /= 10;
        }
    }

    if (sizex > sizey)
        return 1;
    else if (sizex < sizey)
        return 2;
    else
    {
        short flag = 0;

        for (int i = x.size_of_array - 1; i >= 0; i--)
        {
            if (x.array[i] > y.array[i])
            {
                flag = 1;
                break;
            }
            else if (x.array[i] < y.array[i])
            {
                flag = 2;
                break;
            }
        }

        if (flag == 1)
            return 1;
        else if (flag == 2)
            return 2;
        else
            return 0;
    }
}

void assign(uint1024 *x, uint1024 y)
{
    x->size_of_array = y.size_of_array;

    x->array = (int *)(realloc(x->array, x->size_of_array * sizeof(int)));

    for (int i = 0; i < x->size_of_array; i++)
    {
        x->array[i] = y.array[i];
    }
}

uint1024 div_by_large(uint1024 x, uint1024 y)
{
    uint1024 left, mid, right;

    right.array = (int *)(calloc(x.size_of_array, sizeof(int)));
    right.size_of_array = x.size_of_array;

    for (int i = 0; i < right.size_of_array; i++)
    {
        right.array[i] = x.array[i];
    }

    int size = fmax(x.size_of_array, y.size_of_array);

    left.array = (int *)(calloc(size, sizeof(int)));
    left.size_of_array = 0;

    mid.array = (int *)(calloc(size, sizeof(int)));
    mid.size_of_array = 0;

    bool IsExecuted = 0;

    while (large_number_comparator(x, left) == 1)
    {
        if (IsExecuted)
            break;

        assign(&mid, add_op(x, left));
        mid = div_by_small(mid, 2);

        if (large_number_comparator(mult_op(mid, y), right) == 1)
            assign(&x, mid);

        else if (large_number_comparator(mult_op(mid, y), right) == 2)
            assign(&left, mid);

        else if (large_number_comparator(mult_op(mid, y), right) == 0)
        {
            assign(&left, mid);

            IsExecuted = 1;
        }
    }

    if (large_number_comparator(mult_op(left, y), right) == 0)
        return left;
    else if (large_number_comparator(mult_op(x, y), right) == 0)
        return x;
}

int main()
{
    uint1024 number1, number2;

    scanf_value(&number1);

    if (number1.IsError != 1)
    {
        scanf_value(&number2);

        if (number2.IsError != 1)
        {

            int number_to_div_by;
            scanf("%d", &number_to_div_by);

            printf("\nsum is: ");
            printf_value(add_op(number1, number2));
            printf("\n");

            printf("\ndifference is: ");
            printf_value(subtr_op(number1, number2));
            printf("\n");

            printf("\nproduct is: ");
            printf_value(mult_op(number1, number2));
            printf("\n");

            printf("\ndivision result for number1 is: ");
            printf_value(div_by_small(number1, number_to_div_by));
            printf("\n");

            printf("\ndivision result for number2 is: ");
            printf_value(div_by_small(number2, number_to_div_by));
            printf("\n");

            printf("\ndivision number1/number2 : ");
            printf_value(div_by_large(number1, number2));
            printf("\n");
        }
        else
        {
            free(number1.array);
            free(number2.array);
            return -1;
        }
    }
    else
    {
        free(number1.array);
        free(number2.array);
        return -1;
    }

    free(number1.array);
    free(number2.array);
    return 0;
}
/**
999999999   00000000     00000000     000000000     00000000
999         00    00     00    00     00     00     00    00
999         00    00     00    00     00     00     00    00
999999999   00   00      00   00      00     00     00   00
999         0000000      0000000      00     00     0000000
999         00    000    00    000    00     00     00    000
999999999   00     000   00     000   000000000     00     000
**/