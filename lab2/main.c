#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#define max(a,b) (((a) > (b)) ? (a) : (b))
typedef struct {
    int parts[35];
    int size;
} uint1024_t;

void to_zero(uint1024_t *num) {
    for (int i = 34; i >= 0; i--){
        num->parts[i] = 0;
    }
}

uint1024_t String_uint1024_t(char *s) {
    uint1024_t result;
    to_zero(&result);
    result.size = strlen(s);
    int x = 0;
    int j = 1;
    int i = strlen(s);
    int base = 1;
    int k = 34;

    while ((i-- > 0) && (k >= 0)) {
        x += base * (s[i] - 48);
        base *= 10;
        if (j++ >= 9 || i <= 0) {
            result.parts[k--] = x;
            j = 1;
            base = 1;
            x = 0;
        }
    }
    return result;
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
    to_zero(&result);
    result.size = max(num1.size,num2.size);
    result.size = result.size % 9 == 0 ? result.size / 9 : result.size / 9 + 1;
    for (i = 34; i >= 0; i--){
        result.parts[i] = add(num1.parts[i], num2.parts[i], &carry);
    }
    int k;
    k = 0;
    int sz = 35;
    while (result.parts[k] == 0 && k < 35)
    {
        sz--;
        k++;
    }
    result.size = sz;
    return result;
}

uint1024_t subtr_op(uint1024_t num1, uint1024_t num2){
    int i;
    uint8_t carry = 0;
    uint1024_t result;
    to_zero(&result);
    result.size = max(num1.size,num2.size);
    result.size = result.size % 9 == 0 ? result.size / 9 : result.size / 9 + 1;
    for (i = 34; i >= 0; i--){
        result.parts[i] = sub(num1.parts[i], num2.parts[i], &carry);
    }
    int k = 35 - result.size,sz = result.size;
    while (result.parts[k] == 0 && k < 35)
    {
        sz--;
        k++;
    }
    result.size = sz;
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
    to_zero(&result);
    result.size = max(num1.size,num2.size);
    result.size = result.size % 9 == 0 ? result.size / 9 : result.size / 9 + 1;
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
    int f;
    f = 0;
    int sz = 35;
    while (result.parts[f] == 0 && f < 35)
    {
        sz--;
        f++;
    }
    result.size = sz;
    return result;
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


void print_result (uint1024_t res) {
    int f = 0;
    int n = res.size;
    for (int i = 35 - n; i < 35; i++) {
        //i += sprintf(&s[i], "%09d", res.parts[i]);
        
        if(res.parts[i] == 0 && 35 - n != i){
            printf("%s","000000000");
        }
        else if(res.parts[i] / 10 == 0 && 35 - n != i){
            printf("%s","00000000");
        }
        else if(res.parts[i] / 100 == 0 && 35 - n != i){
            printf("%s","0000000");
        }
        else if(res.parts[i] / 1000 == 0 && 35 - n != i){
            printf("%s","000000");
        }
        else if(res.parts[i] / 10000 == 0 && 35 - n != i){
            printf("%s","00000");
        }
        else if(res.parts[i] / 100000 == 0 && 35 - n != i){
            printf("%s","0000");
        }
        else if(res.parts[i] / 1000000 == 0 && 35 - n != i){
            printf("%s","000");
        }
        else if(res.parts[i] / 10000000 == 0 && 35 - n != i){
            printf("%s","00");
        }
        else if(res.parts[i] / 100000000 == 0 && 35 - n != i){
            printf("%s","0");
        }
        printf("%d",res.parts[i]);

        
    }
    printf("\n");
}
int main() {
    uint1024_t num1;
    uint1024_t num2;
    uint1024_t res;
    to_zero(&num1);
    to_zero(&num2);
    to_zero(&res);
    char s[315];
    printf("%s","Enter first number:");
    scanf("%s", s);
    num1 = String_uint1024_t(s);
    printf("%s","Enter first number:");
    scanf("%s", s);
    num2 = String_uint1024_t(s);
    res = subtr_op(num1, num2);
    printf("%s","The result of subtraction:");
    print_result(res);
    res = add_op(num1, num2);
    printf("%s","The result of addition:");
    print_result(res);
    res = mult_op(num1,num2);
    printf("%s","The result of multiplication:");
    print_result(res);
    return 0;
} 