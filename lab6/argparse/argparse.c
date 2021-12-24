#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "argparse.h"

#define DEFAULT_ARGUMENTS_COUNT 4

Args argparse(int argc, char** argv) {
  if (argc < 4) {
    fprintf(stderr, "Not enough arguments");
    exit(EXIT_FAILURE);
  }

  Args returnValue;
  returnValue.sourceFiles = NULL;
  returnValue.create = 0;
  returnValue.extract = 0;
  returnValue.list = 0;

  for (int i = 1; i < argc; ++i) {
    char* str;
    asprintf(&str, "%s", argv[i]);

    if (strcmp(str, "--file") == 0) {
      ++i;
      char* param;
      asprintf(&param, "%s", argv[i]);

      if  (param[0] == '-') {
        fprintf(stderr, "Incorrect filename");
        exit(EXIT_FAILURE);
      }

      returnValue.filename = param;
    } else if (strcmp(str, "--create") == 0) {
      returnValue.create = 1;
      returnValue.sourceFiles = malloc(argc - DEFAULT_ARGUMENTS_COUNT);
      returnValue.sourceFilesCount = argc - DEFAULT_ARGUMENTS_COUNT;

      int counter = 0;
      for (int j = i + 1; j < argc; ++j) {
        char* param;
        asprintf(&param, "%s", argv[j]);
        returnValue.sourceFiles[counter] = param;
        ++counter;
        ++i;
      }
    } else if (strcmp(str, "--extract") == 0) {
      returnValue.extract = 1;
    } else if (strcmp(str, "--list") == 0) {
      returnValue.list = 1;
    } else {
      fprintf(stderr, "Incorrect argument");
      exit(EXIT_FAILURE);
    }

    free(str);
  }

  return returnValue;
}

