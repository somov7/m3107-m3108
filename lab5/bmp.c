#include <stdio.h>
#include <stdlib.h>
#include <iso646.h>
#include <string.h>
#include "bmp.h"
#include "game.h"
#include "argParse.h"
#define MAX_DIRECTORY_LEN 1000

void freePixelArray(int height, int** arr){
    for (int i = 0; i < height; i++){
        free(arr[i]);
    }
    free(arr);
}

int** mallocPixelArray(int height, int width){
    int** resArr = (int**) calloc(height, sizeof (int));
    for (int i = 0; i < height; i++){
        resArr[i] = (int*)calloc(width, sizeof(int));
    }
    return resArr;
}

int** bmpToPixelsArray(int height, int width, FILE* fin, bitMapFile bmp){
    int **resArr = mallocPixelArray(height, width);
    fseek(fin, (int)bmp.bfOffs, SEEK_SET);  // перемещаем указатель на начало считываения пикселей

    int realWidth = (width / 8) + ((width % 8 > 0) ? 1 : 0);    // действительная ширина где находятся значащие биты
    int scanLineSize = realWidth;
    if (realWidth % 4 != 0){
        scanLineSize = realWidth + (4 - (realWidth % 4));
    }
    char* scanLine = calloc(scanLineSize, sizeof(char));

    for (int y = height - 1; y >= 0; y--){  // записываем снизу вверх, т.к. пиксели считываются с левого нижнего угла
        fread(scanLine, 1, scanLineSize, fin);  // cчитываем строку с учётом паддинга
        for (int x = 0; x < width; x++){
            int curBytePos = x / 8;
            int curBit = 1 << (7 - x % 8);
            int pixel = scanLine[curBytePos] & curBit;
            if (pixel == 0){        // если бит равен 1(белый цвет), то будем помечать его как 1, если равен нулю - нулём
                resArr[y][x] = LIVE_CELL;
            }
            else{
                resArr[y][x] = NOT_LIVE_CELL;
            }
        }
    }
    free(scanLine);
    return resArr;
}

void pixelArrayToBmp(int** pixelArr, int height, int width, char* offset, int generationNumber, int offsetLen, char* directoryName){

    char *strGenNumber = calloc(10, sizeof(char));
    itoa(generationNumber, strGenNumber, 10);
    char *fullFilePath = calloc(MAX_DIRECTORY_LEN, sizeof(char));
    strcat(fullFilePath, directoryName);
    strcat(fullFilePath, "/");
    strcat(fullFilePath, strGenNumber);
    strcat(fullFilePath, ".bmp");


    FILE *fout = fopen(fullFilePath, "wb");
    openCorrect(fout);

    fwrite(offset, 1, offsetLen, fout); // записываем все байты до самой битмапы

    int realWidth = (width / 8) + ((width % 8 > 0) ? 1 : 0);    // действительная ширина где находятся значащие биты
    int scanLineSize = realWidth;
    if (realWidth % 4 != 0){
        scanLineSize = realWidth + (4 - (realWidth % 4));
    }

    for (int y = height - 1; y >= 0; y--){
        char* scanLine = calloc(scanLineSize, sizeof(char));
        for (int x = 0; x < width; x++){
                int curBytePosition = x / 8;
                if (pixelArr[y][x] == 0){
                    scanLine[curBytePosition] = (char)(1 << (7 - x % 8)) bitor scanLine[curBytePosition];
                }
        }
        fwrite(scanLine, 1, scanLineSize, fout);
        free(scanLine);
    }

    fclose(fout);
    free(fullFilePath);
    free(strGenNumber);
}
