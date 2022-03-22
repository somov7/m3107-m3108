#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dir.h>

#pragma pack(1)

unsigned int convertEndians(unsigned int val) {
    return ((val >> 24) & 0x000000FF) |
           ((val >> 8) & 0x0000FF00) |
           ((val << 8) & 0x00FF0000) |
           ((val << 24) & 0xFF000000);
}

int getDigits(int val) {
    int digits = 0;
    if (val == 0) {
        return 1;
    }
    while (val > 0) {
        digits++;
        val /= 10;
    }
    return digits;
}

void parseArgs(int argc, char *argv[], char **inputFileName, char **outputDirName, int *maxIter, int *dumpFreq) {

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--input")) {
            *inputFileName = argv[++i];
        } else if (!strcmp(argv[i], "--output")) {
            *outputDirName = argv[++i];
        } else if (!strcmp(argv[i], "--max_iter")) {
            i++;
            *maxIter = atol(argv[i]);
        } else if (!strcmp(argv[i], "--dump_freq")) {
            i++;
            *dumpFreq = atol(argv[i]);
        } else {
            printf("Please use commands like: --input fileName.bmp --output directionName/ --max_iter 100 --dump_freq 1.");
            exit(-1);
        }
    }

    if (*inputFileName == NULL || *outputDirName == NULL) {
        printf("Please write name of input_file or output_dir.");
        exit(-1);
    }
}

typedef union bmpHeader {
    unsigned char buffer[62];

    struct {
        // BITMAPFILEHEADER
        unsigned short id;
        unsigned int size;
        unsigned int reserved;
        unsigned int bfOffBits;
        // BITMAPINFO
        int biSize;
        int biWidth;
        int biHeight;
        short biPlanes;
        short biBitCount;
        int biCompression;
        int biSizeImage;
        int biXPelsPerMeter;
        int biYPelsPerMeter;
        int biClrUsed;
        int biClrImportant;
        unsigned char cl1_rgbBlue;
        unsigned char cl1_rgbGreen;
        unsigned char cl1_rgbRed;
        unsigned char cl1_rgbReserved;
        unsigned char cl2_rgbBlue;
        unsigned char cl2_rgbGreen;
        unsigned char cl2_rgbRed;
        unsigned char cl2_rgbReserved;
    } data;
} bmpHeader;

int **mapAlloc(int width, int height) {
    int i;
    int **map = (int **) malloc(height * sizeof(int *));

    for (i = 0; i < height; i++) {
        map[i] = (int *) malloc(width * sizeof(int));
    }

    return map;
}

void mapFree(int **map, int height) {
    int y;
    for (y = 0; y < height; y++) {
        free(map[y]);
    }
    free(map);
}

void mapCopy(int **from, int **to, int width, int height) {
    int x, y;
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            to[y][x] = from[y][x];
        }
    }
    mapFree(from, height);
}

void dumpMap(int **map, union bmpHeader bmpHeader, char *outputDirName, int iteration) {
    FILE *outputFile;
    mkdir(outputDirName);

    int outputFileNameLength = strlen(outputDirName) + 4 + getDigits(iteration);
    char *outputFileName = calloc(outputFileNameLength + 1, sizeof(char));

    strncpy(outputFileName, outputDirName, strlen(outputDirName));
    itoa(iteration, outputFileName + strlen(outputDirName), 10);
    strcat(outputFileName, ".bmp");

    outputFile = fopen(outputFileName, "wb");

    if (outputFile == NULL) {
        printf("Problem with opening outputFile.");
        exit(-1);
    }

    fwrite(&bmpHeader, sizeof(char), 62, outputFile);


    int width = bmpHeader.data.biWidth;
    int height = bmpHeader.data.biHeight;

    int x, y, j, buffer;

    for (y = 0; y < height; y++) {
        buffer = 0;
        for (x = 0; x < width / 32; x++) {
            buffer = 0;
            for (j = 0; j < 32; j++) {
                buffer |= !map[y][x * 32 + j] << (32 - j - 1);
            }
            buffer = convertEndians(buffer);
            fwrite(&buffer, sizeof(int), 1, outputFile);
        }
        buffer = 0;
        for (x = width / 32 * 32; x < width; x++) {
            buffer |= !map[y][x] << (32 - x % 32 - 1);
        }
        buffer = convertEndians(buffer);
        fwrite(&buffer, sizeof(int), 1, outputFile);
    }

    fclose(outputFile);
}

