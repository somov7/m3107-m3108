#include <stdio.h>
#include <string.h>
#include <stdlib.h>


char *get_file_name(char *path) {
    char *name;
    int len = strlen(path);
    int i = len - 1;
    while (i >= 0 && path[i] != '/' && path[i] != '\\')
        i--;

    name = (char *) malloc(len - i);
    name[len - i - 1] = '\0';
    memcpy(name, path + i + 1, len - 1 - i);
    return name;    
}

int get_size(FILE *f){
    fseek(f, 0L, SEEK_END);
    int size= ftell(f);
    fseek(f, 0L, SEEK_SET);
    return size;
}


void get_archive(char *name) {
    FILE *fin, *fout;
    char n, nsize;
    int fsize;
    char *tname, *text;

    fin = fopen(name, "rb");
    fread(&n, 1, 1, fin);

    for (int i = 0; i < n; i++) {
        fread(&nsize, 1, 1, fin);
        fread(&fsize, 4, 1, fin);

        tname = (char *) malloc(nsize + 1);
        tname[nsize] = '\0';
        fread(tname, 1, nsize, fin);

        fout = fopen(tname, "wb");

        text = (char *) malloc(fsize);
        fread(text, 1, fsize, fin);

        fwrite(text, 1, fsize, fout);
        fclose(fout);
        free(tname);
        free(text);
    }
}

void list(char *name) {
    FILE *fin;
    char n, nsize;
    int fsize;
    char *tname, *text;

    fin = fopen(name, "rb");
    fread(&n, 1, 1, fin);

    for (int i = 0; i < n; i++) {
        fread(&nsize, 1, 1, fin);
        fread(&fsize, 4, 1, fin);

        tname = (char *) malloc(nsize + 1);
        tname[nsize] = '\0';
        fread(tname, 1, nsize, fin);

        fseek(fin, fsize, SEEK_CUR);
        printf("%s ", tname);
        free(tname);
    }
}

void pack(int argc, char *argv[], char *name) {
    char n, nsize;
    int fsize;
    char *tname, *text;
    FILE *fout, *fin;

    fout = fopen(name, "wb");
    n = argc - 4;
    fputc(n, fout);

    for (int i = 4; i < argc; i++) {
        fin = fopen(argv[i], "rb");
        tname = get_file_name(argv[i]);

        nsize = strlen(tname);
        fsize = get_size(fin);
        fputc(nsize, fout);
        fwrite(&fsize, sizeof(int), 1, fout);
        fwrite(tname, sizeof(char), nsize, fout);

        text = (char*) malloc(fsize);
        fread(text, 1, fsize, fin);
        fwrite(text, 1, fsize, fout);
        fclose(fin);
        free(text);
    }
    fclose(fout);
}



int main(int argc, char *argv[]) {
    char filename[100];

    // filename = argv[2]

    if (!strcmp("--create", argv[3])) {
        pack(argc, argv, argv[2]);
    } else if (!strcmp("--extract", argv[3])) {
        get_archive(argv[2]);
    } else if (!strcmp("--list", argv[3])) {
        list(argv[2]);
    } else {
        printf("ERROR: unknown command");
        return -1;
    }
    return 0;
}