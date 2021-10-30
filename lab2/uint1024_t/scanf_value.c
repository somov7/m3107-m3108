#include "uint1024_t.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void scanf_value(uint1024_t* x) {
  char string[1024];
  scanf("%s", string);
  int length = strlen(string);
  int size = length; // works for ???

  free(x->arr);

  x->size = size;
  x->arr = malloc(size * sizeof(int));

  for (int i = 0; i < size; ++i) {
    x->arr[i] = (int)string[size - i - 1] - 48;
  }
}