int **getMap(char *inputFileName, int *width, int *height, union bmpHeader *bmpHeader) {
    FILE *inputFile;
    if ((inputFile = fopen(inputFileName, "rb")) == NULL) {
        printf("File is not exist.");
        exit(-1);
    }
    if (!fread(bmpHeader->buffer, sizeof(char), 62, inputFile)) {
        printf("Problem with reading file.");
        exit(-1);
    }

    if (bmpHeader->data.biSize != 40 || bmpHeader->data.biClrUsed != 0 || bmpHeader->data.biBitCount != 1) {
        printf("It is not a monochrome .bmp file.");
        exit(-1);
    }

    *width = bmpHeader->data.biWidth;
    *height = bmpHeader->data.biHeight;

    int **map = mapAlloc(*width, *height);

    int i, x, y, buffer;


    for (y = 0; y < *height; y++) {
        i = 0;
        for (x = 0; x < *width; x++) {
            if (i % 32 == 0) {
                i = 0;
                fread(&buffer, sizeof(int), 1, inputFile);
                buffer = convertEndians(buffer);
            }
            map[y][x] = !((buffer >> (32 - i - 1)) & 1);
            i++;
        }
    }

    fclose(inputFile);
    return map;
}

int **getNextMapGeneration(int **map, int **mapAlt, int width, int height) {
    int neighbours, x, y;
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            neighbours = 0;
            if (map[(y + height + 1) % height][(x + width - 1) % width]) neighbours++;
            if (map[(y + height + 1) % height][(x + width) % width]) neighbours++;
            if (map[(y + height + 1) % height][(x + width + 1) % width]) neighbours++;

            if (map[(y + height) % height][(x + width - 1) % width]) neighbours++;
            if (map[(y + height) % height][(x + width + 1) % width]) neighbours++;

            if (map[(y + height - 1) % height][(x + width - 1) % width]) neighbours++;
            if (map[(y + height - 1) % height][(x + width) % width]) neighbours++;
            if (map[(y + height - 1) % height][(x + width + 1) % width]) neighbours++;

            mapAlt[y][x] = (neighbours == 2 && map[y][x]) || (neighbours == 3);
        }
    }
    return mapAlt;
}

void StartGame(int argc, char *argv[]) {
    char *inputFileName = NULL;
    char *outputDirName = NULL;
    int maxIter = -1;
    int dumpFreq = 1;
    int width, height;

    parseArgs(argc, argv, &inputFileName, &outputDirName, &maxIter, &dumpFreq);

    bmpHeader bmpHeader;

    int **map = getMap(inputFileName, &width, &height, &bmpHeader);

    char isPlaying = 1;
    int i, dumps = 1;
    dumpMap(map, bmpHeader, outputDirName, 0);

    while (isPlaying) {
        int **mapAlt = mapAlloc(width, height);
        mapAlt = getNextMapGeneration(map, mapAlt,
                                      width, height);

        mapCopy(mapAlt, map, width, height);

        if (i % dumpFreq == 0) {
            dumpMap(map, bmpHeader, outputDirName, dumps++);
        }
        if (maxIter != -1) {
            maxIter--;
            isPlaying = maxIter > 0 ? 1 : 0;
        }
        i++;
    }

    mapFree(map, height);
    printf("Game finished.");
}

int main(int argc, char *argv[]) {
    StartGame(argc, argv);
    return 0;
}