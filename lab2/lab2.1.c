define _CRT_SECURE_NO_WARNINGS

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#define Findmax(a,b) (((a) > (b)) ? (a) : (b))
const int cc = 1e9;  //radix

typedef struct
{
    int32_t* int32;
    size_t size;
} uint1024_t;

uint1024_t from_uint(unsigned int x) 
{
    int size;
    if (x >= cc) size = 2;
    else size = 1;
    uint1024_t t;
    t.int32 = malloc(size * sizeof(int32_t));// size * sizeof(int32_t)
    t.size = size;
    t.int32[0] = x % cc;
    if (size > 1) t.int32[1] = x / cc;
    return t;
}

void printf_value(uint1024_t x)//вывод
{
    int size = x.size;
    printf("%d", !size ? 0 : x.int32[size - 1]); //display significant digits without 9 zeros
    for (int index = size - 2; index >= 0; --index) //output the result: significant digits are printed at the end of the array
        printf("%09d", x.int32[index]);
}

void scanf_value(uint1024_t* x)//заполнение полного массива
{
    char str[1024];
    scanf("%s", str);
    int len = strlen(str);
    int size = len % 9 == 0 ? len / 9 : len / 9 + 1;
    uint1024_t result;
    result.int32 = malloc(size * sizeof(int32_t));
    result.size = size;
    for (int index = len, j = 0; index > 0; index -= 9, j++)
    {
        str[index] = '\0'; //убираю нули
        result.int32[j] = (atoi(index >= 9 ? str + index - 9 : str)); //I take 9 digits from the end, if the index is less than 9, I go to the rest of the line
    }
    *x = result;
}


uint1024_t add_op(uint1024_t x, uint1024_t y)//сложение
{
    uint1024_t result;
    int size = Findmax(x.size, y.size);
    result.int32 = calloc(size, sizeof(int32_t));
    result.size = size;
    int32_t over = 0;
    for (int index = 0; index < size || over; index++)
    {
        if (index == size)
        {
            result.int32 = realloc(result.int32, (size + 1) * sizeof(int32_t));
            result.int32[size] = 0;
            result.size++;
        }
        result.int32[index] = over + (index < x.size ? x.int32[index] : 0) + (index < y.size ? y.int32[index] : 0);
        if (result.int32[index] >= cc) over = 1, result.int32[index] -= cc;
        else over = 0;
    }
    return result;
}

uint1024_t subtr_op(uint1024_t x, uint1024_t y)//вычитание
{
    uint1024_t result;
    int size = Findmax(x.size, y.size);
    result.int32 = calloc(size, sizeof(int32_t));
    result.size = size;
    int over = 0;
    for (int index = 0; index < size || over; index++)
    {
        if (index == size)
            break;
        result.int32[index] = x.int32[index] - (index < y.size ? y.int32[index] : 0) - over;
        if (result.int32[index] < 0) {
            over = 1;
            result.int32[index] += cc;
        }
        else over = 0;
    }
    while (result.int32[result.size - 1] == 0 && result.size > 1)
        result.size--;
    result.int32 = realloc(result.int32, result.size * sizeof(int32_t));
    return result;
}

int main()
{
    setlocale(LC_ALL, "Rus");
    uint1024_t num1, num2;
    char ArrayOfChar[310];
    memset(ArrayOfChar, 0, 310);
    printf("Введите число\n");
    scanf_value(&num1);
    printf("Введите число\n");
    scanf_value(&num2);
    fseek(stdin, 0, SEEK_END);
    while (1)
    {
        printf("Выберете: + - * \n");
        scanf("%c", ArrayOfChar);
       if (!strcmp(ArrayOfChar, "-")) num1 = subtr_op(num1, num2);
        else if (!strcmp(ArrayOfChar, "+")) num1 = add_op(num1, num2);
        else break;
        printf_value(num1);   
        fseek(stdin, 0, SEEK_END);
        printf("\nВведите число\n");
        scanf_value(&num2);
        fseek(stdin, 0, SEEK_END);
        return 0;
    }
    }

 
 
