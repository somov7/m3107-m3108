#include <stdio.h>
#include <string.h>

unsigned int convert_endians(unsigned int val) {
    return ((val >> 24) & 0x000000FF) |
           ((val >> 8) & 0x0000FF00) |
           ((val << 8) & 0x00FF0000) |
           ((val << 24) & 0xFF000000);
}

void extract(char *file_name) {
    FILE *fp = fopen(file_name, "rb+");
    unsigned long long int now = 0; // Позиция на имя файла
    unsigned long long int start = 0; // Позиция на ин-цию файла
    int chr; // Символ, который мы считываем из файла
    // Считываем все символы до конца файла
    while ((chr = getc(fp)) != EOF) {
        start++;
        if (chr == '\n')
            break;
    }
    // Устанавливаем на начало файла
    fseek(fp, 4, SEEK_SET);
    char filen[128] = {0}; // Имя файла
    unsigned long long int sizefl; // Его размер
    FILE *fl; // Файл с выводом
    while (fscanf(fp, "< %s : %llu > ", filen, &sizefl) != 0) {
        fl = fopen(filen, "wb");
        if (fl == NULL) {
            break;
        }
        // Кол-во байтов, которые остались до конца
        now = ftell(fp);
        fseek(fp, start, SEEK_SET);
        start += sizefl;
        while (sizefl > 0) {
            chr = getc(fp);
            putc(chr, fl);
            sizefl--;
        }
        fseek(fp, now, SEEK_SET);
        fclose(fl);
    }
    printf("Done\n");
}


void list(char *file_name) {     // Костыльная херня
    FILE *fp = fopen(file_name, "rb");
    char filen[128]; // Имена файлов
    unsigned int cnt;
    fread(&cnt, sizeof(int), 1, fp);
    unsigned int number = convert_endians(cnt);
    //printf("%d", number);
    for (unsigned int i = 0; i < number; i++) {
        fscanf(fp, "%s", filen);
        fscanf(fp, "%s", filen);
        printf("%s ", filen); // Выводит имя
        fscanf(fp, "%s", filen);
        fscanf(fp, "%s", filen);
        printf("Number of bytes: %s\n", filen); // Кол-во байт
        fscanf(fp, "%s", filen);
    }
    fclose(fp);
}

void create(char *file_name, int argc, char *argv[]) {
    int tmp;
    FILE *fp = fopen(file_name, "wb");
    FILE *fl;
    unsigned int count = argc - 5;
    //printf("%d", count);
    unsigned long long int name_size[128]; // Кол-во байт в файле
    for (int i = 5; i < argc; i++) {
        fl = fopen(argv[i], "rb");
        if (fl == NULL) {
            continue;
        }
        fseek(fl, 0, SEEK_END);
        name_size[i - 5] = ftell(fl);
        fseek(fl, 0, SEEK_SET);
        fclose(fl);
    }
    unsigned int number = convert_endians(count);
    fwrite(&number, sizeof(int), 1, fp);
    for (int i = 0; i < argc - 5; i++) {
        fprintf(fp, "< %s : %llu > ", argv[i + 5], name_size[i]);
    }
    fprintf(fp, "\n");
    for (int i = 5; i < argc; i++) {
        fl = fopen(argv[i], "rb");
        if (fl == NULL) {
            printf("Impossible to open file %s to read\n", argv[i]);
            continue;
        } else {
            printf("File archived %s\n", argv[i]);
        }
        while ((tmp = getc(fl)) != EOF) {
            putc(tmp, fp);
        }
        fclose(fl);
    }
    printf("Archive created\n");
}

int main(int argc, char *argv[]) {
    int i;
    char *file_name;
    printf("\n");
    for (i = 0; i < argc; i++) {
        if (!strcmp("--file", argv[i])) {
            file_name = argv[i + 1];
        }
        if (!strcmp("--create", argv[i])) {
            create(file_name, argc, argv);
            break;
        }
        if (!strcmp("--extract", argv[i])) {
            extract(file_name);
        }
        if (!strcmp("--list", argv[i])) {
            list(file_name);
        }
    }
    return 0;
}
// arc --file data.arc --create a.txt b.md c.bmp
// arc --file data.arc --list
// arc --file data.arc --extract