#ifndef GAMEOFLIFE_BMP_H
#define GAMEOFLIFE_BMP_H
#endif //GAMEOFLIFE_BMP_H
// :D
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#pragma pack(push, 1)
typedef struct BITMAPFILEHEADER {
    uint16_t Type; // сигнатура "BM"
    uint32_t fileSize; // размер файла
    uint16_t Reserved1;
    uint16_t Reserved2;
    uint32_t OffsetBits; // Смещение изображения от начала файла
} BitMapFileHeader;

typedef struct BITMAPINFOHEADER {
    uint32_t headerSize; // длина заголовка
    uint32_t width; // ширина
    uint32_t height; // высота
    uint16_t planes; // число плоскостей
    uint16_t BitCount; // глубина цвета (бит на точку)
    uint32_t Compression; // тип сжатия
    uint32_t imageSize; // размер изображения
    uint32_t XpelsPerMeter; // горизонтальное разрешение (точки на метр)
    uint32_t YpelsPerMeter; // вертикальное разрешение (точки на метр)
    uint32_t ColorsUsed; // число используемых цветов
    uint32_t ColorsImportant; // число основных цветов

} BitMapInfoHeader;
#pragma pack(pop)

int **bmp_to_arr(char *filepath, int *height, int *width, FILE **pImage, struct BITMAPFILEHEADER *bitmapFileHeader,
                 struct BITMAPINFOHEADER *bitmapInfoHeader);

void
arr_to_bmp(int **arr, int height, int width, int iter_num, char *dir_name, struct BITMAPFILEHEADER *bitmapFileHeader,
            FILE **pImage);