#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void create(char *archive, char **filenamesArr, int num) {

    FILE *fout = fopen(archive, "wb");
    fwrite(&num, sizeof(int), 1, fout);

    for (int i = 0; i < num; i++) {
        int len = (int) strlen(filenamesArr[i]);
        fwrite(&len, sizeof(int), 1, fout);
        fwrite(filenamesArr[i], sizeof(char), len, fout);
        FILE *fin = fopen(filenamesArr[i], "rb");
        fseek(fin, 0L, SEEK_END);
        long fileSize = (long) ftell(fin);
        fwrite(&fileSize, sizeof(long), 1, fout);
        rewind(fin);
        char symbol;
        while (!feof(fin)) {
            symbol = (char)fgetc(fin);
            fputc((int) symbol, fout);

        }
        fclose(fin);
        remove(filenamesArr[i]);
    }

    fclose(fout);
}

void extract(char *archive, int argc) {

    FILE *fin = fopen(archive, "rb");
    int num = 0;
    fread(&num, sizeof(int), 1, fin);

    for (int i = 0; i < num; i++) {
        printf("%d \n", num);
        int len;
        int trash;
        char filename[argc];
        fread(&len, sizeof(int), 1, fin);
        fread(filename, sizeof(char), len, fin);
        printf("len %d ", len);
        FILE *fout = fopen(filename, "wb");
        long fileSize = 0;
        fread(&fileSize, sizeof(long), 1, fin);

        int count = 0;
        while (count < fileSize) {
            char symbol;
            symbol = (char)fgetc(fin);
            fputc((int) symbol, fout);
            count++;
        }
        fread(&trash, 1, 1, fin);
        fclose(fout);

    }
    fclose(fin);
    remove(archive);
}

void showFiles(char *archive, int argc) {

    FILE *fin = fopen(archive, "rb");
    int num = 0;
    fread(&num, sizeof(int), 1, fin);

    for (int i = 0; i < num; i++) {
        int len;
        int trash;
        char filename[argc];
        fread(&len, sizeof(int), 1, fin);
        fread(filename, sizeof(char), len, fin);
        FILE *fout = fopen(filename, "wb");
        long fileSize = 0;
        fread(&fileSize, sizeof(long), 1, fin);

        printf("File name : %s \n", filename);

        int count = 0;

        while (count < fileSize) {
            char symbol;
            symbol = (char)fgetc(fin);
            fputc((int) symbol, fout);
            count++;
        }
        fread(&trash, 1, 1, fin);
        fclose(fout);
        remove(&filename[i]);
    }
    fclose(fin);
}

int main(int argc, char **argv) {
    char **filenamesArr = calloc(argc, sizeof(char*));
    char *arcName;

    bool flag = false;
    int filesCounter = 0;
    for (int i = 1; i < argc; i++){
        if (!strcmp("--file", argv[i])){
            arcName = argv[i+1];
            flag = true;
        }
    }
    if (!flag){
        printf("Invalid Archive Name");
        return EXIT_FAILURE;
    }
    for (int i = 1; i < argc; i++){
        if (!strcmp("--create", argv[i])){
            for (int j = i + 1; j < argc; j++){
                filenamesArr[filesCounter] = argv[j];
                filesCounter++;
            }
            create(arcName, filenamesArr, filesCounter);

        }
        else if (!strcmp("--extract", argv[i])){
            extract(arcName, argc);

        }
        else if (!strcmp("--list", argv[i])){
            showFiles(arcName, argc);

        }

    }

    return 0;
}