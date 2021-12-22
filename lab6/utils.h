#ifndef FILE_ARCHIVER_UTILS_H
#define FILE_ARCHIVER_UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

long int getSize(FILE *filePointer);

void create(int filesCount, FILE **files, char **filenames, char *filename);

void extract(FILE *filePointer);

void printList(FILE *filePointer);

#endif
