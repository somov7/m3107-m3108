#include "arc.h"
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>



Arc arc_create(char *arc_name) {
    FILE *file = fopen(arc_name, "wb");
    Header h = {"arc", sizeof(Header), 0};
    Arc arc = {h, file};
    fseek(file, sizeof(Header), SEEK_CUR);
    return arc;
}

long get_file_size(FILE *file) {
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);
    return file_size;
}

char *read_file_name(FILE *in, char size) {
    char *file_name = malloc(size + 1);
    fread(file_name, 1, size, in);
    file_name[size] = 0;
    return file_name;
}

Arc arc_open(const char *arc_name) {
    FILE *file = fopen(arc_name, "rb");
    size_t arc_size = get_file_size(file);
    Header h;
    fread(&h, sizeof(Header), 1, file);
    Arc arc = {h, file};
    return arc;
}



void write(FILE *in, FILE *out, long n) {
    long  buffer_size = 4096;
    char buff[buffer_size];
    while (n > 0 && !feof(in) && !feof(out)) {
        long k = n < buffer_size ? n : buffer_size;
        fread(buff, 1, k, in);
        fwrite(buff, 1, k, out);
        n -= buffer_size;
    }
}

int arc_extract(Arc *arc) {
    FILE *in = arc->file;
    char name_size;
    long file_size = arc->header.file_size;
    long read = 0;
    printf("%d\n", ftell(in));
    while (!feof(in) && read < file_size) {
        long size;
        fread(&size, sizeof(long), 1, in);
        fread(&name_size, 1, 1, in);

        char *name = read_file_name(in, name_size);
        FILE *out = fopen(name, "wb");

        write(in, out, size);
        fclose(out);
        read += size + name_size + sizeof(char) + sizeof(long);
    }
    fclose(in);
}

long arc_add(Arc *arc, char* file_name) {
    FILE *in = fopen(file_name, "rb");
    FILE *out = arc->file;
    if (in == NULL) {
        perror(file_name);
        return -1;
    }

    long file_size = get_file_size(in);
    fwrite(&file_size, sizeof(long), 1, out);
    size_t file_name_len = strlen(file_name);
    if (file_name_len > 255) {
        fclose(out);
        return -1;
    }
    arc->header.file_size += file_size + file_name_len + sizeof(char) + sizeof(long);
    fwrite(&file_name_len, 1, 1, out);
    fwrite(file_name, sizeof(char), file_name_len, out);
    //write(in, out, file_size);
    unsigned long read;
    const int bsize = 4096;
    char buff[4096];
    while (!feof(in) && (read = fread(buff, 1, bsize, in)) > 0) {
        fwrite(buff, 1, read, out);
    }
    arc->header.files++;
    return file_size;
}

char **get_list(Arc *arc) {
    char **names = malloc(arc->header.files);
    FILE *in = arc->file;
    char name_size;
    long file_size = arc->header.file_size;
    long read = 0;
    long i = 0;
    while (!feof(in) && read < file_size && i < arc->header.files) {
        long size;
        fread(&size, sizeof(long), 1, in);
        fread(&name_size, 1, 1, in);

        names[i++] = read_file_name(in, name_size);
        fseek(in, size, SEEK_CUR);
        read += size + name_size + sizeof(char) + sizeof(long);
    }
    return names;
}

void arc_close(Arc *arc) {
    rewind(arc->file);
    fwrite(&arc->header, sizeof(Header), 1, arc->file);
    fclose(arc->file);
}
