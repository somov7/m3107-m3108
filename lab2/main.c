/* Orlov Aleksandr, 12-5, M3107,  11.10.2021 */

#include <stdio.h>
#include "uint1024.h"

int main(void)
{
    uint1024_t test1, test2, test = from_uint(1234567890);;

    scanf_value(&test1);
    scanf_value(&test2);
    
    printf("\n\n");
    printf_value(subtr_op(test1, test2));
    printf("\n\n");
    printf_value(add_op(test1, test2));
    printf("\n\n");
    printf_value(mult_op(test1, test2));
    return 0;
}