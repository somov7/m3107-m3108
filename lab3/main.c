#include <stdlib.h>
#include <stdio.h>
#include "tasks/tasks.h"

#include <time.h>

int main(int argc, char** argv) {
  clock_t start = clock();

  if (argc < 3) {
    printf("Not enough arguments\n");
    exit(1);
  }

  char* filePath = argv[1];
  int window = atoi(argv[2]);

  FILE* file = fopen(filePath, "r");
  if (file == NULL) {
    printf("File not found\n");
    exit(1);
  }

  task1(file);

  task2(file, window);

  fclose(file);

  clock_t end = clock();
  double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
  printf("Time: %f\n", time_spent);

  return 0;
}
