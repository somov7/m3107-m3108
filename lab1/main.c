#include <stdio.h>
#include <string.h>
// #include <stdlib.h>

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

    while(!feof(Lab1)) {
        fgetc(Lab1);
        bytes++;
    }

    char arr[bytes];
    rewind(Lab1);

    for (int i = 0; i < bytes; i++) {
        arr[i] = fgetc(Lab1);
    }

    for (int i = 0; i < bytes; i++) {
        if (arr[i + 1] == EOF) {
            if (arr[i] == ' ' || arr[i] == '\t' || arr[i] == '\n') {
                break; }
            else if (arr[i] != ' ' && arr[i] != '\t' && arr[i] != '\n') {
                words++; }
        }
        if (arr[i] == '\n') {
            lines++; }
        else if ((arr[i] == ' ' || arr[i] == '\t' || arr[i] == '\n') && i > 0) {
            if (arr[i - 1] == ' ' || arr[i - 1] == '\t' || arr[i] == '\n') {
                continue;
            }
            else {
                words++;
            }
        }
    }

    if ((strcmp(argv[1], "-w") == 0) || (strcmp(argv[1], "--words") == 0)) {
        printf("%d \n", words);
    }
    else if ((strcmp(argv[1], "-l") == 0) || (strcmp(argv[1], "--lines") == 0)) {
        printf("%d \n", lines);
    }
    else if ((strcmp(argv[1], "-c") == 0) || (strcmp(argv[1], "--bytes") == 0)) {
        printf("%d \n", bytes - 1);
    }
    else {
        fprintf(stderr, "%s\n", "Error 228. You can try: '-l', '--lines'; '-c', '--bytes'; '-w', '--words'.");
        return 1;
    }

    return 0;
}