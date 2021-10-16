#include <stdio.h>
#include <string.h>
#include <stdlib.h>

 int linesCount = 1;
int wordCount = 0;
int bytesCount = 0;
int thisWord = 0;

void text(int doWords, int doLines, int doBytes, FILE *file)
{
    int symbol;
    while((symbol = fgetc(file)) != EOF)
    {
        bytesCount ++;
        if (symbol == '\n' || symbol == '\t' || symbol == ' ')
        {
            if (thisWord == 1)
            {
                wordCount++;
                thisWord = 0;
            }
        } else
        {
            thisWord = 1;
        }

        if (symbol == '\n')
        {
            linesCount++;
        }

        // symbolBefore = symbol;
    }
    if (thisWord == 1)
    {
        wordCount++;
    }

    if (doWords)
    {
        printf("words: %d\n", wordCount);
    }
    if (doLines)
    {
        printf("lines: %d\n", linesCount);
    }
    if (doBytes)
    {
        printf("bytes: %d\n", bytesCount);
    }
}

int main (int argc, char* argv[])
{
    if (argc < 3)
    {
        printf("Isn't correct, use WordCount.exe [options] filename");
        return 1;
    }

    FILE *file;

    int doWords = 0;
    int doLines = 0;
    int doBytes = 0;

    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i],"--words") || !strcmp(argv[i],"-w")) 
        {
            doWords = 1;
        }
        else if (!strcmp(argv[i],"--lines") || !strcmp(argv[i],"-l"))
        {
            doLines = 1;
        }
        else if (!strcmp(argv[i],"--bytes") || !strcmp(argv[i],"-c"))
        {
            doBytes = 1;
        }
        else
        {
            file = fopen(argv[i], "r");
            if (file == NULL) 
            {
                printf("Programm can't open file for reading\n");
                return 1;
            }
        }
    }

    text(doWords, doLines, doBytes, file);

    // int symbolBefore = fgetc(file);
    

    return 0;
}