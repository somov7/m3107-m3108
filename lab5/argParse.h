#ifndef LAB5_ARGPARSE_H
#define LAB5_ARGPARSE_H
#endif //LAB5_ARGPARSE_H
#include <stdio.h>
#include <stdlib.h>

#define MAX_FILENAME_LEN 100
#define MAX_DIRECTORY_LEN 1000

int MAX_ITER = 200; // (inf)
int DUMP_FREQ = 1; // save frequency
char* INPUT_FILE_NAME;
char* DIRECTORY_NAME;

void printUsage(char* err);
void openCorrect(FILE* file);
void argParse(int argv, char** argc);