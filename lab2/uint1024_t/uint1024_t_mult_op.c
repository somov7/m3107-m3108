#include "uint1024_t.h"
#include <stdlib.h>

#define BASE 10

uint1024_t mult_op(uint1024_t x, uint1024_t y) {
  uint1024_t result;

  int carry = 0;
  int i = 0;
  
  int resultSize = x.size + y.size;

  result.arr = calloc(resultSize, sizeof(int));
  result.size = resultSize;

  while (i < x.size) {
    int j = 0;
    while (j < y.size || carry) {
      int tmp = 0;
      if (j < y.size) {
        tmp = result.arr[i + j] + x.arr[i] * y.arr[j] + carry;
      } else {
        tmp = result.arr[i + j] + carry;
      }
      result.arr[i + j] = tmp % BASE;
      carry = tmp / BASE;
      ++j;
    }
    ++i;
  }

  while (result.arr[result.size - 1] == 0 && resultSize > 1) {
    result.arr = realloc(result.arr, (result.size - 1) * sizeof(int));
    result.size -= 1;
  }

  return result;
}
