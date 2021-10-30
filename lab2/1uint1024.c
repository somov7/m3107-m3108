#include <stdio.h>
#include <math.h>
#include <string.h>

const int base = 1000000000;
const int max_length = 9;
const int arr_size = 35;
const int str_size = 310;

typedef struct {
    int arr[arr_size];
    int ranks;
} uint1024_t;

void to_zero(uint1024_t *x){
    for (int i = 0; i < arr_size; i++){
        x->arr[i] = 0;
    }
}

uint1024_t from_uint(unsigned int x)
{
    uint1024_t newnumber;

    to_zero(&newnumber);

    if (x < base)
    {
        newnumber.ranks = 1;
        newnumber.arr[0] = x;
    }
    else
    {
        newnumber.ranks = 2;
        newnumber.arr[0] = x % base;
        newnumber.arr[1] = x / base;
    }

    return newnumber;
}

uint1024_t printf_value(uint1024_t x){
    if (x.ranks == 0){
        printf("0");
    }
    else{
        printf("%d", x.arr[x.ranks-1]);
    }
    for (int i = x.ranks - 2; i>=0; --i){
	    printf ("%09d", x.arr[i]);
    }
}

uint1024_t add_op(uint1024_t x, uint1024_t y)
{
    uint1024_t result;
    long long carry = 0;
    int hrank = 1;

    for (int i = 0; i < arr_size; i++){
        carry = (long long)(x.arr[i] + y.arr[i]) + carry;
        result.arr[i] = carry % base;
        carry = carry / base;
    }

    for (int i = arr_size - 1; i >= 0; i--){
        if (result.arr[i] != 0){
            hrank = i + 1;
            break;
        }
    }

    result.ranks = hrank;

    return result;
}

uint1024_t subtr_op(uint1024_t x, uint1024_t y)
{
    uint1024_t result;
    long long carry = 0;
    int hrank = 1;

    for (int i = 0; i < arr_size; i++){
        carry = (long long)(x.arr[i] - y.arr[i]) - carry;
        if (carry >= 0)
        {
            result.arr[i] = carry;
            carry = 0;
        }
        else
        {
            result.arr[i] = base + carry;
            carry = 1;
        }
    }
    for (int i = arr_size - 1; i >= 0; i--){
        if (result.arr[i] != 0)
        {
            hrank = i + 1;
            break;
        }
    }
    result.ranks = hrank;

    return result;
}

uint1024_t mult_op(uint1024_t x, uint1024_t y)
{
    uint1024_t result;
    long long carry = 0;
    int hrank = 1;

    to_zero(&result);

    for (int i = 0; i < arr_size; i++){
        for (int j = 0; i + j < arr_size; j++)
        {
            carry = (long long)(x.arr[i]) * (long long)y.arr[j] + carry + result.arr[i + j];
            result.arr[i + j] = carry % base;
            carry = carry / base;
        }
    }
    for (int i = arr_size - 1; i >= 0; i--){
        if (result.arr[i] != 0)
        {
            hrank = i + 1;
            break;
        }
    }
    result.ranks = hrank;

    return result;
}

void scanf_value(uint1024_t *x)
{
    char temp[str_size];

    int digits = 0;
    to_zero(x);
    scanf("%309s", temp);

    int i = strlen(temp) - 1;

    while (i >= 0){
        x->arr[digits / max_length] += (temp[i] - '0') * pow (10, digits % max_length);
        i--;
        digits++;
    }
    x->ranks = (digits - 1) / max_length + 1;
}

int main()
{
  uint1024_t x,y;
  
  //1-й способ
  unsigned int a,b;
  scanf("%u %u", &a, &b);
  x = from_uint(a);
  y = from_uint(b);

  //2-й способ
  // scanf_value(&x);
  // scanf_value(&y);
  
  printf("Addition: ");
  printf_value(add_op(x,y));
  printf("\n");
  printf("Subtraction: ");
  printf_value(subtr_op(x,y));
  printf("\n");
  printf("Multiplication: ");
  printf_value(mult_op(x,y));

}

