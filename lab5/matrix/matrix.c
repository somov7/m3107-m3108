#include <string.h>
#include <stdlib.h>

#include "../structs.h"
#include "matrix.h"

Matrix generateMatrix(Bmp bmp, FILE* fp) {
  Matrix matrix;
  int imageSize;
  memcpy(&matrix.width, bmp.infoHeader.width, 4);
  memcpy(&matrix.height, bmp.infoHeader.height, 4);
  memcpy(&imageSize, bmp.infoHeader.imageSize, 4);

  int dataOffset;
  memcpy(&dataOffset, bmp.fileHeader.dataOffset, 4);
  fseek(fp, dataOffset, SEEK_SET);

  // because bit count in task is 8 the
  int rowSize = ((1 * matrix.width + 31) / 32) * 4; // in bytes

  matrix.arr = calloc(matrix.height, sizeof(int*));
  for (int i = 0; i < matrix.height; ++i) {
    matrix.arr[i] = calloc(matrix.width, sizeof(int));
  }

  unsigned char c;
  int counter = 0;

  if (bmp.colorTable.red == 0xFF && bmp.colorTable.green == 0xFF && bmp.colorTable.blue == 0xFF) {
    matrix.black = 1;
    matrix.white = 0;
  } else {
    matrix.black = 0;
    matrix.white = 1;
  }

  for (int i = matrix.height - 1; i > -1; --i) {
    counter = 0;

    fread(&c, sizeof(unsigned char), 1, fp);

    int bytesCount;
    bytesCount = 1;

    for (int j = 0; j < matrix.width; ++j) {
      matrix.arr[i][j] = (c >> (7 - counter)) & 1;

      if (counter == 7) {
        fread(&c, sizeof(unsigned char), 1, fp);
        bytesCount += 1;
        counter = 0;
        continue;
      }

      ++counter;
    }
    for (int j = bytesCount; j < rowSize; ++j) {
      fread(&c, sizeof(unsigned char), 1, fp);
    }
  }

  return matrix;
}

void printMatrix (Matrix matrix) {
  for (int i = 0; i < matrix.height; ++i) {
    for (int j = 0; j < matrix.width; ++j) {
      printf("%d ", matrix.arr[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}
