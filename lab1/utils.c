#include <stdio.h>
#include "utils.h"

int getLines(FILE *filePointer) {
    int sym;
    int lines = 1;
    while ((sym = fgetc(filePointer)) != EOF) {
        if (sym == '\n') {
            lines++;
        }
    }

    return lines;
}

int getWords(FILE *filePointer) {
    int words = 0;
    int sym;
    int lastSym = ' ';
    while ((sym = fgetc(filePointer)) != EOF) {
        if ((words == 0 && sym != ' ' && sym != '\t' && sym != '\n') ||
            ((lastSym == ' ' || lastSym == '\t' || lastSym == '\n') && sym != ' ' && sym != '\t' &&
                    sym != '\n')) {
            words++;
        }
        lastSym = sym;
    }
    return words;
}

long int getBytes(FILE *filePointer) {
    fseek(filePointer, 0L, SEEK_END);
    long int res = ftell(filePointer);
    return res;
}