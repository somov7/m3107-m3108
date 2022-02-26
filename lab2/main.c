#include "uint1024.h"

int main()
{
    char op[1];
    uint1024_t a, b, res;

    while (1) {
        printf("Введите первое число: ");
        scanf_value(&a);

        printf("Введите второе число: ");
        scanf_value(&b);

        printf("Введите операцию (+, -, *): ");
        scanf("%s", &op);

        if (strcmp(op, "+") == 0) {
            res = add_op(a, b);
        } else if (strcmp(op, "-") == 0) {
            res = subtr_op(a, b);
        } else if (strcmp(op, "*") == 0) {
            res = mult_op(a, b);
        } else {
            return 0;
        }

        printf("Результат: ");
        printf_value(res);
        printf("\n\n");
    }

    return 0;
}