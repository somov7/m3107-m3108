#define TEMP_FILE "info.txt"
#define MAX_FILENAME_LEN 1000
#define BUFFER_SIZE 4096
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

char *getFileName(char **filePath);
uint32_t getHeaderSize(char **str, int numberFiles);
void getInfo(char **str, int numberFiles);
void inCompress(char **str, int numberFiles, char *arcName);
void outCompress(char* arcName);
void showFiles(char* arcName);
