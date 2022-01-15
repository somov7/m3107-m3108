#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "arc.h"
#include "utility.h"

Arch open(const char *archName)
{
    FILE *file = fopen(archName, "rb");
    fileHeader header;
    size_t arc_size = get_file_size(file);

    fread(&header, sizeof(fileHeader), 1, file);
    Arch arch = {header, file};

    return arch;
}

//создаём сам архив
Arch init(char *archName)
{
    FILE *file = fopen(archName, "wb");

    fileHeader header = {"arc", sizeof(fileHeader), 0};
    Arch arch = {header, file};
    fseek(file, sizeof(fileHeader), SEEK_CUR);

    return arch;
}

long create(Arch *arch, char* fileName)
{
    FILE *fileIn = fopen(fileName, "rb"); //файл, который помещаем в архив
    FILE *fileOut = arch->file; //это наш архив

    if (fileIn == NULL)
    {
        perror(fileName);
        return -1;
    }

    long fileSize = get_file_size(fileIn);
    fwrite(&fileSize, sizeof(long), 1, fileOut);

    size_t fileNameSize = strlen(fileName);
    if (fileNameSize >= 256)
    {
        fclose(fileOut);
        return -1;
    }

    arch->header.fileSize += fileSize + fileNameSize + 5;
    fwrite(&fileNameSize, sizeof(char), 1, fileOut);
    fwrite(fileName, sizeof(char), fileNameSize, fileOut);

    unsigned long isReading;
    int partition = 1024;
    char dataBuffer[partition];
    while (!feof(fileIn) && (isReading = fread(dataBuffer, 1, partition, fileIn)) > 0)
    {
        fwrite(dataBuffer, 1, isReading, fileOut);
    }
    arch->header.fileCounter++;

    return fileSize;
}

char **list(Arch *arch)
{
    FILE *file = arch->file;
    char **files = malloc(arch->header.fileCounter);
    char nameSize;
    long fileSize = arch->header.fileSize;
    long isReading = 0, i = 0;

    while (!feof(file) && isReading < fileSize && i < arch->header.fileCounter)
    {
        long size;
        fread(&size, sizeof(long), 1, file);
        fread(&nameSize, 1, 1, file);

        files[i++] = read_file_name(file, nameSize);
        fseek(file, size, SEEK_CUR);
        isReading += size + nameSize + 5;
    }

    return files;
}

int extract(Arch *arch)
{
    FILE *fileIn = arch->file; //указатель на содержимое архива ес что
    char nameSize;
    long isReading = 0, fileSize = arch->header.fileSize;

    while (!feof(fileIn) && isReading < fileSize)
    {
        long size;
        fread(&size, sizeof(long), 1, fileIn);
        fread(&nameSize, 1, 1, fileIn);

        char *fileName = read_file_name(fileIn, nameSize);
        FILE *fileOut = fopen(fileName, "wb");

        //write(fileIn, fileOut, size);
        long container = size;
        long partition = 1024;
        char dataBuffer[partition];
        while (container > 0 && !feof(fileIn) && !feof(fileOut))
        {
            long buffContainer;
            if (container < partition)
            {
                buffContainer = container;
            }
            else
            {
                buffContainer = partition;
            }
            fread(dataBuffer, 1, buffContainer, fileIn);
            fwrite(dataBuffer, 1, buffContainer, fileOut);
            container -= partition;
        }


        isReading += size + nameSize + 5;

        fclose(fileOut);
    }
    fclose(fileIn);
}

void close(Arch *arch)
{
    fseek(arch->file, 0, SEEK_SET);
    fwrite(&arch->header, sizeof(fileHeader), 1, arch->file);
    fclose(arch->file);
}

