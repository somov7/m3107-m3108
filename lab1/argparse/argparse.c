#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "argparse.h"

#define ARG_REQUIRED_AMOUNT 3

int optAmount = 3;
struct Option opts[] =
{
  {"--lines", "-l", LINES},
  {"--bytes", "-c", BYTES},
  {"--words", "-w", WORDS}
};

struct Args argparse (int argc, char** argv) {
  struct Args returnValue;

  //check if enough arguments
  if (argc != ARG_REQUIRED_AMOUNT) {
    printf("Amount of arguments is wrong\n");
    exit(1);
  }

  //parsing option
  //check if first argument is not an option
  if (argv[1][0] != '-') {
    printf("First argument should be an option\n");
    exit(1);
  }

  //parse an option
  if (argv[1][0] == '-') {
    if (argv[1][1] == '-') {
      //control if option is found
      int flag = 0;

      //parse a long option
      for (int i = 0; i < optAmount; ++i) {
        if (strcmp(argv[1], opts[i].longName) == 0) {
          returnValue.argType = opts[i].argType;
          flag = 1;
          break;
        }
      }

      //control if option is found
      if (flag == 0) {
        printf("Given option is not specified\n");
        exit(1);
      }
    } else {
      //control if option is found
      int flag = 0;

      //parse a short option
      for (int i = 0; i < optAmount; ++i) {
        if (strcmp(argv[1], opts[i].shortName) == 0) {
          returnValue.argType = opts[i].argType;
          flag = 1;
          break;
        }
      }

      //control if option is found
      if (flag == 0) {
        printf("Given option is not specified\n");
        exit(1);
      }
    }
  }

  returnValue.filePath = argv[2];
  return returnValue;
}