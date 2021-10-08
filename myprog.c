#include <stdio.h>
#include <string.h>
#include <ctype.h> 
#include <locale.h>

int main(int argc, char **argv)
{
    setlocale(LC_ALL, "Rus");
    int symbol = 0;
    int counter_words = 0;
    int counter_lines = 0;
    int counter_bytes = 0;
    int last_symbol = 0;
    if (argc != 3)
    {
        fprintf(stderr, "Ошибка!\nНеверное количество параметров.\n");
        return 1;
    }
    else if (argv[2] == NULL)
    {
        fprintf(stderr, "Ошибка!\nТакого файла не существует!\n");
        return 1;
    }
    FILE *fin;
    fin = fopen(argv[2], "r");
    symbol = fgetc(fin);
    counter_words++;
    while (symbol != EOF)
    {
        if ((isspace(last_symbol) == 0) && (last_symbol !=0))
        {
            if (isspace(symbol) == 1)
            {
                counter_words++;
            }
        }
        if ((symbol == '\n') || (symbol == '\f') || (symbol == '\v'))
        {
            counter_lines++;
        }
        counter_bytes++;
        last_symbol = symbol;
        symbol = fgetc(fin);
    }
    fclose(fin);
    if ((last_symbol == 0) || (isspace(last_symbol) == 1))
    {
        counter_words--;
    }
    if ((last_symbol != '\n') || (last_symbol != '\f') || (last_symbol != '\v'))
        {
            counter_lines++;
        }
    fclose(fin);
    if ((strcmp(argv[1], "-l") == 0) || (strcmp(argv[1], "--lines") == 0)) 
    {
        printf("%d\n", counter_lines);
    }
    else if ((strcmp(argv[1], "-c") == 0) || (strcmp(argv[1], "--bytes") == 0)) 
    {
        printf("%d\n", counter_bytes);
    }
    else if ((strcmp(argv[1], "-w") == 0) || (strcmp(argv[1], "--words") == 0)) 
    {
        printf("%d\n", counter_words);
    }
    else 
    {
        fprintf(stderr, "Ошибка!\nНеверный вызов функции. Используйте другое значение.\n");
        return 1;
    }
    return 0;
}