#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include "bmp.h"
#include "gamelive.h"

char *extract_bits(char *bitmap, uint32_t w, uint32_t h, uint32_t actual_w) {
    uint32_t bit_size = (w + 2)*(h + 2);
    char *bits = calloc(bit_size, 1);
    char str[actual_w * 8];
    for (int i = 1; i < h+1; ++i) {
        for (int j = 0; j < actual_w; j++) {
            for (int k = 0; k < 8; ++k) {
                str[j * 8 + k] = (char)((bitmap[(i-1)*actual_w + j] & (1 << (7 - k))) >> (7 - k));
            }
        }
        for (int j = 1; j < w+1; j++) {
            bits[i * (w+2)  + j] = str[j - 1];
        }
    }

    return bits;
}

char long_to_char(uint64_t bits) {
    char ans = 0;
    for (int i = 7; i <= 0; i--) {
        ans |= (bits & (1 << i)) >> i;
    }
    return ans;
}

void zip_bits(char *bits, char *bitmap, uint32_t w, uint32_t h, uint32_t actual_w) {
    char *tmp = calloc(actual_w, 1);
    printf("%d\n", actual_w);
    for (int i = 1; i < h; ++i) {
        int p = -1;
        for (int j = 1; j < w; j++) {
            int sh = (j - 1) % 8;
            if (sh == 0){
                p++;
                tmp[p] = 0;
            }
            tmp[p] += bits[i*h + j] << (7 - sh);
        }
        //printf("%d\n", p);
        for (int j = 0; j < actual_w; j++) {
            bitmap[(i-1) * actual_w + j] = tmp[j];
        }
    }
}

void reverse(char s[])
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void itoa(int n, char s[])
{
    int i, sign;

    if ((sign = n) < 0)  /* записываем знак */
        n = -n;          /* делаем n положительным числом */
    i = 0;
    do {       /* генерируем цифры в обратном порядке */
        s[i++] = n % 10 + '0';   /* берем следующую цифру */
    } while ((n /= 10) > 0);     /* удаляем */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

void print_bits(const char* bits, int h, int w) {
    int k = 1;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; j++) {
            printf("%d", bits[i*(w) + j]);
        }
        printf(" %d\n", k++);
    }
    printf("\n\n");
}

int main(int argc, char **argv) {
    BMP *bmp = NULL;
    char *out_dir = NULL;
    int gener_cnt = 0;
    int frec = 1;
    int c;
    size_t str_size;
    if (argc < 2) {
        exit(EXIT_FAILURE);
    }
    while ((c = getopt(argc, argv, "i:o:m:f:")) != -1) {
        if (c == 'i') {
            bmp = open(optarg);
        } else if (c == 'o') {
            str_size = strlen(optarg);
            out_dir = malloc(str_size);
            strcpy(out_dir, optarg);
            strcat(out_dir, "img");
            str_size = str_size + 3;
        } else if (c == 'm') {
            gener_cnt = atoi(optarg);
        } else if (c == 'f') {
            frec = atoi(optarg);
        }
        else {
            perror("bad options");
            printf("%u\n", c);
            exit(EXIT_FAILURE);
        }
    }
    if (bmp == NULL) {
        perror("not bmp");
        exit(EXIT_FAILURE);
    }

    int32_t w = get_BMP_width(bmp);
    int32_t h = get_BMP_height(bmp);
    uint32_t size = get_data_size(bmp);
    int realW = get_actual_width(bmp);
    printf("%d size: %d\n", realW, size);
    char *bits = extract_bits(bmp->bitmap, w, h, realW/8);
    char num[10];
    for (int i = 1; i <= gener_cnt; i++) {
        if (i % frec == 0) {
            //print_bits(bits, h+2, w+2);
            zip_bits(bits, bmp->bitmap,  w+2, h+2, realW/8);
            itoa(i, num);
            strcat(out_dir, num);
            strcat(out_dir, ".bmp");
            creat_from_bmp(bmp, out_dir);
            out_dir[str_size] = 0;
        }
        //printf("acadcad\n");
        next(w+2, h+2, bits);
    }

    free(out_dir);
    delete(bmp);
}