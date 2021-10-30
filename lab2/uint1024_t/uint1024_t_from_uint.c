#include "uint1024_t.h"
#include <stdlib.h>

#define INTSIZE 1024
#define BASE 10

struct uint1024_t from_uint(unsigned int x) {
  uint1024_t returnValue;
  int* tmp = malloc(INTSIZE * sizeof(int));

  int i = 0;
  while (x != 0) {
    tmp[i] = x % BASE;
    x /= BASE;
    ++i;
  }

  returnValue.size = i;
  returnValue.arr = (int*)malloc(i * sizeof(int));
  for (int j = 0; j < returnValue.size; ++j) {
    returnValue.arr[j] = tmp[j];
  }

  free(tmp);

  return returnValue;
}
