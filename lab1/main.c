#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void checkInput(unsigned short argc, FILE *file) {
    if (argc != 3) {
        printf("Two arguments expected!");
        exit(1);
    }
    if (file == NULL) {
        printf("Error opening file!");
        exit(1);
    }

}

void countLines(FILE *file) {
    char ch;
    unsigned linesCounter = 0;
    while ((ch = getc(file)) != EOF) {
        if (ch == '\n') {
            linesCounter += 1;
        }
    }

    linesCounter += 1;
    printf("There are %d lines in file", linesCounter);
}

void countBytes(FILE *file) {
    char ch;
    unsigned bytesCounter = 0;
    while ((ch = getc(file)) != EOF) {
        bytesCounter += 1;
        if (ch == '\n') {
            bytesCounter += 1;
        }
    }
    printf("The size of the file is around %d bytes", bytesCounter);
}

void countWords(FILE *file) {
    char ch;
    unsigned wordsCounter = 0;
    char prevChar = ' ';
    while ((ch = getc(file)) != EOF) {
        if ((!isspace(ch)) && (prevChar == ' ')) {
            prevChar = ch;
        } else if ((isspace(ch)) && (prevChar != ' ')) {
        wordsCounter += 1;
        prevChar = ' ';
        }
    }
    if (prevChar != ' ') {
        wordsCounter += 1;
    }
    printf("There are %d words in file", wordsCounter);
}

int main(int argc, char *argv[]) {
    FILE *importFile;
    importFile = fopen(argv[2], "r");
    checkInput(argc, importFile);
    if ((strcmp(argv[1], "-l") == 0) || (strcmp(argv[1], "--lines") == 0)) {
        countLines(importFile);
        return 0;
    } else if ((strcmp(argv[1], "-c") == 0) || (strcmp(argv[1], "--bytes") == 0)) {
        countBytes(importFile);
        return 0;
    } else if ((strcmp(argv[1], "-w") == 0) || (strcmp(argv[1], "--words") == 0)) {
        countWords(importFile);
        return 0;
    } else {
        printf("Unknown command!");
        return 1;
    }
}
