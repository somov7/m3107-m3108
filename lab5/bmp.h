#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <iso646.h>
#include <string.h>
#include "game.h"

#define MAX_STRING_SIZE 512

#pragma pack(push, 1)
typedef struct stBitMapFile {
    /* Header */
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfRes1;
    uint16_t bfRes2;
    uint32_t bfOffs;

    /* InfoHeader */
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCnt;
    uint32_t biCompr;
    uint32_t biSizeIm;
    int32_t biXPels;
    int32_t biYPels;
    uint32_t biClrUsed;
    uint32_t biClrImp;
} bitMapFile;
#pragma pack(pop)

int **bmpToPixels(FILE* f, bitMapFile bmp);
int arrToBmp(int** pixels, bitMapFile bmp, int generation, char* dirname, char* offset);
int** newGeneration(int** pixels, bitMapFile bmp);
void fieldInit(byte* F, int** pixels, bitMapFile bmp);