#include <stdio.h>
#include "utils.h"

int getLines(FILE *filePointer) {
    fseek(filePointer, 0L, SEEK_END);
    int fileLength = ftell(filePointer);
    char str[fileLength];
    rewind(filePointer);

    int lines = 0;

    while (fgets(str, fileLength, filePointer)) {
        lines++;
    }

    return lines;
}

int getWords(FILE *filePointer) {
    fseek(filePointer, 0L, SEEK_END);
    int fileLength = ftell(filePointer);
    char str[fileLength];
    rewind(filePointer);

    int words = 0;

    while (fgets(str, fileLength, filePointer)) {
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