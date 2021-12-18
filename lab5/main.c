#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include "bmp.h"
#include "gamelive.h"

char *extract_bits(char *bitmap, uint32_t size, uint16_t w, uint16_t h) {
    uint32_t bit_size = w*h;
    char *bits = malloc(bit_size);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < 8; j++) {
            bits[8*i + j] = (char)((bitmap[i] & (1 << j)) >> j);
        }
    }
    return bits;
}

char *zip_bits(char *bits, uint32_t size) {
    char *bitmap = malloc(size);
    for (int i = 0; i < size; ++i) {
        char zip = 0;
        for (int j = 0; j < 8; ++j) {
            zip |= bits[8*i + j] << j;
        }
        bitmap[i] = zip;
    }
    return bitmap;
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

// /Users/anton/Desktop/image4.bmp
// /Users/anton/Desktop/
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
    uint16_t w = get_BMP_width(bmp);
    uint16_t h = get_BMP_height(bmp);
    uint32_t size = get_data_size(bmp);
    char *bits = extract_bits(bmp->bitmap, size, w, h);
    char num[10];
    for (int i = 0; i < gener_cnt; i++) {
       if (i % frec) {
           free(bmp->bitmap);
           bmp->bitmap = zip_bits(bits, size);
           for (int i = 0; i < size; ++i) {
               bmp->bitmap[i] = ~bmp->bitmap[i];
           }
           itoa(i, num);
           strcat(out_dir, num);
           strcat(out_dir, ".bmp");
           creat_from_bmp(bmp, out_dir);
           out_dir[str_size] = 0;
       }
        next(w, h, bits);
    }
    free(out_dir);
    delete(bmp);
}