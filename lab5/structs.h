#ifndef PROGA_LABA5_STRUCTS_H
#define PROGA_LABA5_STRUCTS_H

typedef struct Matrix {
  int** arr;
  int width;
  int height;
  int black;
  int white;
} Matrix;

#pragma pack(push, 1)

typedef struct BmpFileHeader {
    unsigned char signature[2];
    unsigned char fileSize[4];
    unsigned char reserved[4];
    unsigned char dataOffset[4];
} BmpFileHeader;

typedef struct BmpInfoHeader {
    unsigned char size[4];
    unsigned char width[4];
    unsigned char height[4];
    unsigned char planes[2];
    unsigned char bitsPerPixel[2];
    unsigned char compression[4];
    unsigned char imageSize[4];
    unsigned char xPixelsPerM[4];
    unsigned char yPixelsPerM[4];
    unsigned char colorsUsed[4];
    unsigned char colorsImportant[4];
} BmpInfoHeader;

typedef struct BmpColorTable {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char reserved;
} BmpColorTable;

typedef struct BmpGap {
    unsigned char* arr;
    int size;
} BmpGap;

typedef struct Bmp {
  BmpFileHeader fileHeader;
  BmpInfoHeader infoHeader;
  BmpColorTable colorTable;
  BmpGap gap;
  unsigned char* remainer;
} Bmp;

#pragma pack(pop)

#endif
