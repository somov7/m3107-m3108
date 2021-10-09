#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    FILE *Lab1;
    if (argv[2] != NULL) {
        Lab1 = fopen(argv[2], "r");
        if (Lab1 == NULL) {
            fprintf(stderr, "%s\n", "Error 1337. File is not exist. Try out new / another file");
            return 1;
        }
    }
    if (argv[1] == NULL || argv[2] == NULL) {
        fprintf(stderr, "%s\n", "Error 666. Few arguments.");
        return 1;
    }

    int bytes = 0, lines = 1, words = 0;

    while (!feof(Lab1)) {
        bytes++;
        if (fgetc(Lab1) == '\n') {
            lines++;
        }
        if (fgetc(Lab1) != ' ' && fgetc(Lab1) != '\t' && fgetc(Lab1) != '\v' && fgetc(Lab1) != '\r' && fgetc(Lab1) != '\0') {
            words++;
        }
    }

    if ((strcmp(argv[1], "-w") == 0) || (strcmp(argv[1], "--words") == 0)) {
        printf("%d \n", words);
    }
    else if ((strcmp(argv[1], "-l") == 0) || (strcmp(argv[1], "--lines") == 0)) {
        printf("%d \n", lines);
    }
    else if ((strcmp(argv[1], "-c") == 0) || (strcmp(argv[1], "--bytes") == 0)) {
        printf("%d \n", bytes);
    }
    else {
        fprintf(stderr, "%s\n", "Error 228. You can try: '-l', '--lines'; '-c', '--bytes'; '-w', '--words'.");
        return 1;
    }
    return 0;
}