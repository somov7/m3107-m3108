//
// Created by Антон Чемодуров on 14.11.2021.
//

#include "archiver.h"
#include <string.h>
#include "Map/prime.h"

#define BUFFER_SIZE 4096

int errn = 0;

const char *errors[] = {
        "no errors",
    "cannot open file",
    "error while reading file",
    "The file already added",
    "FATAL ERROR",
    "max name len 256"
};



Archive *init(const char *arc_name, const char *mod) {
    FILE *arc_file = fopen(arc_name, mod);
    if (arc_file == NULL) {
        perror("cannot open file");
        return NULL;
    }

    Archive *arc = malloc(sizeof(Archive));
    if (arc == NULL)  {
        perror("cannot allocate memory");
        fclose(arc_file);
        return NULL;
    }

    arc->arc = arc_file;
    arc->file_names = set_init(get_prime(1000, 100));
    if (arc->file_names == NULL) {
        fclose(arc_file);
        return NULL;
    }
    return arc;
}

Archive *arc_create(char *arc_name) {
    return init(arc_name, "wb");
}

long get_file_size(FILE *file) {
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);
    return file_size;
}

char *read_file_name(FILE *in, char size) {
    char *file_name = malloc(size + 1);
    if (fread(file_name, 1, size, in) != size) {
        perror("cannot read file");
        free(file_name);
        return NULL;
    }
    file_name[size] = 0;
    return file_name;
}

Archive *arc_open(const char *arc_name) {
    Archive *arc = init(arc_name, "r+b");
    if (arc == NULL) {
        perror(arc_name);
        return NULL;
    }
    long i = 0;
    FILE *in  = arc->arc;
    char name_size;
    long file_size = get_file_size(in);
    long read = 0;
    while (!feof(in) && read < file_size) {
        if (fread(&i, sizeof(long), 1, in) != 1) goto FATAL;

        if (fread(&name_size, 1, 1, in) != 1) goto FATAL;
        char *file_name = read_file_name(in, name_size);

        if (set_exist(arc->file_names, file_name)) {
            errn = 3;
        }
        set_add(&arc->file_names, file_name);
        fseek(in, i, SEEK_CUR);
        read += i + name_size + sizeof(char) + sizeof(long);
    }
    return arc;

FATAL:
        errn = 3;
        arc_close(arc);
        return NULL;
}

long arc_add(Archive *arc, char* file_name) {
    if (set_exist(arc->file_names, file_name)) return -1;

    FILE *in = fopen(file_name, "rb");
    FILE *out = arc->arc;
    if (in == NULL) {
        perror(file_name);
    }

    long file_size = get_file_size(in);

    if (fwrite(&file_size, sizeof(long), 1, out) != 1) {
        goto FATAL;
    }
    size_t file_name_len = strlen(file_name);
    if (file_name_len > 255) {
        errn = 4;
        fclose(out);
    }
    fwrite(&file_name_len, 1, 1, out);
    if (fwrite(file_name, sizeof(char), file_name_len, out) != file_name_len) {
        goto FATAL;
    }

    unsigned long read;
    const int bsize = 4096;
    char buff[4096];
    while (!feof(in) && (read = fread(buff, 1, bsize, in)) > 0) {
        if(fwrite(buff, 1, read, out) != read) goto FATAL;
    }
    set_add(&arc->file_names, file_name);
    return file_size;

    FATAL:
        errn = 3;
        fclose(in);
        return -1;
}

void write(FILE *in, FILE *out, long n) {
    long  buffer_size = 4096;
    char buff[buffer_size];
    while (n > 0 && !feof(in) && !feof(out)) {
        long k = n < buffer_size ? n : buffer_size;
        int read = fread(buff, 1, k, in);
        int write = fwrite(buff, 1, k, out);
        n -= buffer_size;
    }
}

int arc_extract(char *arc_name) {
    FILE *in = fopen(arc_name, "rb");
    if (in == NULL) {
        perror(arc_name);
    }
    char name_size;
    long file_size = get_file_size(in);
    long read = 0;
    while (!feof(in) && read < file_size) {
        long size;
        if (fread(&size, sizeof(long), 1, in) != 1) goto FATAL;

        if (fread(&name_size, 1, 1, in) != 1) goto FATAL;

        char *name = read_file_name(in, name_size);
        FILE *out = fopen(name, "wb");
        if (out == NULL) {
            perror(name);
            goto FATAL;
        }
        write(in, out, size);
        fclose(out);
        read += size + name_size + sizeof(char) + sizeof(long);
    }
    fclose(in);
    return 0;
FATAL:
    errn = 3;
    fclose(in);
    return -1;
}

void arc_close(Archive *arc) {
    fclose(arc->arc);
    set_free(arc->file_names);
    free(arc);
}
const char *get_errorm() {
    return errors[errn];
}