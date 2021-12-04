//
// Created by Антон Чемодуров on 03.12.2021.
//

#include "LZW.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char *start;
    size_t size;
}Key;


//int cmp(Key x, Key y) {
//    for (int i = 0; i < x.size; i++) {
//        if (y.size == i) {
//            return -1;
//        }
//        if (x.key[i] < y.size[j]) {
//            return -1;
//        } else if (x.key[i] > y.size[j]) {
//            return 1
//        } else {
//            return 0;
//        }
//    }
//    return 1;
//}

uint16_t find(Key key) {

}

uint16_t get_block_size(char size) {
    if (size == 1) return 3;
    return size + 1;
}

size_t init_dict(Key *dict, char start_table_size) {
    char size = start_table_size;
    if (size == 2) size = 4;
    for (int i = 0; i < size + 2; i++) {
        char *code = malloc(sizeof(char));
        *code = i;
        Key key = {
          code,
          1
        };
        dict[i] = key;
    }
    return size + 2;
}

void add_bits(char *compressed, size_t *size, char bits, uint16_t bloc_size) {

}


char *LZW_compress_for_GIF(char *data, size_t data_size, char start_table_size) {
    uint16_t block_size = get_block_size(start_table_size);
    Key *dict = malloc(MAX_DICT_SIZE * sizeof(Key));
    size_t dict_size = init_dict(dict, start_table_size);
    size_t compressed_size = 1024;
    char *compressed = malloc(compressed_size);
    add_bits(compressed, &compressed_size, *dict[dict_size-1].start, block_size);
    size_t i = 0;
    while (i < data_size) {


    }
}