#include "bmp.h"
#include "game.h"

int **bmp_to_arr(char *filepath, int *height, int *width, FILE **pImage, struct BITMAPFILEHEADER *bitmapFileHeader,
                 struct BITMAPINFOHEADER *bitmapInfoHeader) {
    // открываем файл
    *pImage = fopen(filepath, "rb");
    if (*pImage == NULL) {
        printf("file == null");
        exit(1);
    }
    // читаем fileHeader
    fread(&(*bitmapFileHeader), sizeof(BitMapFileHeader), 1, *pImage);
    // читаем infoHeader
    fread(&bitmapInfoHeader, sizeof(BitMapInfoHeader), 1, *pImage);
    // присваиваем высоту и ширину
    *height = (int) bitmapInfoHeader->height;
    *width = (int) bitmapInfoHeader->width;
    // выдеряем память под массив
    int **arr = mallocForArr(*height, *width);
    // перемещаем указатель в начало битов изображения
    fseek(*pImage, (int) bitmapFileHeader->OffsetBits, SEEK_SET);
    int x = 0; // доп переменная для подсчета в строке 25
    // колдуем :o
    if (*width % 8 > 0) x = 1;
    int realWidth = (*width / 8) + x;
    int scanLineSize = realWidth;
    if (realWidth % 4 != 0) {
        scanLineSize = realWidth + (4 - (realWidth % 4));
    }
    char *scanLine = calloc(scanLineSize, sizeof(char));

    for (int y = *height - 1; y >= 0; y--) {
        fread(scanLine, 1, scanLineSize, *pImage);
        for (int x = 0; x < *width; x++) {
            int curBytePos = x / 8;
            int curBit = 1 << (7 - x % 8);
            int pixel = scanLine[curBytePos] & curBit;
            if (pixel == 0) {
                arr[y][x] = LIVE_CELL;
            }
            else {
                arr[y][x] = DEAD_inside_CELL;
            }
        }
    }
    // память лишней не бывает о3о
    free(scanLine);

    return arr;
}

void
arr_to_bmp(int **arr, int height, int width, int iter_num, char *dir_name, struct BITMAPFILEHEADER *bitmapFileHeader,
           FILE **pImage) {

    char *buf = (char *) malloc(bitmapFileHeader->OffsetBits);
    fseek(*pImage, 0, SEEK_SET);
    fread(buf, 1, bitmapFileHeader->OffsetBits, *pImage);

    char *genInStr = calloc(10, sizeof(char));
    itoa(iter_num, genInStr, 10);
    char *newFilePath = calloc(1000, sizeof(char));
    strcat(newFilePath, dir_name);
    strcat(newFilePath, "/generation");
    strcat(newFilePath, genInStr);
    strcat(newFilePath, ".bmp");

    FILE *pNewImage = fopen(newFilePath, "wb");
    if (pNewImage == NULL) {
        printf("file == NULL");
        exit(2);
    }

    fwrite(buf, 1, bitmapFileHeader->OffsetBits, pNewImage);
    free(genInStr);
    free(buf);

    int x = 0;
    if (width % 8 > 0) x = 1;
    int realWidth = (width / 8) + x;
    int scanLineSize = realWidth;
    if (realWidth % 4 != 0) {
        scanLineSize = realWidth + (4 - (realWidth % 4));
    }
    char *scanLine = calloc(scanLineSize, sizeof(char));

    for (int y = height - 1; y >= 0; y--) {
        fread(&scanLine, 1, scanLineSize, *pImage);
        for (int x = 0; x < width; x++) {
            int curBytePos = x / 8;
            int curBit = 1 << (7 - x % 8);
            int pixel = scanLine[curBytePos] & curBit;
            if (pixel == 0) {
                arr[y][x] = LIVE_CELL;
            }
            else {
                arr[y][x] = DEAD_inside_CELL;
            }
        }
        fwrite(scanLine, 1, scanLineSize, pNewImage);
        free(scanLine);
    }
    fclose(pNewImage);
    free(newFilePath);
}
