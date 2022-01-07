#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "id3v2.h"

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

long int get_tag_size(FILE* file)
{
    char* data = (char*) malloc(10);
    fread(&data[0], 1, 10, file);

    struct ID3v2_header id;
    memcpy(&id, &data[0], sizeof(struct ID3v2_header));

    unsigned int tagSize = calcSize(id);

    free(data);
    return tagSize;
}

void read_frame(FILE* file, long int frameSize, int isGet)
{
    char* data = (char*) malloc(frameSize);
    fread(&data[0], 1, frameSize, file);

    if (isGet == 1)
    {
        for (int i = 0; i < frameSize; i++)
        {
            printf("%c", data[i]);
        }
        printf("\n");
    }

    free(data);
}

long int read_frame_header(FILE* file, int isSet)
{
    char* data = (char*) malloc(10);
    fread(&data[0], 1, 10, file);

    struct ID3v2_frame_header frame;
    memcpy(&frame, &data[0], sizeof(struct ID3v2_frame_header));

    unsigned int frameSize = frame.size[0] + frame.size[1] + frame.size[2] + frame.size[3];


    if (isSet == 1)
    {
        for (int i = 0; i < 4; i++)
        {
            printf("%c", frame.identifier[i]);
        }
        printf(" ");
    }

    free(data);
    return frameSize;
}

long int read_frame_id(FILE* file, int isSet, char frameName[])
{
    char* data = (char*) malloc(10);
    fread(&data[0], 1, 10, file);

    struct ID3v2_frame_header frame;
    memcpy(&frame, &data[0], sizeof(struct ID3v2_frame_header));

    unsigned int frameSize = frame.size[0] + frame.size[1] + frame.size[2] + frame.size[3];

    int cmp = 0;
    for (int i = 0; i < 4; i++)
    {
        if (frameName[i] == data[i])
        {
            cmp++;
        }
    }

    if (cmp == 4)
    {
        if (isSet == 1)
        {
            frameSize = 0;
            return frameSize;
        }

        for (int i = 0; i < 4; i++)
        {
            printf("%c", frame.identifier[i]);
        }
        printf(" ");

        read_frame(file, frameSize, 1);
        return 0;
    }
    cmp = 0;

    free(data);
    return frameSize;
}


void show(FILE* file)
{
    long int tagSize = get_tag_size(file); //Читаем заголовок тэга ID3v2 и получаем размер тэга
    long int frameSize;
    int counter = 10;

    while (counter < tagSize)
    {
        frameSize = read_frame_header(file, 1);
        if (frameSize == 0)
        {
            break;
        }

        counter += 10;
        counter += frameSize;
        read_frame(file, frameSize, 1);
    }
}

void get(FILE* file, char frameName[])
{
    long int tagSize = get_tag_size(file); //Читаем заголовок тэга ID3v2 и получаем размер тэга
    long int frameSize;
    int counter = 0;

    while (counter < tagSize)
    {
        frameSize = read_frame_id(file, 0, frameName);

        if (frameSize == 0)
        {
            break;
        }

        counter += 10;
        counter += frameSize;
        read_frame(file, frameSize, 0);
    }
}

void set(FILE* file, char frameName[], char value[], int length, char fileName[])
{
    long int fileSize = get_file_size(file); //Получаем размер файла
    long int tagSize = get_tag_size(file); //Читаем заголовок тэга ID3v2 и получаем размер тэга
    long int frameSize;
    int counter = 0;

    while (counter < tagSize)
    {
        frameSize = read_frame_id(file, 1, frameName);
        if (frameSize == 0)
        {
            break;
        }

        counter += 10;
        counter += frameSize;
        read_frame(file, frameSize, 0);
    }
    fseek(file, counter + 10, SEEK_SET);

    int curptr = read_frame_header(file, 0);
    counter += 20;

    char* ndata = (char* ) malloc(fileSize);
    fseek(file, 0, SEEK_SET);
    fread(&ndata[0], 1, counter , file);

    for (int i = 0; i < length; i++)
    {
        ndata[counter + i] = value[i];
    }

    for (int i = 0; i < 4; i++)
    {
        ndata[counter  + i - 4] = 0;
    }

    ndata[counter - 3] = length;

    fseek(file, counter + curptr, SEEK_SET);
    fread(&ndata[counter + length], 1, fileSize - counter - length, file);

    FILE* nfile = fopen(fileName, "wb");
    fwrite(&ndata[0], sizeof(char), fileSize - (curptr - length), nfile);
    fclose(nfile);

    free(ndata);
}


int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Rus");

    char* fileName = malloc(100* sizeof(char));
    char* frameName = malloc(4 * sizeof(char));
    char* value = malloc(100 * sizeof(char));
    char showFlag = 0;
    char setFlag = 0;
    char getFlag = 0;
    int length = 0;

    for (int i = 1;i < argc; i++){
        if (strcmp(argv[i], "--show") == 0) {
            showFlag = 1;
            continue;
        }
        if (argv[i][2] == 'f') {
            fileName = strpbrk(argv[i], "=") + 1;
        }
        if (argv[i][2] == 'g') {
            getFlag = 1;
            frameName = strpbrk(argv[i], "=") + 1;
            continue;
        }
        if (argv[i][2] == 's') {
            setFlag = 1;
            frameName = strpbrk(argv[i], "=") + 1;
            continue;
        }
        if (argv[i][2] == 'v') {
            value = strpbrk(argv[i], "=") + 1;
            continue;
        }
    }

    length = strlen(value);
    FILE* file = fopen(fileName, "rb+");
    isFile(file); // Проверка на наличие файла

    if (getFlag)
        get(file, frameName);

    if (setFlag)
    {
        if (strlen(frameName) == 4)
        {
            set(file, frameName, value, length, fileName);
        }

        else
        {
            printf("Error: incorrect frame name\n");
            exit(EXIT_FAILURE);
        }
    }

    if (showFlag)
        show(file);

    fclose(file);
    free(fileName);
    free(value);
    free(frameName);
    return 0;
}
