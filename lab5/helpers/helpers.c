//
// Created by Rustam Tishkov on 14.02.2022.
//

#include <stdlib.h>
#include <string.h>
#include "helpers.h"
#include "stdbool.h"

short **createMatrix(BMPHeader header) {
    short **data = calloc(sizeof(short *), header.height);
    for (int i = 0; i < header.height; i++) {
        data[i] = calloc(sizeof(short), header.width);
    }
    return data;
}

void freeMatrix(short **matrix, BMPHeader header) {
    for (int i = 0; i < header.height; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void fillMatrix(short **matrix, short **tempMatrix, BMPHeader header) {
    for (int x = 0; x < header.height; x++) {
        for (int y = 0; y < header.width; y++) {
            matrix[x][y] = tempMatrix[x][y];
        }
    }
}

void updateTempMatrix(short **matrix, short **tempMatrix, int x, int y, int cells, bool *isStateChanged,
                      bool *hasLiveCells) {
    if (matrix[x][y] == POINT) {
        if (cells < 2 || cells > 3) {
            tempMatrix[x][y] = NO_POINT;
            *isStateChanged = true;
        } else {
            tempMatrix[x][y] = POINT;
            *hasLiveCells = true;
        }
    } else {
        if (cells == 3) {
            tempMatrix[x][y] = POINT;
            *hasLiveCells = true;
            *isStateChanged = true;
        } else {
            tempMatrix[x][y] = NO_POINT;
        }
    }
}

void checkCoords(BMPHeader header, int *tempX, int *tempY) {
    if (*tempX == header.height)
        *tempX = 0;
    else if (*tempX == -1)
        *tempX = header.height - 1;

    if (*tempY == header.width)
        *tempY = 0;
    else if (*tempY == -1)
        *tempY = header.width - 1;
}

FILE *createFile(Params params, int generation) {
    char *name = calloc(1, strlen(params.output) + 20);
    sprintf(name, "%s/generation_%d.bmp", params.output, generation);
    FILE *fp = fopen(name, "wb"); // open file for write
    free(name);

    if (fp == NULL) {
        fprintf(stderr, "Cannot create file\n");
        exit(EXIT_FAILURE);
    }

    return fp;
}

void checkBfType(FILE *file) {
    unsigned short bfType;
    if (fread(&bfType, 2, 1, file) == 0)
        exit(EXIT_FAILURE);

    if (bfType != BF_TYPE) { // проверяем bfType == "BM"
        fprintf(stderr, "Файл не в формате BMP");
        exit(EXIT_FAILURE);
    }
}

void checkBitsForPixel(FILE *file) {
    short pixelBits;
    if (fread(&pixelBits, sizeof(short), 1, file) == 0)
        exit(EXIT_FAILURE);
    if (pixelBits != 1) {
        fprintf(stderr, "Файл не в формате bmp");
        exit(EXIT_FAILURE);
    }
}

void getHeaderData(FILE *file, BMPHeader *header) {
    if (fread(&header->offset, 4, 1, file) == 0)
        exit(EXIT_FAILURE);
    skipFileData(file, SEEK_CUR, 4);
    if (fread(&header->width, 4, 1, file) == 0)
        exit(EXIT_FAILURE);
    if (fread(&header->height, 4, 1, file) == 0)
        exit(EXIT_FAILURE);
}

void skipFileData(FILE *file, int position, int size) {
    if (fseek(file, size, position) != 0) // скипаем длину заголовка
        exit(EXIT_FAILURE);
}

void getFileHeader(BMPHeader *header, FILE *file) {
    header->fileHeader = calloc(1, header->offset); // считываем файлхедер для сохранения в файлах поколений
    skipFileData(file, SEEK_SET, 0);
    if (fread(header->fileHeader, 1, header->offset, file) == 0)
        exit(EXIT_FAILURE);
}

void writeFileHeader(FILE *file, BMPHeader *header) {
    int writtenToFile = fwrite(header->fileHeader, sizeof(char), header->offset, file);
    if (writtenToFile != header->offset) { // проверяем заканчивается ли хеддер на оффсете
        exit(EXIT_FAILURE);
    }
}
