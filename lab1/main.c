#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[1]) {
    FILE *Lab1;
    Lab1 = fopen(argv[2], "r");
    if (argv[1] == NULL || argv[2] == NULL) {
        printf("Error 666. Few arguments.");
    }
    else if (Lab1 == NULL){
        printf("Error 1337. Try out new file");
        return 1;
    }


    int bytes = 0, lines = 1, words = 0;

    while(!feof(Lab1)) {
        fgetc(Lab1);
        bytes++;
    }

    char* arr;
    rewind(Lab1);
    arr = (char*)malloc(bytes + 1);

    for (int i = 0; i < bytes; i++) {
        arr[i] = fgetc(Lab1);
    }

    for (int i = 0; i < bytes; i++) {
        if (i == 0 && arr[i] != '\n' && arr[i] != ' ' && arr[i] != '\t') {
            words++;
        }
        if (arr[i] == '\n' && i != 0) {
            lines++;
        }
        if (arr[i] == ' ' || arr[i] == '\t') {
            continue;
        }
        else if (arr[i + 1] == ' ') {
            words++;
        }

    }

    if (strcmp(argv[1], "-w") == 0) {
        printf("%d \n", words);
    }
    else if (strcmp(argv[1], "--words") == 0) {
        printf("%d \n", words);
    }
    else if (strcmp(argv[1], "-l") == 0) {
        printf("%d \n", lines);
    }
    else if (strcmp(argv[1], "--lines") == 0) {
        printf("%d \n", lines);
    }
    else if (strcmp(argv[1], "-c") == 0) {
        printf("%d", bytes - 1);
    }
    else if (strcmp(argv[1], "--bytes") == 0) {
        printf("%d", bytes - 1);
    }
    else {
        printf("Error 228. Try out: '-l', '--lines'; '-c', '--bytes'; '-w', '--words'.");
    }
}