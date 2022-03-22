#include "uint1024_t.h"
#include <stdlib.h>

#define max(a,b) (((a) > (b)) ? (a) : (b))

#define BASE 10

uint1024_t subtr_op(uint1024_t x, uint1024_t y) {
  uint1024_t result;

  int carry = 0;
  int i = 0;

  int resultSize = max(x.size, y.size);

  result.size = resultSize;
  result.arr = calloc(result.size, sizeof(int));

  while (i < resultSize || carry) {
    if (i < y.size) {
      result.arr[i] = x.arr[i] - carry - y.arr[i];
    } else {
      result.arr[i] = x.arr[i] - carry;
    }

    carry = result.arr[i] < 0;
    if (carry) {
      result.arr[i] += BASE;
    }

    ++i;
  }

  while (result.arr[result.size - 1] == 0 && resultSize > 1) {
    result.arr = realloc(result.arr, (result.size - 1) * sizeof(int));
    result.size -= 1;
  }

  return result;
}