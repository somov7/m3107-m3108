//
// Created by Rustam Tishkov on 14.02.2022.
//

#ifndef LAB5_HELPERS_H
#define LAB5_HELPERS_H

#include <stdbool.h>
#include "../game/game.h"

short **createMatrix(BMPHeader header);

void freeMatrix(short **data, BMPHeader header);

void fillMatrix(short **matrix, short **tempMatrix, BMPHeader header);

void
updateTempMatrix(short **matrix, short **tempMatrix, int x, int y, int cells, bool *isStateChanged, bool *hasLiveCells);

FILE *createFile(Params params, int generation);

void checkCoords(BMPHeader header, int *tempX, int *tempY);

void checkBfType(FILE *file);

void checkBitsForPixel(FILE *file);

void getHeaderData(FILE *file, BMPHeader *header);

void skipFileData(FILE *file, int position, int size);

void getFileHeader(BMPHeader *header, FILE *file);

unsigned char getCharacter(unsigned char character, short **data, int x, int y, int bit);

void writeFileHeader(FILE *file, BMPHeader *header);

#endif //LAB5_HELPERS_H
