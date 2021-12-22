#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

int main(int argc, char *argv[]) {
    char *filename;

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--file")) {
            filename = argv[i + 1];
            i++;
        }

        if (!strcmp(argv[i], "--create")) {
            int filesCount = argc - 4;

            FILE **files = malloc(filesCount * sizeof(FILE));
            char **filenames = malloc(filesCount * sizeof(char *));


            for (int j = 0; j < filesCount; j++) {
                filenames[j] = argv[j + 4];
            }

            for (int j = 0; j < filesCount; j++) {
                files[j] = fopen(argv[j + 4], "rb");

                if (files[j] == NULL) {
                    printf("File %s not found\n", argv[j + 4]);
                    filesCount--;
                    j--;
                }
            }

            create(filesCount, files, filenames, filename);
            exit(EXIT_SUCCESS);
        }

        if (!strcmp(argv[i], "--extract")) {
            FILE *fptr = fopen(filename, "rb");

            if (fptr == NULL) {
                printf("File %s not found\n", filename);
                exit(EXIT_FAILURE);
            }

            extract(fptr);
            exit(EXIT_SUCCESS);
        }

        if (!strcmp(argv[i], "--list")) {
            FILE *fptr = fopen(filename, "rb");

            if (fptr == NULL) {
                printf("File %s not found\n", filename);
                exit(EXIT_FAILURE);
            }

            printList(fptr);
            exit(EXIT_SUCCESS);
        }
    }
}