//
// Created by Rustam Tishkov on 10.01.2022.
//

#ifndef LAB5_GAME_H
#define LAB5_GAME_H

#include <stdio.h>
#include <stdlib.h>
#include "../argparse/argparse.h"

#define POINT 1
#define NO_POINT 0
#define BF_TYPE 0x4d42
#define WHITE 0xFF

typedef struct BMPHeader {
    uint32_t offset; // where image pixels starts, position in bmp: 10
    uint32_t width; // position in bmp: 18
    uint32_t height; // position in bmp: 22
    char *fileHeader; // header till data begin
} BMPHeader;

short** readData(FILE *file);
void saveGeneration(short **data, int generation, Params params);

int getNeighbourCells(short **matrix, BMPHeader header, int x, int y);
int processGeneration(short **matrix);
void start(FILE *file, Params params);

#endif //LAB5_GAME_H
