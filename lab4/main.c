#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct {
    char id[3];
    char version[2];
    char flags;
    char size[4];
} id3_Header;

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

int main(int argc, char *argv[]){
    FILE *importFile;
    importFile = fopen(argv[2], "r+b");
    checkInput(argc, importFile);
}