#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        printf("Correct usage: WordCount.exe [option] filename");
        return 1;
    }
    int cwords = 0, cbytes = 0, clines = 1;
    FILE* fp;
    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i],"--words") || !strcmp(argv[i],"-w")) cwords = 1;
        else if (!strcmp(argv[i],"--lines") || !strcmp(argv[i],"-l")) clines = 1;
        else if (!strcmp(argv[i],"--bytes") || !strcmp(argv[i],"-c")) cbytes = 1;
        else
        {
            fp = fopen(argv[i], "r");
            if (fp == NULL) 
            {
                printf("Can't open file\n");
                return 1;
            }
        }
    }

    int words = 0, bytes = 0, lines = 0, reading_word = 0;
    int c;
    while ((c = fgetc(fp)) != EOF)
    {
        bytes++;
        if (c == ' ' || c == '\t' || c == '\n')
        {
            if (reading_word)
            {
                reading_word = 0;
                words++;
            }
        }
        else reading_word = 1;
        if (c == '\n') lines++;
    }
    if (cwords) printf("words: %d\n", words+reading_word);
    if (clines) printf("lines: %d\n", lines+(bytes>0));
    if (cbytes) printf("bytes: %d\n", bytes);

    return 0;
}