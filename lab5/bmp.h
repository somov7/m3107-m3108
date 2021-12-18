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

/**
 * задаем выравниевание структур
 * про выравние почитай подробнее в гугле
 * https://habr.com/ru/post/142662/
 * делаем мы это чтобы можно было сразу
 * считывать из файла в структуру минуя промежуточный буффер
 */
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

/**
 * информационный залоговок bmp версии CORE
 * разпологается после основного заголовка и занием 12 байт
 */
typedef struct {
    uint32_t bcSize; // размер заголовка, он же и указатель версии bmp файла всегда 12 байт для CORE типа
    uint16_t bcWidth; // ширина изображения
    uint16_t bcHeight; // высота
    uint16_t bcPlanes; // хз что это
    uint16_t bcBitCount; // глубина цвета в нашем случае всегда равна 1 так как файл монохромный, только два цвета
}bmp_core_header;

/**
 * информационный залоговок bmp версии 3, 4, 5
 * размер и версию заголовка указывает первые 4 байта
 * для нашей задачи нужно знать только высоту, ширину, глубину цвета
 * так как 5 включает 4 включает 3 версии (см Вики)
 *  мы можем для 3, 4, 5 создать одну структуру и обрабатывать их одинаково
 */
typedef struct {
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
} bmp_info_header;

#pragma pack(pop)


/**
 * структура хранящая все распасеный bmp файл
 */
typedef struct {
    bmp_file_header *header; // заголовок
    /**
     * какой-то из информационных заголовков
     * один из них всегда будет NULL
     */
    bmp_core_header *info_core_header;
    bmp_info_header *info_header;
    char *bitmap; // сжатые пиксильные данные
}BMP;
/**
 * 1 0 1 1 0 1 0 1
 */
int creat_from_bmp(const BMP *bmp, const char *file_name);
int creat_from_data(uint16_t width, uint16_t height, char *bitmap, size_t bitmap_size, const char *file_name);
BMP *open(const char *file_name);
uint32_t get_BMP_width(BMP *bmp);
uint32_t get_BMP_height(BMP *bmp);
void delete(BMP *bmp);
uint32_t get_data_size(BMP *bmp);
#endif //LAB5_BMP_H
