#ifndef WORDCOUNT_UTILS_H
#define WORDCOUNT_UTILS_H

const int MAX_LENGTH = 255;

int getLines(FILE *filePointer) {
    char str[MAX_LENGTH];
    int lines = 0;

    while (fgets(str, MAX_LENGTH, filePointer)) {
        lines++;
    }

    return lines;
}

int getWords(FILE *filePointer) {
    char str[MAX_LENGTH];
    int words = 0;

    while (fgets(str, MAX_LENGTH, filePointer)) {
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

#endif //WORDCOUNT_UTILS_H
