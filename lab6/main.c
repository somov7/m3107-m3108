#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Error: not enough arguments");
        return -1;
    }
    char *filename = argv[2];
    char *command = argv[3];

    if (argc > 4) {
        if (strcmp(command, "--create") != 0) {
            printf("Error: excessive number of arguments");
            return -1;
        }

        for (int i = 4; i < argc; i++) {
            FILE *current = fopen(argv[i], "rb");
            if (!current) {
                printf("Error: missing file(s)");
                fclose(current);
                return -1;
            }
        }

        FILE *archive = fopen(filename, "wb");

        char number[5];
        sprintf(number, "%04d", argc - 4);
        fputs(number, archive);
        fputc( '|', archive);

        for (int i = 4; i < argc; i++) {
            fputs(argv[i], archive);
            fputc( '|', archive);

            FILE *current = fopen(argv[i], "rb");
            fseek(current, 0, SEEK_END);
            long amount = ftell(current);
            char length[32];
            sprintf(length, "%ld", amount);
            fputs(length, archive);
            fputc( '|', archive);
            fclose(current);
        }
        fputc( '\n', archive);
        for (int i = 4; i < argc; i++) {
            FILE *current = fopen(argv[i], "rb");
            char now;
            fseek(current, 0, SEEK_END);
            long amount = ftell(current);
            rewind(current);
            for (long j = 0; j < amount; j++) {
                now = fgetc(current);
                fputc(now, archive);
            }
        }
    }

    if (argc == 4) {
        FILE *archive = fopen(filename, "rb");
        if (!archive) {
            printf("Error: no file");
            return -1;
        }

        int count = 0;
        for (int i = 0; i < 4; i++) {
            char temp;
            temp = fgetc(archive);
            count += (temp - '0') * pow(10, 3 - i);
        }

        if (strcmp(command, "--list") == 0) {
            printf("There are %d files:", count);
            int counting = 0;
            while (counting < count * 2 + 1) {
                char currentChar = fgetc(archive);
                if (currentChar == '|') {
                    counting++;
                    if (counting % 2 != 0) printf("\n");
                } else if (counting % 2 != 0) printf("%c", currentChar);
            }
        } else if (strcmp(command, "--extract") == 0) {
            fgetc(archive); // getting '|'
            long file_lengths[count];
            char **file_names;
            int counting = 0;
            while (counting < count * 2 + 1) {
                char currentChar = fgetc(archive);
                if (currentChar == '|') {
                    counting++;
                    if (counting % 2 != 0) printf("\n");
                } else if (counting % 2 != 0) printf("%c", currentChar);
            }

        } else {
            printf("Error: wrong command");
            return -1;
        }
    }



    return 0;
}
