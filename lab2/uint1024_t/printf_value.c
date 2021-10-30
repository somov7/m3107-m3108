#include "uint1024_t.h"
#include <stdio.h>

void printf_value(uint1024_t x) {
  for (int i = 0; i < x.size; ++i) {
    printf("%d", x.arr[x.size - 1 - i]);
  }
  printf("\n");
}
