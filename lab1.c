#include <stdio.h>
int main(int argc, char **argv)
{
    int symbol = 0;
    int previous_symbol = 0;
    int number_of_words = 1;
    int number_of_lines = 1;
    int number_of_bytes = 0;
    if (argc != 3)
    {
        fprintf(stderr, "error\ninvalid number of parameters");
    }
    else
    {
        FILE *fin;
        fin = fopen(argv[2], "r");
        if (fin == NULL)
        {
            fprintf(stderr, "error\nthe file does not exist");
        }
        else
        {
            symbol = fgetc(fin);
            while (symbol != EOF)
            {
                if ((isspace(previous_symbol) == 0) && ((previous_symbol != 0)))
                {
                    if (isspace(symbol))
                    {
                        number_of_words++;
                    }
                }
                if ((symbol == 10))
                {
                    number_of_lines++;
                }
                number_of_bytes++;
                previous_symbol = symbol;
                symbol = fgetc(fin);
            }
            if ((isspace(previous_symbol)) || (previous_symbol == 0))
            {
                number_of_words--;
            }
            if ((strcmp(argv[1], "-l") == 0) || (strcmp(argv[1], "--lines") == 0))
            {
                printf("%d", number_of_lines);
            }
            else if ((strcmp(argv[1], "-c") == 0) || (strcmp(argv[1], "--bytes") == 0))
            {
                printf("%d", number_of_bytes);
            }
            else if ((strcmp(argv[1], "-w") == 0) || (strcmp(argv[1], "--words") == 0))
            {
                printf("%d", number_of_words);
            }
            else
            {
                fprintf(stderr, "error\ninvalid argument");
            }
        }
        fclose(fin);
    }
    return 0;
}