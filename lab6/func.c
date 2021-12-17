#include "func.h"
int end_of_file (FILE *file) {
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}

void create(FILE *arc_file, int num_of_files, char*files[]) {
    FILE *file;
    for (int i = 0; i < num_of_files; i++) {
        unsigned int fn_len = strlen(files[i]);
        fwrite(&fn_len, sizeof(unsigned int), 1, arc_file);
        for (int j = 0; j < fn_len; j++)
            fputc(files[i][j], arc_file);

        file = fopen(files[i], "rb");
        if (!file) {
            printf("fail\n");
        }
        //printf("%s", files[i]);
        int file_len = end_of_file (file);
        fwrite(&file_len, sizeof(unsigned int), 1, arc_file);
        for (int j = 0; j < file_len; j++)
            fputc(getc(file), arc_file);
        fclose(file);
    }
    fclose(arc_file);
}

void extract(FILE *arc_file) {
    FILE *new_file;
    int eof = end_of_file(arc_file);
    while (ftell(arc_file) < eof) {
        unsigned int size, size2;


        fread(&size, sizeof(unsigned int), 1, arc_file);
        char file_name[size];
        fread(&file_name, size, 1, arc_file);
        new_file = fopen(file_name, "wb");
        //printf("%s\n %d\n", file_name, size);
        if (!new_file)
            printf("fail\n");
        fread(&size2, sizeof(unsigned int), 1, arc_file);
        for (int i = 0; i < size2; i++)
            fputc(getc(arc_file), new_file);
        fclose(new_file);
    }
    fclose(arc_file);
}

void list(FILE *arc_file) {
    int eof = end_of_file(arc_file);

    while (ftell(arc_file) < eof) {
        int size, file_len;

        fread(&size, sizeof(unsigned int), 1, arc_file);
        char file_name[size];
        fread(&file_name, size, 1, arc_file);
        fread(&file_len, sizeof(unsigned int), 1, arc_file);
        printf("%s\n", file_name);
        fseek(arc_file, file_len, SEEK_CUR);
    }
    fclose(arc_file);
}