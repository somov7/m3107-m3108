#include <iso646.h>
#include "bmp.h"
#include "game.h"


int **gameLife(int ***pixelArr, int height, int width) {
    int **tempArr = mallocPixelArray(height, width);
    int neighbourCnt;

    for (int y = 0; y < height; y++) {                  // проверка всех правил игры
        for (int x = 0; x < width; x++) {
            neighbourCnt = getNeighborCount(*pixelArr, y, x, height, width);
            if ((neighbourCnt == 3) or
                (neighbourCnt == 2 and (*pixelArr)[y][x] == LIVE_CELL)) {
                tempArr[y][x] = LIVE_CELL;
            } else {
                tempArr[y][x] = NOT_LIVE_CELL;
            }
        }
    }

    freePixelArray(height, pixelArr);

    return tempArr;
}

int neighbourCheck(int **pixelArr, int row, int col, int height, int width) {
    if (row < 0 || row >= height || col < 0 || col >= width) {
        return 0;
    } else if (pixelArr[row][col] != LIVE_CELL) {
        return 0;
    } else {
        return 1;
    }
}

int getNeighborCount(int **table, int row, int col, int height, int width) {
    int neighborCounter = 0;
    neighborCounter += neighbourCheck(table, row - 1, col - 1, height, width);
    neighborCounter += neighbourCheck(table, row - 1, col, height, width);
    neighborCounter += neighbourCheck(table, row - 1, col + 1, height, width);
    neighborCounter += neighbourCheck(table, row, col - 1, height, width);
    neighborCounter += neighbourCheck(table, row, col + 1, height, width);
    neighborCounter += neighbourCheck(table, row + 1, col - 1, height, width);
    neighborCounter += neighbourCheck(table, row + 1, col, height, width);
    neighborCounter += neighbourCheck(table, row + 1, col + 1, height, width);
    return neighborCounter;
}

