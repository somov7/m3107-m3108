#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "argparse.h"

#define INF 1000000

Args argparse(int argc, char** argv) {
  if (argc < 5 || argc > 9 || argc % 2 == 0) {
    fprintf(stderr, "Wrong amount of arguments");
    exit(EXIT_FAILURE);
  }

  Args returnValue;
  returnValue.maxIter = INF;
  returnValue.dumpFreq = 1;

  for (int i = 1; i < argc; i += 2) {
    char* arg;
    asprintf(&arg, "%s", argv[i]);

    if (strcmp(arg, "--input") == 0) {
      asprintf(&returnValue.input, "%s", argv[i + 1]);
    } else if (strcmp(arg, "--output") == 0) {
      asprintf(&returnValue.output, "%s", argv[i + 1]);
    } else if (strcmp(arg, "--max_iter") == 0) {
      char* argval;
      asprintf(&argval, "%s", argv[i + 1]);
      returnValue.maxIter = (int)strtol(argval, NULL, 10);
      free(argval);
    } else if (strcmp(arg, "--dump_freq") == 0) {
      char* argval;
      asprintf(&argval, "%s", argv[i + 1]);
      returnValue.dumpFreq = (int)strtol(argval, NULL, 10);
      free(argval);
    } else {
      fprintf(stderr, "Wrong argument");
      exit(EXIT_FAILURE);
    }
  }
  return returnValue;
}