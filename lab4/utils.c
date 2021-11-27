#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wchar.h>
#include "tag.h"


int bytes_to_int( // конвертим байты в инт побитовым сдвигом
        uint8_t chr[4],
        bool is_new_version_mode) {
    // В версии 2.4 используется семь бит
    if (is_new_version_mode) {
        return (int) chr[0] << 21 |
               (int) chr[1] << 14 |
               (int) chr[2] << 7 |
               (int) chr[3] << 0;
    }
    return (int) chr[0] << 24 |
           (int) chr[1] << 16 |
           (int) chr[2] << 8 |
           (int) chr[3] << 0;
}

void int_to_bytes( // конвертим инт в байты побитовым сдвигом
        int x,
        uint8_t *ch,
        bool is_new_version_mode,
        TagHeader header
) {
    int bits = 127;
    // В версии 2.4 используется семь бит
    if (header.version[0] == 4 || is_new_version_mode) {
        ch[3] = x & bits;
        ch[2] = (x >> 7) & bits;
        ch[1] = (x >> 14) & bits;
        ch[0] = (x >> 21) & bits;
    } else {
        ch[3] = x & bits;
        ch[2] = (x >> 8) & bits;
        ch[1] = (x >> 16) & bits;
        ch[0] = (x >> 24) & bits;
    }
}

void print_frame(TagFrame frame, char *temp) {
    if (frame.unicode)
        wprintf(L"%ls: %ls\n", frame.frame_name, temp);
    else
        printf("%s: %s\n", frame.frame_name, temp);
}

char* read_data(int frame_size, FILE *file) {
    char *temp = calloc(frame_size, sizeof(char));
    fread(temp, sizeof(char), frame_size - 1, file);

    return temp;
}
