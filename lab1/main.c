#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void checkInput(unsigned short argc, FILE *file) {
    if (argc != 3) {
        fprintf(stderr, "Two arguments expected!");
        exit(1);
    }
    if (file == NULL) {
        fprintf(stderr, "Error opening file!");
        exit(1);
    }

}

unsigned countLines(FILE *file) {
    char ch;
    unsigned linesCounter = 0;
    while ((ch = getc(file)) != EOF) {
        if (ch == '\n') {
            linesCounter += 1;
        }
    }

    linesCounter += 1;
    return linesCounter;
}

unsigned countBytes(FILE *file) {
    char ch;
    unsigned bytesCounter = 0;
    while ((ch = getc(file)) != EOF) {
        bytesCounter += 1;
    }
    return bytesCounter;
}

unsigned countWords(FILE *file) {
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
    return wordsCounter;
}

int main(int argc, char *argv[]) {
    FILE *importFile;
    importFile = fopen(argv[2], "r");
    checkInput(argc, importFile);
    if ((strcmp(argv[1], "-l") == 0) || (strcmp(argv[1], "--lines") == 0)) {
        printf("There are %d lines in file", countLines(importFile));
    } else if ((strcmp(argv[1], "-c") == 0) || (strcmp(argv[1], "--bytes") == 0)) {
        printf("The size of the file is %d bytes", countBytes(importFile));
    } else if ((strcmp(argv[1], "-w") == 0) || (strcmp(argv[1], "--words") == 0)) {
        printf("There are %d words in file", countWords(importFile));
    } else {
        fprintf(stderr, "Unknown command!");
        return 1;
    }
    return 0;
}
