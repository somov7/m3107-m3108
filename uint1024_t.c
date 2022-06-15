#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

const int64_t int32_max = 4294967296;

struct uint1024_t{
    uint32_t int1024[32];
};

int64_t max(int64_t a, int64_t b);
char* uint1024_to_bin(struct uint1024_t x);
struct uint1024_t bin_to_uint1024(char* bin);
char* add_bin(char* x, char* y);
char* string_to_bin(char* string, int size);
struct uint1024_t from_uint(uint32_t x);
struct uint1024_t add_op(struct uint1024_t x, struct uint1024_t y);
struct uint1024_t subtr_op(struct uint1024_t x, struct uint1024_t y);
struct uint1024_t mult_op(struct uint1024_t x, struct uint1024_t y);
void printf_value(struct uint1024_t x);
void scanf_value(struct uint1024_t *x);


int64_t max(int64_t a, int64_t b){
    if(a > b) return a;
    return b;
}

char* uint1024_to_bin(struct uint1024_t x) {
    char *bin = (char*)malloc(sizeof(char) * 1024);
    for (int i = 0; i < 1024; i++){
        bin[i] = 0;
    }

    for (int i = 31; i >= 0; i--) {
        char *cur_bin = (char*)malloc(sizeof(char*) * 32);
        int k = 0;
        while (x.int1024[i] > 0) {
            cur_bin[k++] = x.int1024[i] % 2;
            x.int1024[i] /= 2;
        }

        for (int j = 0; j < k; j++) {
            bin[(31 - j) + 32 * i] = cur_bin[j];
        }
        free(cur_bin);
    }
    return bin;
}

struct uint1024_t bin_to_uint1024(char* bin) {
    struct uint1024_t ans = from_uint(0);

    uint64_t an = 0, k = 1, pos = 32;
    for (int i = 1023; i >= 0; i--) {
        if(i % 32 == 31) {

            if(pos < 32) {
                ans.int1024[pos] = an;
            }
            k = 1;
            an = 0;
            pos--;
        }
        an += k * bin[i];
        k *= 2;
    }
    ans.int1024[pos] = an;

    free(bin);
    return ans;
};

char* add_bin(char* x, char* y) {
    int k = 0;
    for (int i = 1023; i >= 0; i--) {
        k += x[i] + y[i];
        x[i] = k % 2;
        k /= 2;
    }

    return x;
}

char* string_to_bin(char* string, int size) {
    char* bin = (char*)malloc(sizeof(char) * 1024);
    for (int i = 0; i < 1024; i++) {
        bin[i] = 0;
    }
    int cur_pos = 1023, begin = 0;
    while (begin < size && cur_pos >= 0) {
        bin[cur_pos] = string[size - 1] % 2;
        cur_pos--;

        int x = 0;
        for (int i = begin; i < size; i++) {
            x += string[i];
            string[i] = x / 2;
            x = (x % 2) * 10;
        }

        while (begin < size && string[begin] == 0) {
            begin++;
        }
    }

    return bin;
}


struct uint1024_t from_uint(uint32_t x) {
    struct uint1024_t ans;
    for (int i = 0; i < 32; i++)ans.int1024[i] = 0;
    ans.int1024[31] = x;

    return ans;
}

struct uint1024_t add_op(struct uint1024_t x, struct uint1024_t y){ /// ans = x + y
    struct uint1024_t ans = from_uint(0);
    uint64_t surplus = 0;
    for(int i = 31; i >= 0; i--){
        uint64_t buffer = x.int1024[i];
        buffer += y.int1024[i] + surplus;
        ans.int1024[i] = buffer % int32_max;
        surplus = buffer / int32_max;
    }

    return ans;
}

struct uint1024_t subtr_op(struct uint1024_t x, struct uint1024_t y){ /// ans = x - y

    struct uint1024_t ans = from_uint(0);
    int lack = 0;
    for(int i = 31; i >= 0; i--){
        int64_t buffer = x.int1024[i];
        buffer -= y.int1024[i] + lack;
        if(buffer < 0){
            buffer += int32_max;
            lack = 1;
        }
        else lack = 0;

        ans.int1024[i] = buffer;
    }

    return ans;
}

struct uint1024_t mult_op(struct uint1024_t x, struct uint1024_t y){ /// ans = x * y
    char *binx = uint1024_to_bin(x);
    char *biny = uint1024_to_bin(y);

    char *bin_ans = (char*)malloc(sizeof(char) * 1024);
    for (int i = 0; i < 1024; i++){
        bin_ans[i] = 0;
    }

    for (int i = 1023; i >= 0; i--) {
        if (binx[i] == 0) {
            continue;
        }

        char *bin = (char*)malloc(sizeof(char) * 1024);
        for (int i = 0; i < 1024; i++){
            bin[i] = 0;
        }

        for (int j = 1023; j >= 1023 - i; j--) {
            bin[j - (1023 - i)] = biny[j];
        }
        bin_ans = add_bin(bin_ans, bin);
        free(bin);
    }
    free(binx);
    free(biny);

    return bin_to_uint1024(bin_ans);
}

void printf_value(struct uint1024_t x){
    char *bin = uint1024_to_bin(x);

    char *dec = (char*)malloc(sizeof(char) * 310);
    char *factor = (char*)malloc(sizeof(char) * 310);

    for (int i = 309; i >= 0; i--) {
        factor[i] = dec[i] = 0;
    }
    factor[309] = 1;

    for (int i = 1023; i >= 0; i--) {
        if (bin[i] == 1) {
            int buffer = 0;
            for (int i = 309; i >= 0; i--) {
                buffer += dec[i] + factor[i];
                dec[i] = buffer % 10;
                buffer /= 10;
            }
        }

        int buffer = 0;
        for (int i = 309; i >= 0; i--) {
            buffer += 2 * factor[i];
            factor[i] = buffer % 10;
            buffer /= 10;
        }
    }

    int fl = 0;
    for (int i = 0; i < 310; i++) {
        if (fl == 1) {
            printf("%d", dec[i]);
        }
        else if (dec[i] != 0) {
            fl = 1;
            printf("%d", dec[i]);
        }
    }
    if (fl == 0) {
        printf("0");
    }

    free(factor);
    free(bin);
    free(dec);
}

void scanf_value(struct uint1024_t *x){
    int size = 1, cur_pos = 0;
    char* num = (char*)malloc(sizeof(char));
    char c;
    while (1) {
        scanf("%c", &c);
        if (c < '0' || c > '9') {
            break;
        }
        if (cur_pos == size) {
            size *= 2;
            num = (char*)realloc(num, sizeof(char) * size);
        }

        num[cur_pos] = c - '0';
        cur_pos++;
    }
    num = (char*)realloc(num, sizeof(char) * cur_pos);
    char* bin = string_to_bin(num, cur_pos);
    *x = bin_to_uint1024(bin);
    free(num);
}

int main()
{
    struct uint1024_t a = from_uint(0), b;
    scanf_value(&a);
    scanf_value(&b);
    printf_value(add_op(a, b));
    printf("\n");
    printf_value(mult_op(a, b));
    printf("\n");
    printf_value(subtr_op(a, b));
    printf("\n");
    return 0;
}
