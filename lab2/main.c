#include <stdio.h>
#include "uint1024_t/uint1024_t.h"

int main() {
  char c;
  printf("Type two numbers then choose an operator\n");

  uint1024_t a, b, result;
  scanf_value(&a);
  scanf_value(&b);

  c = getchar();
  c = getchar();

  switch (c) {
    case '+':
      result = add_op(a, b);
      break;
    case '-':
      result = subtr_op(a, b);
      break;
    case '*':
      result = mult_op(a, b);
      break;
  }

  printf_value(result);

  return 0;
}
