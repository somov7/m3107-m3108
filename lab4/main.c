#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

#include "tasks/tasks.h"

#define MAX_STR_SIZE 1000

int main(int argc, char** argv) {
  if (argc < 3 || argc > 4) {
    printf("Wrong amount of arguments");
    exit(1);
  }

  char str[MAX_STR_SIZE];
  sscanf(argv[1], "%11s", str);
  if (strcmp(str, "--filepath=") != 0) {
    printf("No filepath specified");
    exit(1);
  }
  sscanf(argv[1], "--filepath=%s", str);

  FILE* fp;
  fp = fopen(str, "r+b");

  if (fp == NULL) {
    printf("Unable to open file");
    exit(1);
  }

  sscanf(argv[2], "%6s", str);
  if (strcmp(str, "--show") == 0) {
    show(fp);
    return 0;
  }

  sscanf(argv[2], "%5s", str);
  if (strcmp(str, "--get") == 0) {
    sscanf(argv[2], "--get=%s", str);
    int status = get(fp, str);
    if (status == 0) {
      printf("%s was not found", str);
    }
    return 0;
  }

  sscanf(argv[2], "%5s", str);
  if (strcmp(str, "--set") == 0) {
    char str1[MAX_STR_SIZE];
    sscanf(argv[2], "--set=%s", str);
    sscanf(argv[3], "--value=%s", str1);

    set(fp, str, str1);
    return 0;
  }

  return 0;
}
