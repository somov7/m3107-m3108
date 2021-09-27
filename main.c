#include <stdio.h>
#include <ctype.h>
#include <string.h>
int main(int argc, char ** argv) {
    int c, before_c, counter_Strings = 1, counter_Symbols = 1, counter_Words = 1;
    if ((argc != 3) || argv[2] == NULL) {
        fprintf(stderr, "Проверьте количество параметров\n");
        return 0;
    }
    FILE *fin;
    fin = fopen(argv[2], "r");
    c = fgetc(fin);
    if (c == EOF)  {
        counter_Symbols = 0;
    }
    c = fgetc(fin);
    before_c = c;
    while (c != EOF) {
        if ((c == '\n') || (c == '\v') || (c == '\f'))
        {
            counter_Strings++;
        }
        if ((isspace(c) || (c == EOF)) && (isspace(before_c) == 0)) {
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
    }
    return 0;
}


