#define TEMP_FILE "info.txt"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

void printUsage(char *str);
void openCorrect(FILE *file);
char *getFileName(char *filePath);
uint32_t getHeaderSize(char **str, int numberFiles);
void getInfo(char **str, int numberFiles);
void inCompress(char **str, int numberFiles, char *arcName);
