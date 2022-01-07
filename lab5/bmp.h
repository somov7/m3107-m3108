//
// Created by Антон Чемодуров on 30.11.2021.
//

#ifndef LAB5_BMP_H
#define LAB5_BMP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BMP_TYPE 0x4d42
#define BMP_HEADER_S 14
#define BMP_CORE_HEADER_S 12
#define BMP_CORE_COLORTab_S 6


#pragma pack(push, 1)

/**
 * заголовок bmp файла всегда занимат
 * 14 байт и распологается в начале файла
 */
typedef struct {
    uint16_t bfType; // сигнатура файла всегла равна 0x4d42="BM" (2 байта)
    uint32_t bfSize; // размер файла в байтах (4 байта)
    uint16_t bfReserved1; // хз что это (2 байта)
    uint16_t bfReserved2; // (2 байта)
    uint32_t bfOffBits; // смешение пиксильных данных от начала файла в байтах (2 байта)
}bmp_file_header;


typedef struct {
    uint32_t bcSize; // размер заголовка, он же и указатель версии bmp файла всегда 12 байт для CORE типа
    uint16_t bcWidth; // ширина изображения
    uint16_t bcHeight; // высота
    uint16_t bcPlanes; // хз что это
    uint16_t bcBitCount; // глубина цвета в нашем случае всегда равна 1 так как файл монохромный, только два цвета
}bmp_core_header;


typedef struct {
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
} bmp_info_header;

#pragma pack(pop)


typedef struct {
    bmp_file_header *header; // заголовок
    bmp_core_header *info_core_header;
    bmp_info_header *info_header;
    char *bitmap;
}BMP;


int creat_from_bmp(const BMP *bmp, const char *file_name);
int creat_from_data(uint16_t width, uint16_t height, char *bitmap, size_t bitmap_size, const char *file_name);
BMP *open(const char *file_name);
int32_t get_BMP_width(BMP *bmp);
int32_t get_BMP_height(BMP *bmp);
uint32_t get_data_size(BMP *bmp);
int32_t get_actual_width(BMP *bmp);
void delete(BMP *bmp);
#endif //LAB5_BMP_H
