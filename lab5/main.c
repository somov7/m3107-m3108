#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEAD 0
#define ALIVE 1

typedef struct {
    char smth[18];
    char width[4];
    char height[4];
    char other[28];
} HeaderBMP;

typedef struct {
    int height, width;
} Bitmap;

typedef struct {
    int R, G, B;
} Pixel;

int bytes_to_int(unsigned char *ch) {
    return (int) (ch[3] << 24 | ch[2] << 16 | ch[1] << 8 | ch[0]);
}

int map_pixel_to_cell(Pixel pixel) {
    if (pixel.B == 0 && pixel.R == 0 && pixel.G == 0)
        return ALIVE;
    return DEAD;
}

int is_neighbour_alive(int x, int y, int w, int h, int **field) {
    x = (x + w) % w;
    y = (y + h) % h;
    return field[y][x];
}

int get_number_of_neighbours(int x, int y, int **field, int w, int h) {
    int count = 0;

    for(int i = x - 1; i <= x + 1; i++){
        for(int j = y - 1; j <= y + 1; j++){
            if(i == x && j == y) 
                continue;
            count += is_neighbour_alive(i, j, w, h, field);
        }
    }

    return count;
}

void next_game_iteration(int **field, Bitmap bitmap) {
    int neighCount;

    int **next_field = (int **)malloc(sizeof(int*) * bitmap.height);
    for (int i = 0; i < bitmap.height; i++)
        next_field[i] = (int*)malloc(bitmap.width * sizeof(int));

    for (int y = 0; y < bitmap.height; y++)
        for (int x = 0; x < bitmap.width; x++) {
            neighCount = get_number_of_neighbours(x, y, field, bitmap.width, bitmap.height);
            if (field[y][x] == ALIVE) {
                if (neighCount < 2 || neighCount > 3)
                    next_field[y][x] = DEAD;
                else
                    next_field[y][x] = ALIVE;
            }
            else {
                if (neighCount == 3)
                    next_field[y][x] = ALIVE;
                else
                    next_field[y][x] = DEAD;
            }
        }
    
    for (int y = 0; y < bitmap.height; y++)
        for (int x = 0; x < bitmap.width; x++)
            field[y][x] = next_field[y][x];

    for (int i = 0; i < bitmap.height; i++)
        free(next_field[i]);
    free(next_field);
}

FILE* getFilePath(char* dirname, int i) {
    char path[256];
    char filename[32];

    sprintf(filename, "%ld", i);
    strcpy(path, dirname);
    strcat(path, "/");
    strcat(path, filename);
    strcat(path, ".bmp");

    return fopen(path, "wb");
}

int check_on_error_in_arguments(char* argv[]) {
    if (argv[1] == NULL || argv[2] == NULL || argv[3] == NULL || argv[4] == NULL) {
        printf("Not enough arguments\n");
        return 1;
    }
    if (strcmp(argv[1],"--input") || strcmp(argv[3],"--output")) {
        printf("Wrong format\n");
        return 1;
    }
    return 0;
}

char* read_dirname(char* argv[]) {
    return argv[4];
}

int read_maxIter_or_dumpFreq(char* argv[], char* parameterName, int defaultValue) {
    int value = defaultValue;
    if (argv[5] != NULL && !(strcmp(argv[5], parameterName))) {
        value = atoi(argv[6]);
    }
    if (argv[7] != NULL && !(strcmp(argv[7], parameterName))) {
        value = atoi(argv[8]);
    }
    return value;
}

int** read_field(FILE* fin, Bitmap bitmap) {
    Pixel** pixels = (Pixel**)malloc(bitmap.height * sizeof(Pixel*));
    for (int i = 0; i < bitmap.height; i++)
        pixels[i] = (Pixel*)malloc(bitmap.width * sizeof(Pixel));
    for (int i = bitmap.height - 1; i >= 0; i--)
        for (int j = 0; j < bitmap.width; j++) {
            pixels[i][j].B = getc(fin);
            pixels[i][j].G = getc(fin);
            pixels[i][j].R = getc(fin);
        }

    int **field = (int **) malloc(bitmap.height * sizeof(int*));
    for (int i = 0; i < bitmap.height; i++) {
        field[i] = (int *) malloc(bitmap.width * sizeof(int));
    }

    for (int i = 0; i < bitmap.height; i++)
        for (int j = 0; j < bitmap.width; j++) {
            field[i][j] = map_pixel_to_cell(pixels[i][j]);
        }

    for (int i = 0; i < bitmap.height; i++)
        free(pixels[i]);
    free(pixels);
    fclose(fin);

    return field;
}

void write_field_to_file(int** field, FILE* fout, Bitmap bitmap, HeaderBMP header) {
    fwrite(&header, 1, 54, fout);

    int n = 0;
    unsigned char* newPixels = (unsigned char*)malloc(bitmap.width * bitmap.height * 3);

    for (int m = bitmap.height - 1; m >= 0; m--)
        for (int j = 0; j < bitmap.width; j++)
            for (int k = 0; k < 3; k++) {
                if (field[m][j] == ALIVE)
                    newPixels[n] = 0;
                else
                    newPixels[n] = 255;
                n++;
            }
    
    fwrite(newPixels, 1, bitmap.width * bitmap.height * 3, fout);
    free(newPixels);
}

int main(int argc, char* argv[]) {
    if (check_on_error_in_arguments(argv)) {
        return 1;
    }

    FILE* fin = fopen(argv[2], "rb");
    if (!fin) {
        printf("Cannot open this file\n");
        return 1;
    }

    char* dirname = read_dirname(argv);
    int maxIter = read_maxIter_or_dumpFreq(argv, "--max_iter", 10);
    int dumpFreq = read_maxIter_or_dumpFreq(argv, "--dump_freq", 1);

    HeaderBMP header;
    fread(&header, 1, 54,  fin);
    Bitmap bitmap;
    bitmap.height = bytes_to_int(header.height);
    bitmap.width = bytes_to_int(header.width);  
    int** field = read_field(fin, bitmap);

    for (int i = 1; i <= maxIter; i++) {
        next_game_iteration(field, bitmap);

        if (i % dumpFreq != 0)
            continue;

        FILE* fout = getFilePath(dirname, i);
        if (!fout) {
            printf("cannot create file\n");
            return 1;
        }
       
        write_field_to_file(field, fout, bitmap, header);
        fclose(fout);
    }

    for (int i = 0; i < bitmap.height; i++)
        free(field[i]);
    free(field);

    return 0;
}