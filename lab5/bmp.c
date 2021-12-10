#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"


int **bmpToPixels(FILE* f, bitMapFile bmp) {
    int width = bmp.biWidth, height = bmp.biHeight, lineLength;
    char* line;
    int** pixels;
    int byteWidth = (width / 8) + ((width % 8 > 0) ? 1 : 0); //should be integer number of bytes

    /* Creating gaming pole */
    pixels = (int **)calloc(width, sizeof(int *));
    for (int i = 0; i < width; i++)
        pixels[i] = (int *)calloc(height, sizeof(int));

    fseek(f, bmp.bfOffs, SEEK_SET);
    
    if (byteWidth % 4 != 0)
        lineLength = byteWidth + (4 - (byteWidth % 4));
    else
        lineLength = byteWidth;

    line = (char *)calloc(lineLength, sizeof(char));

    for (int j = height - 1; j >= 0; j--) {
        fread(line, 1, lineLength, f);

        for (int i = 0; i < width; i++) {
            int currentByte = i / 8, currentBit = 1 << (7 - i % 8);

            if (line[currentByte] & currentBit)
                pixels[i][j] = 0;
            else
                pixels[i][j] = 1;
        }
    }
    free(line);
    return pixels;
}

int arrToBmp(int **pixels, bitMapFile bmp, int generation, char *dirname, char *offset) {
    char* filePath = (char *)calloc(MAX_STRING_SIZE, sizeof(char));
    char* gen = (char*)calloc(30, sizeof(char));
    FILE* f;
    int width = bmp.biWidth, height = bmp.biHeight, lineLength;

    _itoa(generation, gen, 30);
    strcat(filePath, dirname);
    strcat(filePath, "/");
    strcat(filePath, gen);
    strcat(filePath, ".bmp");

    f = fopen(filePath, "wb");

    if (f == NULL)
        return 0;

    fwrite(offset, 1, bmp.bfOffs, f);

    int byteWidth = (width / 8) + ((width % 8 > 0) ? 1 : 0); 

    if (byteWidth % 4 != 0)
        lineLength = byteWidth + (4 - (byteWidth % 4));
    else
        lineLength = byteWidth;

    for (int j = height - 1; j >= 0; j--) {
        char* line = (char*)calloc(lineLength, sizeof(char));

        for (int i = 0; i < width; i++) {
            int currentByte = i / 8;

            if (pixels[i][j] == 0) 
                line[currentByte] = (char)(1 << (7 - i % 8)) | line[currentByte];
        }

        fwrite(line, 1, lineLength, f);
        free(line);
    }
    fclose(f);
    free(filePath);
    free(gen);
    return 1;
}