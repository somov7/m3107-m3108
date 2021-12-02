#include "utils.h"

int main(int argc, char* argv[]) {
    char* dirname;
    pixel** pixels;
    int max_iter = 10, dump_freq = 1;
    FILE* fin, *fout;
    bitmap_str bitmap;
    header_str header;
    int **old_gen;

    if (argv[1] == NULL || argv[2] == NULL || argv[3] == NULL || argv[4] == NULL) {
        printf("Not enough arguments\n");
        return 1;
    }

    if (strcmp(argv[1],"--input") || strcmp(argv[3],"--output")) {
        printf("Wrong format\n");
        return 1;
    }

    fin = fopen(argv[2], "rb");
    if (!fin) {
        printf("Cannot open this file\n");
        return 1;
    }

    dirname = argv[4];

    if (argv[5] != NULL) {
        if (!(strcmp(argv[5], "--max_iter")))
            max_iter = atoi(argv[6]);
        else if (!(strcmp(argv[5], "--dump_freq")))
            dump_freq = atoi(argv[6]);
        if (argv[7] != NULL) {
            if (!(strcmp(argv[7], "--max_iter")))
                max_iter = atoi(argv[8]);
            else if (!(strcmp(argv[7], "--dump_freq")))
                dump_freq = atoi(argv[8]);
        }
    }

    //printf("dirname: %s, max iter: %d, dump freq: %d\n", dirname, max_iter, dump_freq);

    /******* READ THE FILE *******/

    fread(&header, 1, 54,  fin);
    bitmap.height = bytes_to_int(header.height);
    bitmap.width = bytes_to_int(header.width);
    //printf("h: %ld, w: %ld, s: %ld\n", bitmap.height, bitmap.width, bitmap.height* bitmap.width);

    pixels = (pixel**)malloc(bitmap.height * sizeof(pixel*));
    for (int i = 0; i < bitmap.height; i++)
        pixels[i] = (pixel*)malloc(bitmap.width * sizeof(pixel));
    for (int i = bitmap.height - 1; i >= 0; i--)
        for (int j = 0; j < bitmap.width; j++) {
            pixels[i][j].B = getc(fin);
            pixels[i][j].G = getc(fin);
            pixels[i][j].R = getc(fin);
        }

    /*** CREATE OLD GENERATION ***/

    old_gen = (int **)malloc(bitmap.height * sizeof(int*));
    for (int i = 0; i < bitmap.height; i++) {
        old_gen[i] = (int *) malloc(bitmap.width * sizeof(int));
    }

    for (int i = 0; i < bitmap.height; i++)
        for (int j = 0; j < bitmap.width; j++) {
            if ((alive(pixels[i][j])) == 1)
                old_gen[i][j] = 1;
            else
                old_gen[i][j] = 0;
        }

    /********* GAME LIFE *********/

    for (int i = 1; i <= max_iter; i++) {
        char path[256];
        char filename[32];
        unsigned char* new_gen;

        game_life(old_gen, bitmap.width, bitmap.height);

        if (i % dump_freq != 0)
            continue;

        strcpy(path, dirname);
        strcat(path, "/");
        strcat(path, _itoa(i, filename, 10));
        strcat(path, ".bmp");

        fout = fopen(path, "wb");
        if (!fout) {
            printf("cannot create file\n");
            return 1;
        }
        fwrite(&header, 1, 54, fout);
        int n = 0;

        new_gen = (unsigned char*)malloc(bitmap.width * bitmap.height * 3);

        for (int m = bitmap.height - 1; m >= 0; m--)
            for (int j = 0; j < bitmap.width; j++)
                for (int k = 0; k < 3; k++) {
                    if (old_gen[m][j] == 1)
                        new_gen[n] = 0;
                    else
                        new_gen[n] = 255;
                    n++;
                }
        fwrite(new_gen, 1, bitmap.width * bitmap.height * 3, fout);
        fclose(fout);
        free(new_gen);
    }

    for (int i = 0; i < bitmap.height; i++)
        free(old_gen[i]);
    free(old_gen);

    for (int i = 0; i < bitmap.height; i++)
        free(pixels[i]);
    free(pixels);

    fclose(fin);
}