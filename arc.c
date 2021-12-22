#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>

#define MAX_FILE_NAME_SIZE 260

uint32_t fsize(FILE *file)
{
    fseek(file, 0L, SEEK_END);
    uint32_t size = ftell(file);
    fseek(file, 0L, SEEK_SET);
    return size;
}

char *getName(char *path)
{
    char *str = path;
    int pathLength = strlen(path);

    for (int i = pathLength - 1; i >= 0; i--)
        if (path[i] == '/' || path[i] == '\\')
        {
            str = (char *)malloc(pathLength - i);
            memcpy(str, path + i + 1, pathLength - 1 - i);
            str[pathLength - i - 1] = '\0';
            break;
        }
    return str;
}

void createArch(char **argv, int *numberOfFiles, char archName[])
{
    FILE *archive = fopen(archName, "wb");
    if (archive == NULL)
    {
        printf("error during %s opening", archName);
        exit(EXIT_FAILURE);
    }

    char *filename = (char *)(malloc(sizeof(char) * MAX_FILE_NAME_SIZE));
    int numberOfFilesToCheck = *numberOfFiles;
    FILE *fileForPacking;

    for (int i = 0; i < numberOfFilesToCheck; i++)
    {
        fileForPacking = fopen(argv[i + 4], "rb");
        if (fileForPacking == NULL)
        {
            (*numberOfFiles)--;
        }
        fclose(fileForPacking);
    }

    uint32_t infoAboutFilesQuantity = (uint32_t)(*numberOfFiles);
    fwrite(&infoAboutFilesQuantity, 4, 1, archive);

    for (int i = 0; i < numberOfFilesToCheck; i++)
    {
        bool isCompressed = false;

        fileForPacking = fopen(argv[i + 4], "rb");
        if (fileForPacking == NULL)
        {
            printf("can't find %s\n", argv[i + 4]);
            continue;
        }

        uint32_t packingFilenameSize = (uint32_t)(strlen(argv[i + 4]));
        fwrite(&packingFilenameSize, 4, 1, archive);

        uint32_t packingSize = fsize(fileForPacking);
        fwrite(&packingSize, 4, 1, archive);

        filename = getName(argv[i + 4]);

        for (int j = 0; j < packingFilenameSize; j++)
            fwrite(&filename[j], 1, 1, archive);

        char *metaData = (char *)malloc(packingSize);
        fread(metaData, 1, packingSize, fileForPacking);
        fwrite(metaData, 1, packingSize, archive);
        free(metaData);

        if (fclose(fileForPacking) != 0)
        {
            printf("%p ", fileForPacking);
            perror("can't be closed");
        }
        fileForPacking = NULL;

        if (remove(argv[i + 4]) != 0)
        {
            printf("%s ", argv[i + 4]);
            perror("can't be deleted");
        }
    }

    fclose(archive);
}

void showArch(char *archName)
{
    FILE *archive = fopen(archName, "rb");
    if (archive == NULL)
    {
        printf("archive %s is damaged or doesn't exist", archName);
        exit(EXIT_FAILURE);
    }

    uint32_t filesQuantity;
    fread(&filesQuantity, 4, 1, archive);

    for (uint32_t i = 0; i < filesQuantity; i++)
    {
        uint32_t filenameSize, fileSize;
        fread(&filenameSize, 4, 1, archive);
        fread(&fileSize, 4, 1, archive);

        char *filename = (char *)malloc(sizeof(char) * (filenameSize + 1));
        fread(filename, 1, filenameSize, archive);
        filename[filenameSize] = '\0';

        fseek(archive, fileSize, SEEK_CUR);

        printf("%s\n", filename);
        free(filename);
    }

    fclose(archive);
}

void unArch(char *archName)
{
    FILE *archive = fopen(archName, "rb");
    if (archive == NULL)
    {
        printf("archive %s is damaged or doesn't exist", archName);
        exit(EXIT_FAILURE);
    }

    uint32_t filesQuantity;
    fread(&filesQuantity, 4, 1, archive);

    for (uint32_t i = 0; i < filesQuantity; i++)
    {
        FILE *unpackingFile;
        uint32_t filenameSize, fileSize;
        fread(&filenameSize, 4, 1, archive);
        fread(&fileSize, 4, 1, archive);

        char *filename = (char *)malloc(sizeof(char) * (filenameSize + 1));
        fread(filename, 1, filenameSize, archive);
        filename[filenameSize] = '\0';

        if ((unpackingFile = fopen(filename, "wb")) == NULL)
        {
            printf("error during path creating for %s", filename);
            exit(EXIT_FAILURE);
        }

        char *metaData = (char *)(malloc(sizeof(char) * fileSize));
        fread(metaData, 1, fileSize, archive);
        fwrite(metaData, 1, fileSize, unpackingFile);

        free(filename);
    }
    if (fclose(archive) != 0)
    {
        printf("%p ", archive);
        perror("can't be closed");
    }
    archive = NULL;

    if (remove(archName) != 0)
    {
        printf("%s ", archName);
        perror("can't be deleted");
    }
}

int main(int argc, char **argv)
{
    char archName[MAX_FILE_NAME_SIZE];

    if (argc < 4)
    {
        printf("incorrect input");
        exit(EXIT_FAILURE);
    }

    int numberOfFiles = 0, command1 = 0, command2 = 0, command3 = 0;
    for (int i = 0; i < argc; i++)
    {
        if (!strcmp("--file", argv[i]))
            strcpy(archName, argv[i + 1]);

        else if (!strcmp("--create", argv[i]))
        {
            numberOfFiles = argc - 4;
            command1 = 1;
            break;
        }
        else if (!strcmp("--extract", argv[i]))
        {
            command2 = 1;
            break;
        }
        else if (!strcmp("--list", argv[i]))
        {
            command3 = 1;
            break;
        }
    }

    if (!command1 && !command2 && !command3)
    {
        printf("invalid commands\n"
               "available:\n"
               "--create file1.ext file2.ext etc.\n"
               "--list\n");
        exit(EXIT_FAILURE);
    }

    if (command1)
    {
        createArch(argv, &numberOfFiles, archName);
    }

    if (command2)
    {
        unArch(archName);
    }

    if (command3)
    {
        showArch(archName);
    }

    return 0;
}