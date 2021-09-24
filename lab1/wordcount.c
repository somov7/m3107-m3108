#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    int words = 0, lines = 1, bytes = 0;
    FILE *ptr;
    ptr = fopen(argv[2], "r");
    if (argc == 3 && ptr != NULL) {
        char previous_blank = 1;
        char character;
        character = fgetc(ptr);
        while (character != EOF) {
            bytes++;
            if (isspace(character) != 0) {
                if (previous_blank == 0) {
                    words++;
                }
                if (character == '\n') {
                    lines++;
                }
                previous_blank = 1;
            } else {
                previous_blank = 0;
            }
            character = fgetc(ptr);
        }
        if (previous_blank == 0) {
            words++;
        }
        if (strcmp(argv[1], "-l") == 0 || strcmp(argv[1], "--lines") == 0) {
            printf("Line amount: %d", lines);
        } else if (strcmp(argv[1], "-w") == 0 || strcmp(argv[1], "--words") == 0) {
            printf("Word amount: %d", words);
        } else if (strcmp(argv[1], "-c") == 0 || strcmp(argv[1], "--bytes") == 0) {
            printf("Byte amount: %d", bytes);
        } else {
            fprintf(stderr, "Invalid argument: '%s'", argv[1]);
            return -1;
        }
    } else {
        fprintf(stderr, "Either you didn't put exactly ONE argument or there was an error opening the file");
        return -1;
    }

    return 0;
}