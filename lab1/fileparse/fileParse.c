#include <stdio.h>
#include <stdlib.h>

#include "../structs.h"
#include "fileparse.h"

int fileparse (char* filePath, enum argTypes argType) {
  //opening file
  FILE* file = fopen(filePath, "r");
  if (file == NULL) {
    printf("File is not found\n");
    exit(1);
  }

  //common counter for all argument types
  int count = 0;

  //parsing file
  char c;
  int inWord = 0;
    while (EOF != (c = fgetc(file))) {

    //count lines
    if (argType == LINES && c == '\n') {
      ++count;
      continue;
    }

    //count bytes
    if (argType == BYTES) {
      count += sizeof(char);
      continue;
    }

    //count words
    if (argType == WORDS) {
      if (c == ' ' || c == '\t' || c == '\n') {
        if (inWord == 1) {
          inWord = 0;
          ++count;
        }
      } else {
        inWord = 1;
      }
      continue;
    }
  }

  //lines have + 1 count
  if (argType == LINES) {
    ++count;
  }

  if (argType == WORDS) {
    count += inWord;
  }

  return count;
}