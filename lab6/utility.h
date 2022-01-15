#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

long get_file_size(FILE *file)
{
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    return fileSize;
}

char *read_file_name(FILE *file, char nameSize)
{
    char *fileName = malloc(nameSize + 1);
    fread(fileName, 1, nameSize, file);
    fileName[nameSize] = 0;
    return fileName;
}

#endif // UTILITY_H_INCLUDED
