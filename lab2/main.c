#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define BASE 1000000000

struct uint_1024_t {
    unsigned int digits[35];
};

void uint_1024_generate(struct uint_1024_t *digit, unsigned int regular) {
    digit->digits[34] = regular % BASE;
    digit->digits[33] = regular / BASE;
};

void uint_1024_sum(struct uint_1024_t *digit1, struct uint_1024_t *digit2) {
    for (int i = 34; i > 0; i--) {
        digit1->digits[i - 1] += (digit1->digits[i] + digit2->digits[i]) / BASE;
        digit1->digits[i] = (digit1->digits[i] + digit2->digits[i]) % BASE;
    }
    digit1->digits[0] = (digit1->digits[0] + digit2->digits[0]) % BASE;
}

void uint_1024_subtraction(struct uint_1024_t *digit1, struct uint_1024_t *digit2) {
    for (int i = 34; i >= 0; i--) {
        if (digit1->digits[i] < digit2->digits[i]) {
            int temp = i - 1;
            while (temp >= 0 && (digit1->digits[temp] == 0)) {
                printf("%d ", digit1->digits[temp]);
                temp--;
            }
            if (temp >= 0) {
                digit1->digits[temp]--;
                temp++;
                for (temp; temp < i; temp++) {
                    digit1->digits[temp] += BASE - 1;
                }
                digit1->digits[temp] += BASE;
            } else {
                break;
            }
        }
        digit1->digits[i] = (digit1->digits[i] - digit2->digits[i]);
    }
}

void uint_1024_multiplication_short(struct uint_1024_t *digit1, unsigned int regular) {
    struct uint_1024_t result_digit = {0};
    unsigned int a = digit1->digits[34];
    unsigned long long temp = 0;
    for (int digit_len = 34; digit_len > 0; digit_len--) {
        struct uint_1024_t temp_digit = {0};
        temp = (unsigned long long) digit1->digits[digit_len] * regular;
        temp_digit.digits[digit_len] = temp % BASE;
        temp_digit.digits[digit_len - 1] = temp / BASE;
        uint_1024_sum(&result_digit, &temp_digit);
    }
    temp = (unsigned long long) digit1->digits[0] * regular;
    digit1->digits[0] = temp % BASE;
    for (int i = 0; i < 35; i++)
    {
        digit1->digits[i] = result_digit.digits[i];
    }
}
void uint_1024_multiplication (struct uint_1024_t *digit1, struct  uint_1024_t *digit2)
{
    struct uint_1024_t result = {0};
    for (int i = 34; i >=0; i--)
    {
        struct uint_1024_t temp = *digit1;
        struct uint_1024_t shift = {0};
        uint_1024_multiplication_short(&temp, digit2->digits[i]);
        for (int j = i; j >= 0; j--)
        {
            shift.digits[i - j] = temp.digits[34 - j];
        }
        uint_1024_sum(&result, &shift);
    }
    for (int i = 0; i < 35; i++)
    {
        digit1->digits[i] = result.digits[i];
    }
}
void uint1024_printf(struct uint_1024_t *digit)
{
    int i = 0;
    while (digit->digits[i] == 0)
    {
        i++;
    }
    printf("%u", digit->digits[i++]);
    printf(".");
    while (i != 35)
    {
        if (digit->digits[i] != 0)
        {
            int log = 8 - (int)log10(digit->digits[i]);
            for (log; log > 0; log--)
            {
                printf("0");
            }
            printf("%u", digit->digits[i]);
        }
        else
        {
            printf("000000000");
        }
        printf(".");
        i++;
    }
}
void uint1024_scanf(struct uint_1024_t *digit)
{
    char* string = calloc(309, sizeof(char));
    scanf("%s", string);
    int str_len = strlen(string);
    char* str_end = string + str_len - 1;

    int temp;
    int index = 34;
    while(str_end >= string)
    {
        temp = 0;
        for (int i = 0; i < 9; i++)
        {
            if (str_end < string)
            {
                break;
            }
            temp = temp + (*str_end - '0') * pow(10, i);
            str_end--;
        }
        digit->digits[index--] = temp;
    }
}
int main() {
    struct uint_1024_t digit1 = {0};
    struct uint_1024_t digit2 = {0};
    uint1024_scanf(&digit1);
    uint1024_printf(&digit1);
    printf("\n");
    uint_1024_generate(&digit2, 999293);
    uint_1024_multiplication(&digit1, &digit2);
    uint1024_printf(&digit1);
    printf("\n");
    uint_1024_subtraction(&digit1, &digit2);
    uint1024_printf(&digit1);
    printf("\n");
    uint_1024_sum(&digit1, &digit2);
    uint1024_printf(&digit1);
    printf("\n");
    uint_1024_multiplication(&digit1, &digit2);
    uint_1024_multiplication(&digit1, &digit2);
    uint_1024_multiplication(&digit1, &digit2);
    uint_1024_multiplication(&digit1, &digit2);
    uint1024_printf(&digit1);
    printf("\n");
    uint_1024_multiplication(&digit1, &digit1);
    uint_1024_multiplication(&digit1, &digit1);
    uint_1024_multiplication(&digit1, &digit1);
    uint_1024_multiplication(&digit1, &digit1);
    uint1024_printf(&digit1);
    printf("\n");
    return 0;
}