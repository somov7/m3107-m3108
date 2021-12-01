#ifndef GAMEOFLIFE_GAME_H
#define GAMEOFLIFE_GAME_H
#endif //GAMEOFLIFE_GAME_H
// :)
#include <stdlib.h>
#include <stdio.h>
#define LIVE_CELL 0
#define DEAD_inside_CELL 1
int **gameOfLife(int **arr, int height, int width);
int **mallocForArr(int height, int width);
void printArr(int **arr, int h, int w);