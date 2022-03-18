#include <stdio.h>
#include <string.h>


int main(int argc, char **argv) {

    int simvol, linee, slova, pamyt, x;
    FILE *fin;
    fin = fopen(argv[2], "r");

    x = 0, pamyt = 0, slova = 0, linee = 1;
    while ((simvol = fgetc(fin)) != EOF) {
        if (simvol == ' ' || simvol == '\n' || simvol == '\t')
            x = 0;
        else if (x == 0) {
            x = 1;
            ++slova;
        }
        if (simvol == '\n')
            ++linee;
        if (simvol == '\n')
            pamyt += 2;
        else
            ++pamyt;
    }


    if (strcmp(argv[1], "-l") == 0) {
        printf("%d", linee);
    }
    if (strcmp(argv[1], "-w") == 0)  {
        printf("%d", slova);
    }
    if (strcmp(argv[1], "-c") == 0)  {
        printf("%d", pamyt);
    }
    else {
        printf(-1);
    }


    fclose(fin);
}

