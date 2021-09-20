#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

int main(int argc, char *argv[]) {
    FILE *fptr;
    char *filename = argv[argc - 1];
    char *filenamePath = malloc(strlen(filename) + 3);
    strcpy(filenamePath, "./");
    strcat(filenamePath, filename);

    if ((fptr = fopen(filename, "r")) == NULL) {
        printf("File not found\n");
        exit(1);
    }

    for (int i = 1; i < argc - 1; i++) {
        if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--lines") == 0) {
            printf("Lines: %d\n", getLines(fptr));
        } else if (strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "--words") == 0) {
            printf("Words: %d\n", getWords(fptr));
        } else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--bytes") == 0) {
            printf("Bytes: %ld\n", getBytes(fptr));
        } else {
            printf("Undefined arg\n");
            exit(1);
        }
        rewind(fptr);
    }
    printf("\n");
    fclose(fptr);

    return 0;
}