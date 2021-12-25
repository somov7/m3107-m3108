#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ARGUMENTS_COUNT 4
#define SIZE_BYTES 4

char *get_filename(FILE *fp) {
    // получаем размер названия файла
    int filename_size = fgetc(fp);
    char *filename = malloc(filename_size);
    // считываем название файла
    for (int j = 0; j < filename_size; j++) {
        filename[j] = (char) fgetc(fp);
    }

    return filename;
}

int get_file_size(FILE *fp) {
    // считываем размер файла
    int file_size = 0;

    file_size += fgetc(fp) + (fgetc(fp) << 8) + (fgetc(fp) << 16) + (fgetc(fp) << 24);

    return file_size;
}

void extract_arc_file(FILE *fp) {
    // считываем количество файлов
    int files_count = fgetc(fp);
    for (int i = 0; i < files_count; i++) {
        // получаем размер названия файла и выделяем память под него
        char *filename = get_filename(fp);

        FILE *file_to_extract = fopen(filename, "wb");

        int file_size = get_file_size(fp);
        // записываем содержимое отдельного файла
        for (int j = 0; j < file_size; j++) {
            fputc(fgetc(fp), file_to_extract);
        }

        fclose(file_to_extract);
    }

    fclose(fp);
}

void print_files(FILE *fp) {
    // получаем количество файлов
    int files_count = fgetc(fp);

    printf("Файлы в архиве: %d\n", files_count);

    for (int i = 0; i < files_count; i++) {
        char *filename = get_filename(fp);

        int file_size = get_file_size(fp);

        for (int j = 0; j < file_size; j++) {
            fgetc(fp);
        }

        printf("%s\n", filename);
    }

    fclose(fp);
}

void create_arc_file(int files_count, char **filenames, char *arc_file) {
    FILE **files_to_pack = malloc(files_count * sizeof(FILE));
    // открываем файлы для архивирования
    for (int j = 0; j < files_count; j++) {
        files_to_pack[j] = fopen(filenames[j], "rb");

        if (files_to_pack[j] == NULL) {
            printf("Файл %s не найден\n", filenames[j]);
            exit(1);
        }
    }

    FILE *arc_file_result = fopen(arc_file, "wb");
    // записываем количество файлов
    fputc(files_count, arc_file_result);

    for (int i = 0; i < files_count; i++) {
        // получаем размер файла
        fseek(files_to_pack[i], 0L, SEEK_END);
        long size = ftell(files_to_pack[i]);
        fseek(files_to_pack[i], 0L, 0L);

        // записываем размер названия файла
        fputc((int) strlen(filenames[i]), arc_file_result);

        // записываем название файла
        for (int j = 0; j < strlen(filenames[i]); j++) {
            fputc(filenames[i][j], arc_file_result);
        }

        // записываем размер файла
        fwrite(&size, SIZE_BYTES, 1, arc_file_result);
        // записываем содержимое файла
        for (int j = 0, chr = fgetc(files_to_pack[i]); j < size && chr != EOF; j++) {
            fputc(chr, arc_file_result);
            chr = fgetc(files_to_pack[i]);
        }
        fclose(files_to_pack[i]);
    }
    fclose(arc_file_result);
}

void arg_parse(int argc, char *argv[]) {
    char *arc_file;

    for (int i = 1; i < argc; i++) {
        char *arg = malloc(sizeof(char) * strlen(argv[i]));
        sprintf(arg, "%s", argv[i]);

        if (strcmp(arg, "--file") == 0) {
            arc_file = argv[i + 1];
            i++;
        } else if (strcmp(arg, "--create") == 0) {
            int files_count = argc - ARGUMENTS_COUNT;

            char **filenames = malloc(files_count * sizeof(char *));

            for (int j = 0; j < files_count; j++) {
                filenames[j] = argv[j + ARGUMENTS_COUNT];
            }

            create_arc_file(files_count, filenames, arc_file);
            exit(0);
        } else if (strcmp(arg, "--extract") == 0) {
            FILE *fp = fopen(arc_file, "rb");

            if (fp == NULL) {
                printf("Файл %s не найден\n", arc_file);
                exit(1);
            }

            extract_arc_file(fp);
            exit(0);
        } else if (strcmp(arg, "--list") == 0) {
            FILE *fp = fopen(arc_file, "rb");

            if (fp == NULL) {
                printf("Файл %s не найден\n", arc_file);
                exit(1);
            }

            print_files(fp);
            exit(0);
        }
    }
}


int main(int argc, char *argv[]) {
    arg_parse(argc, argv);
    return 0;
}
