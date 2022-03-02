#include <stdio.h>
#include <string.h>

const long long SIZE = 35;
const long long BASE = 1e9;


typedef struct uint1024_t
{
    long long digits[35];

} uint1024_t;

uint1024_t from_uint(unsigned long x){
    uint1024_t result;
    for(int i = 0; i < SIZE; i++){
        result.digits[i] = 0;
    }
    int next = 0;
    while (x)
    {
        result.digits[next++] = x % BASE;
        x /= BASE;
    }
    return result;
}

uint1024_t add_op(uint1024_t x, uint1024_t y){
    for(int i = 0; i < SIZE; i++){
        x.digits[i] += y.digits[i];
    }
    for(int i = 0; i < SIZE - 1; i++){
        if(x.digits[i] >= BASE){
            x.digits[i] -= BASE;
            x.digits[i+1]++;
        }
    }
    return x;
}

uint1024_t subtr_op(uint1024_t x, uint1024_t y){
    for(int i = 0; i < SIZE; i++){
        x.digits[i] -= y.digits[i];
    }
    for(int i = 0; i < SIZE - 1; i++){
        if(x.digits[i] < 0){
            x.digits[i] += BASE;
            x.digits[i+1]--;
        }
    }
    return x;
}

uint1024_t mult_op(uint1024_t x, uint1024_t y){
    uint1024_t result = from_uint(0);
    
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE - i; j++){
            result.digits[i+j] += x.digits[i] * y.digits[j];
        }
    }

    for(int i = 0; i < SIZE - 1; i++){
        result.digits[i+1] += result.digits[i] / BASE;
        result.digits[i] %= BASE;
    }
    return result;
}

void printf_value(uint1024_t x){
    int i = SIZE - 1;
    while (x.digits[i] == 0 && i > 0)
    {
        i--;
    }

    for(int j = i; j >= 0; j--){
        printf("%d", x.digits[j]);
    }
    
}

void scanf_value(uint1024_t *x){
    char buffer[309];
    *x = from_uint(0);
    uint1024_t ten = from_uint(10);

    scanf("%s", buffer);
    for(int i = 0 ; i < 309; i++){
        if(buffer[i] == '\0'){
            break;
        }
        *x = mult_op(*x, ten);
        uint1024_t tmp = from_uint(buffer[i] - '0');
        *x = add_op(*x, tmp);
    }
}

int main()
{
    uint1024_t a;
    uint1024_t b;
    uint1024_t temp;
    printf("Введите первое число: ");
    scanf_value(&a);
    printf("\n");
    printf("Введите второе число: ");
    scanf_value(&b);
    printf("\n\nСумма: ");
    temp = add_op(a, b);
    printf_value(temp);
    printf("\n\nРазность: ");
    temp = subtr_op(a, b);
    printf_value(temp);
    printf("\n\nПроизведение: ");
    temp = mult_op(a, b);
    printf_value(temp);
    return 0;
}