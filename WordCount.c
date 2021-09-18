#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define in 1;
#define out 0;

int fsize(FILE *filename)
{
    fseek(filename, 0LLU, SEEK_END);
    return ftell(filename);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        if (argc != 1)
            printf("%s%d%s", "\nYou've entered ", argc, " arguments. But should've entered 3");
        else
            printf("%s%d%s", "\nYou've entered ", argc, " argument. But should've entered 3");
        printf("%s%s", "\nUse this template:\nWordCount.exe [Option] filename.(your extension)\nto get started with ->", argv[0]);
        printf("%s", "\n\nAlso make sure u running this programm with Command Promt\nAnd the quantity of arguments is 3\n\n");
        return 1;
    }
    int Option = -1;
    if (strcmp(argv[1], "[-l]") == 0 || strcmp(argv[1], "[--lines]") == 0)
        Option = 0;
    else if (strcmp(argv[1], "[-c]") == 0 || strcmp(argv[1], "[--bytes]") == 0)
        Option = 1;
    else if (strcmp(argv[1], "[-w]") == 0 || strcmp(argv[1], "[--words]") == 0)
        Option = 2;
    if (Option == -1)
    {
        printf("\n%s", "Option doesn't exist. Use that list of available commands:\n[-l] or [--lines] to get lines quantity only");
        printf("\n%s", "[-c] or [--bytes] to get size of file in bytes");
        printf("\n%s", "[-w] or [--words] to get the quantity of words\n");
        return -1;
    }
    FILE *filename = fopen(argv[2], "r");
    if (filename == NULL)
    {
        printf("%s", "\nSome kinda error occured during file opening\n1) Check if file exists\n2) Check if file is in the same directory as WordCount.c");
        return -1;
    }
    unsigned long long WordCounter = 0, LineCounter = 0;
    char CurrentSym;
    int WordState = out;
    int LineState = out;
    while ((CurrentSym = getc(filename)) != EOF)
    {
        if (CurrentSym == ' ' || CurrentSym == ',' || CurrentSym == '.' || CurrentSym == '!' || CurrentSym == '?' || CurrentSym == '\n' || CurrentSym == EOF)
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
            LineState = out;
        }
        else if (LineState == 0)
        {
            LineCounter++;
            LineState = in;
        }
    }
    if (Option == 0)
    {
        printf("%s%llu\n%s", "\nThe quantity of lines is: ", LineCounter, "thanks for using!\n\nMade by Oleinikov Vladimir on 13.09.2021");
        fclose(filename);
        return 0;
    }
    if (Option == 1)
    {
        printf("%s%llu%s", "\nThe file size is: ", fsize(filename), " bytes\nthanks for using!\n\nMade by Oleinikov Vladimir on 13.09.2021");
        fclose(filename);
        return 0;
    }
    if (Option == 2)
    {
        printf("%s%llu\n%s", "\nThe quantity of words is: ", WordCounter, "thanks for using!\n\nMade by Oleinikov Vladimir on 13.09.2021");
        fclose(filename);
        return 0;
    }
}
