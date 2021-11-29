#ifndef LAB5_BMP_H
#define LAB5_BMP_H
#endif //LAB5_BMP_H
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <iso646.h>

#define MAX_FILENAME_LEN 10;

#pragma pack(push, 1)
typedef struct stBitMapFile {

    uint16_t bfType;            //Метка BM
    uint32_t bfSize;            //Длина в байтах
    uint16_t bfRes1;            //Резерв1
    uint16_t bfRes2;           //Резерв2
    uint32_t bfOffs;            //Смещение области данных

    //Начало BITMAP_INFO:
    uint32_t biSize;            //Длина BITMAP_INFO
    int32_t biWidth;                //Ширина картинки
    int32_t biHeight;              //высота картинки
    uint16_t biPlanes;            //Число плоскостей
    uint16_t biBitCnt;            //Бит на пиксел
    uint32_t biCompr;            //Тип сжатия
    uint32_t biSizeIm;            //Размер изображения в байтах
    int32_t biXPels;                //Разрешение по горизонтали
    int32_t biYPels;                //Разрешение по вертикали
    uint32_t biClrUsed;            //Количество используемых цветов
    uint32_t biClrImp;            //Количество основных цветов
} bitMapFile;
#pragma pack(pop)

void printUsage(char* err);
void openCorrect(FILE* file);
int** bmpToPixelsArray(int height, int width, FILE* fin, bitMapFile bmp);
int** mallocPixelArray(int height, int width);
void printArr(int** arr, int height, int width);
void freePixelArray(int height, int** arr);
void pixelArrayToBmp(int** pixelArr, int height, int width, char* offset, int generationNumber, int offsetLen);