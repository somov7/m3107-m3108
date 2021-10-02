#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int getLines(FILE *filePointer) {
    fseek(filePointer, 0L, SEEK_END);
    int fileLength = ftell(filePointer);
    char *str = malloc(fileLength + 1);
    rewind(filePointer);

    int lines = 1;
    if (fileLength == 0) {
        return lines;
    }

    while (fgets(str, fileLength + 1, filePointer)) {
        for (int i = 0; i != '\n'; i++) {
            if (str[i] == '\n') {
                lines++;
            }
        }
    }

    return lines;
}

int getWords(FILE *filePointer) {
    fseek(filePointer, 0L, SEEK_END);
    int fileLength = ftell(filePointer);
    char *str = malloc(fileLength + 1);
    rewind(filePointer);

    if (fileLength == 0) {
        return 0;
    }

    int words = 0;

    while (fgets(str, fileLength + 1, filePointer)) {
        char lastSym = ' ';
        for (int i = 0; str[i] != '\0'; i++) {
            if ((words == 0 && str[i] != ' ') ||
                ((lastSym == ' ' || lastSym == '\n') && str[i] != ' ' && str[i] != '\n')) {
                words++;
            }
            lastSym = str[i];
        }
    }
    return words;
}

long int getBytes(FILE *filePointer) {
    fseek(filePointer, 0L, SEEK_END);
    long int res = ftell(filePointer);
    return res;
}