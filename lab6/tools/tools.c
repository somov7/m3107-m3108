#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_SOURCE_FILE_NAME_LENGTH 64

char* getClearFileName(char* string) {
  int stringSize = snprintf(NULL, 0, string);
  for (int i = 0; i < stringSize; ++i) {
    if (string[i] == '/') {
      string += i + 1;
    }
  }
  return string;
}

char* readFileName(FILE* fp) {
  int sourceFileNameLenght = DEFAULT_SOURCE_FILE_NAME_LENGTH;
  char* sourceFileName = calloc(sourceFileNameLenght, sizeof(char));

  char c;
  for (int j = 0; (c = (char)getc(fp)) != '\0'; ++j) {
    if (j == sourceFileNameLenght) {
      sourceFileNameLenght *= 2;
      sourceFileName = realloc(sourceFileName, sourceFileNameLenght);
    }
    sourceFileName[j] = c;
  }

  return sourceFileName;
}