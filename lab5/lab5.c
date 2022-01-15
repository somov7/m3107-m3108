#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#pragma pack(push, 1)

typedef struct {
    uint16_t type;
    uint32_t hsize;
    uint16_t res1;
    uint16_t res2;
    uint32_t offset;
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bits;
    uint32_t compression;
    uint32_t imgsize;
    int32_t xppm;
    int32_t yppm;
    uint32_t colors1;
    uint32_t colors2;
} File;

#pragma pack(pop)


int bounds(int x, int y, File file) {
    if (x < 0 || x >= file.width || y < 0 || y >= file.height)
        return 0;
    else {
        return 1;
    }
}

int next(int **table, int x, int y, File file) {
    int _x[8] = {1, 1, 1, 0, 0, -1, -1, -1};
    int _y[8] = {0, 1, -1, 1, -1, -1, 0, 1};
    int n = 0;
    for (int p = 0; p < 8; ++p) {
        if (bounds(x + _x[p], y + _y[p], file)) {
            int cell = table[x + _x[p]][y + _y[p]];
            n += cell;
        }
    }
    return n;
}

int **generation(int **table, File file) {
    int** table2;
    table2 = (int**)calloc(file.width, sizeof(int*));
    for (int i = 0; i < file.width; i++)
        table2[i] = (int*)calloc(file.height, sizeof(int));

    for (int j = 0; j < file.height; ++j)
        for (int i = 0; i < file.width; ++i) {
            int n = next(table, i, j, file);
            int cell = table[i][j];
            if (n == 3 || cell && n == 2) 
                table2[i][j] = 1;
            else 
                table2[i][j] = 0;
        }
    return table2;
}


int **Pixels(FILE* prev, File next) {
    int width = next.width;
    int height = next.height;
    int length = 0;
    char* line = 0;
    int** table = 0;

    table = (int **)calloc(width, sizeof(int *));
    for (int i = 0; i < width; i++)
        table[i] = (int *)calloc(height, sizeof(int));

    int byteWidth = ceil((double)width / 8);
    if (byteWidth % 4 != 0)
        length = byteWidth + (4 - (byteWidth % 4));
    else
        length = byteWidth;
    line = (char *)calloc(length, sizeof(char));

    for (int j = height - 1; j >= 0; j--) {
        fread(line, 1, length, prev);
        for (int i = 0; i < width; i++) {
            if (line[i / 8] & (1 << (7 - i % 8)))
                table[i][j] = 0;
            else
                table[i][j] = 1;
        }
    }
    return table;
}

void Bmp(int **table, File prev, int gen, char *dirname, char *buff) {
    char* path = (char *)calloc(100, sizeof(char));
    char* name = (char*)calloc(30, sizeof(char));
    FILE* next;
    int width = prev.width, height = prev.height, length;

    strcat(path, dirname);
    strcat(path, "/");
    strcat(path, itoa(gen, name, 10));
    strcat(path, ".bmp");
    next = fopen(path, "wb");

    fwrite(buff, 1, prev.offset, next);
    int bwitdth = ceil((double) width / 8);
    if (bwitdth % 4 != 0)
        length = bwitdth + (4 - (bwitdth % 4));
    else
        length = bwitdth;

    for (int j = height - 1; j >= 0; j--) {
        char* line = (char*)calloc(length, sizeof(char));
        for (int i = 0; i < width; i++) {
            if (table[i][j] == 0) 
                line[i/8] = (char)(1 << (7 - i % 8)) | line[i/8];
        }
        fwrite(line, 1, length, next);
    }
    fclose(next);
}


int main(int argc, char* argv[]) {
    char filename[100], dirname[100];
    char* buff;
    int iter;
    int freq = 1;
    int **table;
    FILE *f;
    File bmp;

    if (argc < 5) {
        printf("not enough args\n");
        return 1;
    }

    for (int i = 1; i < argc; i += 2) {
        if (!strcmp("--input", argv[i]))
            strcpy(filename, argv[i + 1]);
        else if (!strcmp("--output", argv[i]))
            strcpy(dirname, argv[i + 1]);
        else if (!strcmp("--max_iter", argv[i])) {
            iter = atoi(argv[i + 1]);
        } else if (!strcmp("--dump_freq", argv[i])) {
            iter = atoi(argv[i + 1]);
        }
    } 

    f = fopen(filename, "rb");
    fread(&bmp, 1, sizeof(File), f);
    buff = (char *) calloc(sizeof(char), bmp.offset);
    fseek(f, 0, SEEK_SET);
    fread(buff, 1, bmp.offset, f);
    table = Pixels(f, bmp);

    for (int i = 0; i < iter; i++)
        if (i % freq == 0) {
            table = generation(table, bmp);
            Bmp(table, bmp, i, dirname, buff);
        }
    return 0;
}