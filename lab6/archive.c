/* Orlov Aleksandr, 12-5, M3107, 15.12.2021 */

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "archive.h"

unsigned int fsize(FILE *fp){
    fseek(fp, 0L, SEEK_END);
    unsigned int sz=ftell(fp);
    fseek(fp,0L,SEEK_SET);
    return sz;
}

char *find_filename(char *filepath) {
    char *res = filepath;
    int filep_len = strlen(filepath);

    for (int i = filep_len - 1; i >= 0; i--)
        if (filepath[i] == '/' || filepath[i] == '\\') {
            res = (char *) malloc(filep_len - i);
            memcpy(res, filepath + i + 1, filep_len - 1 - i);
            res[filep_len - i - 1] = '\0';
            break;
        }
    return res;
}

int create_archive(char *argv[], int files, char *filename) {
    unsigned char files_number, byte;
    char *temp_filename, *text;
    FILE *output, *f;

    if (files > 255)
        return 0;

    output = fopen(filename, "wb");
    if (output == NULL)
        return 0;

    files_number = files;
    fwrite(&files_number, 1, 1, output);

    for (int i = 0; i < files; i++) {
        unsigned int file_size;

        f = fopen(argv[i + 4], "rb");
        temp_filename = find_filename(argv[i + 4]);
        if (f == NULL)
            continue;

        file_size = fsize(f);
        if (file_size > 4294967294) {
            fclose(f);
            continue;
        }
        if (strlen(temp_filename) > 255) {
            fclose(f);
            continue;
        }
        byte = strlen(temp_filename);

        /* Writing filename size byte */
        fwrite(&byte, 1, 1, output);

        /* Writing file size bytes */
        fwrite(&file_size, sizeof (unsigned int), 1, output);

        for (int j = 0; j < byte; j++)
            fwrite(&temp_filename[j], 1, 1, output);

        text = (char *) malloc(file_size);
        fread(text, 1, file_size, f);
        fwrite(text, 1, file_size, output);
        fclose(f);
        free(text);
    }
    fclose(output);
    return 1;
}

int unpack_archive(char *filename) {
    FILE *input;
    char files;

    input = fopen(filename, "rb");

    if (input == NULL)
        return 0;

    fread(&files, 1, 1, input);

    for (int i = 0; i < files; i++) {
        FILE *f;
        char filename_size;
        int file_size;
        char *temp_filename, *text;

        fread(&filename_size, 1, 1, input);
        fread(&file_size, 4, 1, input);

        temp_filename = (char *) malloc(filename_size + 1);
        fread(temp_filename, 1, filename_size, input);
        temp_filename[filename_size] = '\0';

        f = fopen(temp_filename, "wb");
        if (f == NULL)
            continue;

        text = (char *) malloc(file_size);
        fread(text, 1, file_size, input);

        fwrite(text, 1, file_size, f);
        fclose(f);
        free(temp_filename);
        free(text);
    }
    return 1;
}

void file_list(char *filename) {
    FILE *input;
    char files;

    input = fopen(filename, "rb");

    if (input == NULL)
        return;

    fread(&files, 1, 1, input);

    for (int i = 0; i < files; i++) {
        char filename_size;
        int file_size;
        char *temp_filename, *text;

        fread(&filename_size, 1, 1, input);
        fread(&file_size, 4, 1, input);

        temp_filename = (char *) malloc(filename_size + 1);
        fread(temp_filename, 1, filename_size, input);
        temp_filename[filename_size] = '\0';

        fseek(input, file_size, SEEK_CUR);

        printf("Filename %i: %s\n", i, temp_filename);
        free(temp_filename);
    }
}