#include <stdio.h>
#include <stdint.h>
#include <string.h>
#define MAX_UNSIGNED_INT 4294967296

typedef struct UINT1024 {
    unsigned int array[32];
} uint1024_t;

uint1024_t from_uint(unsigned int x) {
    uint1024_t a;
    for (int i = 0; i <= 31; i++) {
        a.array[i] = 0;
    }
    a.array[0] = x;
    return a;
}

uint1024_t add_op(uint1024_t x, uint1024_t y) {
    int carry = 0;
    uint64_t overflow = MAX_UNSIGNED_INT;
    uint1024_t z = from_uint(0);
    for (int i = 0; i <= 31; i++) {
        uint64_t value = ((uint64_t) x.array[i] + (uint64_t) y.array[i] + carry);
        z.array[i] = value % overflow;
        carry = value / overflow;
    }
    return z;
}

uint1024_t mult_op(uint1024_t x, uint1024_t y) {
    uint1024_t mlt = from_uint(0);
    uint64_t overflow = MAX_UNSIGNED_INT;
    for (int i = 0; i <= 31; i++) {
        uint64_t carry = 0;
        uint1024_t z = from_uint(0);
        for (int j = 0; j <= 31; j++) {
            uint64_t value = ((uint64_t) x.array[j] * (uint64_t) y.array[i] + carry);
            z.array[j] = value % overflow;
            carry = value / overflow;
        }
        if (carry > 0) {
            printf("overflow!\n");
            return mlt;
        }
        
        mlt = add_op(mlt, z);
    }
    return mlt;
}
    

uint1024_t subtr_op(uint1024_t x, uint1024_t y) {
    int carry = 0;
    uint64_t overflow = MAX_UNSIGNED_INT;
uint1024_t sum_mass;
for (int i = 0; i <= 31; i++) {
        sum_mass.array[i] = 0;
    }
   int count = sizeof(x.array)/sizeof(x.array[0]);
   for (size_t i=0; i<count || carry; ++i) {
       if (i == count){
           break;
       }
	x.array[i] -= carry + (i < count ? y.array[i] : 0);
	carry = x.array[i] < 0;
	if (carry != 0) {
     x.array[i] += overflow;
    }
}
    return x;

}

uint1024_t segmentation(uint1024_t x, long long y){

long long carry = 0;
    uint64_t overflow = MAX_UNSIGNED_INT;
uint1024_t sum_mass;
for (int i = 0; i <= 31; i++) {
        sum_mass.array[i] = 0;
    }
   int count = sizeof(x.array)/sizeof(x.array[0]);
   for (int i=(int)count-1; i>=0; --i) {
	long long cur = x.array[i] + carry  * overflow;
	x.array[i] = (int)(cur / y);
	carry = (int)(cur % y);
}
    return x;
}

void scanf_value(uint1024_t *x) {
    char line[310];
    uint1024_t y;
    y = from_uint(0);
    scanf("%s", line);
    for (int i = 0; i < strlen(line); i++) {
        y = mult_op(y, from_uint(10));
        y = add_op(y, from_uint(line[i] - '0'));
    }
    for (int i = 0; i <= 31; i++) {
        x->array[i] = y.array[i];
    }
}

void printf_value(uint1024_t x) {
    uint64_t overflow = MAX_UNSIGNED_INT;
    uint1024_t z = from_uint(0);
    uint1024_t zero = from_uint(0);
    char line[310];
    memset(line, '\0', 310);
    do {
        uint64_t carry = 0;
        for (int i = 31; i >= 0; i--) {
            uint64_t value = (x.array[i] + carry * overflow);
            z.array[i] = value / 10;
            carry = value % 10;
        }
        line[strlen(line)] = carry + '0';
        x = z;
    } while (memcmp(&x, &zero, sizeof(int) * 32) != 0);
    for (int i = strlen(line); i >= 0; i--) {
        printf("%c", line[i]);
    }
}

int main() {
    uint1024_t a = from_uint(0), b = from_uint(0);
    long long y = 0;
    printf("Multiplication test\n");
    scanf_value(&a);
    scanf_value(&b);
    printf_value(mult_op(a, b));
    printf("\n");

    printf("Segmentation test\n");
    scanf_value(&a);
    scanf("%llu",&y);
    printf_value(segmentation(a, y));
    printf("\n");

    printf("Addition test\n");
    scanf_value(&a);
    scanf_value(&b);
    printf_value(add_op(a, b));
    printf("\n");


    printf("Subtraction test\n");
    scanf_value(&a);
    scanf_value(&b);
    printf_value(subtr_op(a, b));
    printf("\n");
}



