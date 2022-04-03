#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, char ** argv) {
    int c, before_c, counter_Strings = 1, counter_Symbols = 1, counter_Words = 1;
    if ((argc != 3) || argv[2] == NULL) {
        fprintf(stderr, "Проверьте количество параметров\n");
        return EXIT_FAILURE;
    }
    FILE *fin;
    
    fin = fopen(argv[2], "r");
    if (fin == NULL) {
        fprintf(stderr, "Проблема с файлом");
        return EXIT_FAILURE;
    }
    c = fgetc(fin);
    if (c == EOF)  {
        counter_Symbols = 0;
        counter_Words = 0;
    }
    before_c = c;
    c = fgetc(fin);
    while (c != EOF) {
        if ((c == '\n') || (c == '\v') || (c == '\f'))
        {
            counter_Strings++;
        }
        if (isspace(c) && (isspace(before_c) == 0)) {
            counter_Words++;
        }
        before_c = c;
        c = fgetc(fin);
        counter_Symbols++;
    }
    if (isspace(before_c) == 1) {
        counter_Words--;
    }
    fclose(fin);
    if ((strcmp(argv[1], "-l") == 0) || (strcmp(argv[1], "--lines") == 0)) {
        printf("%d\n", counter_Strings);
    }
    else if ((strcmp(argv[1], "-c") == 0) || (strcmp(argv[1], "--bytes") == 0)) {
        printf("%d\n", counter_Symbols);
    }
    else if ((strcmp(argv[1], "-w") == 0) || (strcmp(argv[1], "--words") == 0)) {
        printf("%d\n", counter_Words);
    }
    else {
        fprintf(stderr, "ERROR: no input value\n");
        return EXIT_FAILURE;
    }
    return 0;
}

