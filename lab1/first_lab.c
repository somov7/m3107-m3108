#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if(argc < 3)
    {
        printf("Incorrect input");
        return 1;
    }
    int linesout = 0, wordsout = 0, bytesout = 0;
    FILE* our_file;
    for(int i = 1; i < argc; i++)
    {
        if(!strcmp(argv[i], "--lines") || !strcmp(argv[i], "-l"))
        {
            linesout = 1;
        }
        else if(!strcmp(argv[i], "--words") || !strcmp(argv[i], "-w"))
        {
            wordsout = 1;
        }
        else if(!strcmp(argv[i], "--bytes") || !strcmp(argv[i], "-c"))
        {
            bytesout = 1;
        }
        else
        {
            our_file = fopen(argv[i], "r");
            if(our_file == NULL)
            {
                printf("Incorrect filename");
                return 1;
            }
        }
    }
    int lines = 0, words = 0, bytes = 0, word_active = 0;
    char that_one;
    while((that_one = fgetc(our_file)) != EOF)
    {
        bytes++;
        if(that_one == '\n')
        {
            lines++;
        }
        if(that_one == '\t' || that_one == ' ' || that_one == '\n')
        {
            if(word_active == 1)
            {
                word_active = 0;
                words++;
            }
        }
        else
        {
            word_active = 1;
        }
    }
    if(linesout == 1)
    {
        if(bytes != 0)
        {
            lines++;
            printf("%d", lines);
        }
        else
        {
            printf("0");
        }
    }
    if(wordsout == 1)
    {
        printf("%d", words + word_active);
    }
    if(bytesout == 1)
    {
        printf("%d", bytes);
    }
    return 0;
}
