#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define in 1;
#define out 0;

long long unsigned fsize(FILE *filename)
{
    fseek(filename, 0LLU, SEEK_END);
    return ftell(filename);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf((argc != 1) ? "\nYou've entered %d arguments. But should've entered 3" : "\nYou've entered %d argument. But should've entered 3", argc);
        printf("%s%s", "\nUse this template:\nWordCount.exe [Option] filename.(your extension)\nto get started with ->", argv[0]);
        printf("%s", "\n\nAlso make sure u running this programm with Command Promt\nAnd the quantity of arguments is 3\n\n");
        return 1;
    }
    int Option;
    ((strcmp(argv[1], "-l") == 0) || (strcmp(argv[1], "--lines") == 0)) ? (Option = 0) : ((strcmp(argv[1], "-c") == 0) || (strcmp(argv[1], "--bytes") == 0)) ? (Option = 1)
                                                                                     : ((strcmp(argv[1], "-w") == 0) || (strcmp(argv[1], "--words") == 0))   ? (Option = 2)
                                                                                                                                                             : (Option = -1);
    if (Option == -1)
    {
        printf("\n%s", "Option doesn't exist. Use that list of available commands:\n-l or --lines to get lines quantity only");
        printf("\n%s", "-c or --bytes to get size of file in bytes");
        printf("\n%s", "-w or --words to get the quantity of words\n");
        return -1;
    }
    FILE *filename = fopen(argv[2], "r");
    if (filename == NULL)
    {
        printf("%s", "\nSome kinda error occured during file opening\n1) Check if file exists\n2) Check if file is in the same directory as WordCount.c");
        return -1;
    }
    unsigned long long WordCounter = 0, LineCounter = 0;
    char CurrentSym, PreviousSym = '\0';
    int WordState = out;
    int LineState = out;
    while ((CurrentSym = getc(filename)) != EOF)
    {
        PreviousSym = CurrentSym;
        if (CurrentSym == ' ' || CurrentSym == EOF)
        {
            WordState = out;
        }
        else if (WordState == 0)
        {
            WordCounter++;
            WordState = in;
        }
        if (CurrentSym == '\n' || CurrentSym == EOF)
        {
            LineCounter++;
        }
    }
    if (PreviousSym != '\0' && PreviousSym != '\n')
        LineCounter++;
    (Option == 0) ? printf("\nThe quantity of lines is: %llu \nthanks for using!\n\nMade by Oleinikov Vladimir on 13.09.2021\n", LineCounter) : (Option == 1) ? printf("\nThe file size is: %llu bytes\nthanks for using!\n\nMade by Oleinikov Vladimir on 13.09.2021\n", fsize(filename))
                                                                                                                                            : (Option == 2)   ? printf("\nThe quantity of words is: %llu \nthanks for using!\n\nMade by Oleinikov Vladimir on 13.09.2021\n", WordCounter)
                                                                                                                                                              : (Option = -1);
    fclose(filename);
    return 0;
}
