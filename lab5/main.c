#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"
#include "argparse/argparse.h"
#include "bmpparse/bmpparse.h"
#include "matrix/matrix.h"
#include "bmpgenerate/bmpgenerate.h"
#include "gameoflife/gameoflife.h"

int main(int argc, char** argv) {
  Args args = argparse(argc, argv);

  FILE* fp = fopen(args.input, "rb");
  if (fp == NULL) {
    fprintf(stderr, "Wrong file path");
    exit(EXIT_FAILURE);
  }

  Bmp bmp = bmpparse(fp);
  Matrix matrix = generateMatrix(bmp, fp);

  matrix = matrix;

  int counter = args.dumpFreq;
  for (int i = 1; i <= args.maxIter; ++i) {
    gameoflife(matrix);
//    printMatrix(bmp.matrix);

    if (counter == args.dumpFreq) {
      char* output;
      char* num;

      asprintf(&output, "%s", args.output);
      asprintf(&num, "%d", i);

      strcat(output, "/");
      strcat(output, num);
      strcat(output, ".bmp");

      bmpgenerate(bmp, matrix, output);

      counter = 1;
      continue;
    }

    ++counter;
  }

  return 0;
}
