#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int parts[35];
} uint1024_t;

void setZero(uint1024_t *num) {
    for (int i = (34); i >= 0; i--){
        num->parts[i] = 0;
    }
}

int add(int a, int b, uint8_t *carry) {
    int res;
    res = a + b + *carry;
    if (res > 999999999) {
        *carry = 1;
        res -= 1000000000;
    }
    else {
        *carry = 0;
    }
    return res;
}

int sub(int a, int b, uint8_t *carry) {
    int res;
    res = a - b - *carry;
    if (res < 0) {
        *carry = 1;
        res += 1000000000;
    }
    else {
        *carry = 0;
    }
    return res;
}

uint1024_t add_op(uint1024_t num1, uint1024_t num2){
    int i;
    uint8_t carry = 0;
    uint1024_t result;
    setZero(&result);
    for (i = 34; i >= 0; i--){
        result.parts[i] = add(num1.parts[i], num2.parts[i], &carry);
    }
    return result;
}

uint1024_t subtr_op(uint1024_t num1, uint1024_t num2){
    int i;
    uint8_t carry = 0;
    uint1024_t result;
    setZero(&result);
    for (i = 34; i >= 0; i--){
        result.parts[i] = sub(num1.parts[i], num2.parts[i], &carry);
    }
    return result;
}

uint1024_t mult_op(uint1024_t num1, uint1024_t num2){
    int i;
    int j;
    int k;
    long long r;
    int r1, r2;
    uint8_t carry = 0;
    uint1024_t result;
    setZero(&result);

    for (i = 34; i >= 0; i--){
        for (j = 34; j >= 0; j--) {
            k = (i + j - 34);
            if (k < 0)
                continue;
            r = (long long) num1.parts[i] * (long long) num2.parts[j];
            r1 = r % 1000000000;
            r2 = r / 1000000000;
            carry = 0;
            if (r1 > 0){
                result.parts[k] = add(result.parts[k], r1, &carry);
            }
            if (k >= 1 && r2 > 0){
                result.parts[k - 1] = add(result.parts[k - 1], r2, &carry);
            }
        }
    }
    return result;
}

void delLeadZeros(char *str, char *dest)
{
    int i;
    for (i = 0; i < strlen(str) - 1; i++) {
        if (str[i] != '0')
            break;
    }
    strcpy(dest, str + i);
}

uint1024_t strToStruct(char *s) {
    uint1024_t res;
    setZero(&res);
    int i;
    int x = 0;
    int j = 1;
    int index = strlen(s);
    int base = 1;
    int k = 34;

    while ((index-- > 0) && (k >= 0)) {
        x += base * (s[index] - 48);
        base *= 10;
        if (j++ >= 9 || index <= 0) {
            res.parts[k--] = x;
            j = 1;
            base = 1;
            x = 0;
        }
    }
    return res;
}

void ToString (uint1024_t res, char *s) {
    int index = 0;
    for (int i = 0; i < 35; i++) {
        index += sprintf(&s[index], "%09d", res.parts[i]);
    }
    delLeadZeros(s, s);
}

int main() {
    uint1024_t first_num;
    uint1024_t second_num;
    uint1024_t res;
    setZero(&first_num);
    setZero(&second_num);
    setZero(&res);
    char s[35*9];

    for (int i = 0; i < 35; i++){
        first_num.parts[i] = 0;
        second_num.parts[i] = 0;
        res.parts[i] = 0;
    }

    scanf("%s", s);
    first_num = strToStruct(s);

    scanf("%s", s);
    second_num = strToStruct(s);

    res = subtr_op(first_num, second_num);
    ToString(res, s);
    printf("%s\n",s);

    res = add_op(first_num, second_num);
    ToString(res, s);
    printf("%s\n",s);

    res = mult_op(first_num, second_num);
    ToString(res, s);
    printf("%s\n",s);
    return 0;
}
