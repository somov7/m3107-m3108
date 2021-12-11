#ifndef LAB5_GAME_H
#define LAB5_GAME_H

#endif //LAB5_GAME_H

#define LIVE_CELL  1
#define NOT_LIVE_CELL 0

int** gameLife(int ***pixelArr, int height, int width);
int neighbourCheck(int** pixelArr, int row, int col, int height, int width);
int getNeighborCount(int **table, int row, int col, int height, int width);