#include <stdlib.h>
#include "gameoflife.h"
#include "../matrix/matrix.h"

#define DEAD 1
#define LIVE 0

void gameoflife(Matrix matrix) {
  int** newarr = malloc(matrix.height * sizeof(int*));
  for (int i = 0; i < matrix.height; ++i) {
    newarr[i] = malloc(matrix.width * sizeof(int));
  }

  for (int i = 0; i < matrix.height; ++i) {

    for (int j = 0; j < matrix.width; ++j) {
      int cell = matrix.arr[i][j];
      int neighbours = countAliveCellNeighbours(matrix, i, j);

      if (cell == LIVE) {
        if (neighbours == 2 || neighbours == 3) {
          continue;
        } else {
          newarr[i][j] = DEAD;
        }
      } else {
        if (neighbours == 3) {
          newarr[i][j] = LIVE;
        } else {
          newarr[i][j] = DEAD;
        }
      }
    }
  }

  for (int i = 0; i < matrix.height; ++i) {
    for (int j = 0; j < matrix.width; ++j) {
      matrix.arr[i][j] = newarr[i][j];
    }
  }

  for (int i = 0; i < matrix.height; ++i) {
    free(newarr[i]);
  }
  free(newarr);
}

int countAliveCellNeighbours(Matrix matrix, int y, int x) {
  int counter = 0;

  for (int i = y - 1; i <= y + 1; ++i) {
    for (int j = x - 1; j <= x + 1; ++j) {
      if ((i == y && j == x) || (i < 0) || (j < 0) || (i >= matrix.height) || (j >= matrix.width)) {
        continue;
      }
      if (matrix.arr[i][j] == LIVE) {
        ++counter;
      }
    }
  }

  return counter;
}