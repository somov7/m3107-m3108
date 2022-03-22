#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Количество строк
int lines_count(FILE *file)
{
    char c = fgetc(file);
    int counter = 1;

    while (c != EOF)
    {
        if (c == '\n')
            counter++;
        c = fgetc(file);
    }

    return counter;
}

// Количество бит
int bytes_count(FILE *file)
{
    int c = fgetc(file);
    int counter = 0;

    while (c != EOF)
    {
        counter++;
        c = fgetc(file);
    }

    return counter;
}

// Количество слов
int words_count(FILE *file)
{
    int counter = 0;
    fscanf(file," ");

    while (!feof(file))
    {
        counter++;
        fscanf(file,"%*s ");
    }

    return counter;
}

int main(int argc, char *argv[])
{
    FILE *file = fopen(argv[2], "r");

    if (strcmp(argv[1], "-l") == 0 || strcmp(argv[1], "--lines") == 0)
         printf("This file has %d lines", lines_count(file));
    else if(strcmp(argv[1], "-c") == 0 || strcmp(argv[1], "--bytes") == 0)
        printf("This file weights %d bytes", bytes_count(file));
    else if(strcmp(argv[1], "-w") == 0 || strcmp(argv[1], "--words") == 0)
        printf("This file has %d words", words_count(file));

    fclose(file);
    return 0;
}
