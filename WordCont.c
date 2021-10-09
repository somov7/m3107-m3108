#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    int l = 0, c = 0, ch, w = 0, isword = 0;
    FILE* fp;
    fp = fopen(argv[2], "rt");

    /* При некорректном использовании программы выдаёт ошибку
    и печатает инструкцию */
    if (argc != 3)
    {
        printf("You have entered incorrect parameters: you must enter WordCont.exe [OPTION] filename ");
        return 1;
    }

    /* Проверяет, открылся ли файл */
    if (fp == NULL)
    {
        perror("error:");
        return -1;
    }

    /* Это алгоритм подсчёта строк, слов и числа байт */
    while ((ch = fgetc(fp)) != EOF)
    {
        ++c;
        if (ch == '\n')
        {
            ++l;
        }
        if (ch == ' ' || ch == '\n' || ch == '\t')
        {
            isword = 0;
        }
        else if (isword == 0)
        {
            isword = 1;
            ++w;
        }
    }

    /* Это блок с вызовом опций вывода числа строк, слов и байт */
    if ((strcmp(argv[1], "-l") == 0) || (strcmp(argv[1], "--lines") == 0))
    {
        printf("%d\n", ++l);
    }
    if ((strcmp(argv[1], "-c") == 0 ) || (strcmp(argv[1], "--bytes") == 0))
    {
        printf("%d\n", c);
    }
    if ((strcmp(argv[1], "-w") == 0) || (strcmp(argv[1], "--words") == 0))
    {
        printf("%d\n", w);
    }

    fclose(fp);
    return 0;

}
