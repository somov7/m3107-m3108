#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int words = 0, lines = 0, bytes = 0;
    FILE *ptr;
    if (argc == 3) {
        char previous_blank = 0, end_of_line_reached = 0; //Замена bool, 0 - False, 1 - True
        ptr = fopen(argv[2], "r");
        fscanf(ptr, "c");
        char character;
        character = fgetc(ptr);
        while (character != EOF) {
            bytes++;
            if (character == ' ') {
                if (previous_blank == 0 && end_of_line_reached == 0) {
                    words++;
                }
                previous_blank = 1;
                end_of_line_reached = 0;
            } else if (character == '\n') {
                lines++;
                if (previous_blank == 0 && end_of_line_reached == 0) {
                    words++;
                }
                previous_blank = 0;
                end_of_line_reached = 1;
            } else {
                previous_blank = 0;
                end_of_line_reached = 0;
            }
            character = fgetc(ptr);
        }
        if (previous_blank == 0 && end_of_line_reached == 0) {
            words++;
            lines++;
        }
        if (strcmp(argv[1], "-l") == 0 || strcmp(argv[1], "--lines") == 0) {
            printf("Line amount: %d", lines);
        } else if (strcmp(argv[1], "-w") == 0 || strcmp(argv[1], "--words") == 0) {
            printf("Word amount: %d", words);
        } else if (strcmp(argv[1], "-b") == 0 || strcmp(argv[1], "--bytes") == 0) {
            printf("Byte amount: %d", bytes);
        } else {
            printf("Argument input in incorrect");
            return -1;
        }
    } else {
        printf("Incorrect amount of arguments");
        return -1;
    }

    return 0;
}