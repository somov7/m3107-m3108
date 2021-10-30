#include <stdio.h>
#include "uint1024.h"

int main() {
    uint1024_t number1, number2;
    number1 = from_uint(2406078406);
    number2 = from_uint(1111111111);

    uint1024_t res1 = subtr_op(number1, number2);
    printf_value(res1); // 1294967295
    printf("\n");
    printf_value(add_op(res1, number2)); // 2406078406

    uint1024_t number3, number4, number5;
    scanf_value(&number3);
    scanf_value(&number4);
    scanf_value(&number5);

    uint1024_t res2 = add_op(number3, number4);
    printf_value(res2);
    printf_value(subtr_op(number5, res2));

    return 0;
}


