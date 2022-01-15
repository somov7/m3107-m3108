#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "id3v2.h"

#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED
#pragma pack(push, 1)

long int get_file_size(FILE* file)
{
    long int fileSize = 0;
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (fileSize <= 0)
    {
        return -1;
    }

    return fileSize;
}
void isFile(FILE *file)
{
    if (file == NULL)
    {
        perror("Can't find that file in such directory");
        exit(EXIT_FAILURE);
    }
    else
        return;
}



#endif // UTILITY_H_INCLUDED
