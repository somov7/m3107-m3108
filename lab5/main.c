#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "/Library/Frameworks/SDL2.framework/Headers/SDL.h"

#pragma pack(push, 1)
typedef struct {
    uint16_t type;
    uint32_t fileSize;
    uint32_t reserved;
    uint32_t offsetBits;
} FileHeader;

typedef struct {
    uint32_t headerSize;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bitCount;
    uint32_t compression;
    uint32_t imageSize;
    uint32_t horizontalResolution;
    uint32_t verticalResolution;
    uint32_t colorsCount;
    uint32_t mainColorsCount;
    uint8_t colorsTable[2][4];
} InfoHeader;
#pragma pack(pop)

static FileHeader fileHeader;
static InfoHeader infoHeader;
char ALIVE = 1;
char DEAD = 0;

enum ErrorTypes {
    FILE_NOT_FOUND = 1,
    CANT_CREATE_FILE,
    INPUT_NOT_FOUND,
    OUTPUT_NOT_FOUND,
    WRONG_FILE_FORMAT,
    WRONG_HEADER_SIZE,
    NOT_MONOCHROME_BMP,
    WRONG_ARGUMENTS_COUNT,
    WRONG_MAX_ITERATION,
    WRONG_FREQUENCY,
};

void error(enum ErrorTypes error) {
    switch (error) {
        case FILE_NOT_FOUND:
            printf("Error: File not found\nCheck the correctness of filepath\n");
            break;
        case CANT_CREATE_FILE:
            printf("Error: Can't create file\nCheck the correctness of output directory\n");
            break;
        case INPUT_NOT_FOUND:
            printf("Error: Argument of '--input' not found\n");
            break;
        case OUTPUT_NOT_FOUND:
            printf("Error: Argument of '--output' not found\n");
            break;
        case WRONG_FILE_FORMAT:
            printf("Error: File should be with .bmp format\n");
            break;
        case WRONG_HEADER_SIZE:
            printf("Error: Size of file's header is incorrect\n");
            break;
        case NOT_MONOCHROME_BMP:
            printf("Error: Bitmap file should be monochrome\n");
            break;
        case WRONG_ARGUMENTS_COUNT:
            printf("Error: Wrong arguments count\n");
            break;
        case WRONG_MAX_ITERATION:
            printf("Error: Argument of '--max_iter' should be positive\n");
            break;
        case WRONG_FREQUENCY:
            printf("Error: Argument of '--dump_freq' should be positive\n");
            break;
    }
    printf("Correct usage: --input <filepath> --output <directory path> --max_iter N (optional) --dump_freq N (optional)\n");
    exit(error);
}

void checkArgumentsCount(int argsCount) {
    if (argsCount < 5)
        error(WRONG_ARGUMENTS_COUNT);
}

FILE *getFile(char *filename, char *mode) {
    FILE *file = fopen(filename, mode);
    if (file == NULL && !strcmp(mode, "rb"))
        error(FILE_NOT_FOUND);
    if (file == NULL && !strcmp(mode, "wb"))
        error(CANT_CREATE_FILE);
    unsigned long stringLength = strlen(filename);
    filename += stringLength - 3;
    if (strcmp(filename, "bmp") != 0)
        error(WRONG_FILE_FORMAT);
    return file;
}

FILE *getFileFromArgs(int argumentsCount, char **arguments) {
    char *filename = NULL;
    for (int i = 1; i < argumentsCount; ++i) {
        if (strcmp(arguments[i], "--input") == 0 && i < argumentsCount - 1) {
            filename = arguments[i + 1];
            break;
        }
    }
    if (filename == NULL)
        error(INPUT_NOT_FOUND);
    return getFile(filename, "rb");
}

char *getDirectory(int argumentsCount, char **arguments) {
    char *directory = NULL;
    for (int i = 1; i < argumentsCount; ++i) {
        if (!strcmp(arguments[i], "--output") && i < argumentsCount - 1) {
            directory = arguments[i + 1];
            break;
        }
    }
    if (directory == NULL)
        error(OUTPUT_NOT_FOUND);
    return directory;
}

int getMaxIteration(int argumentsCount, char **arguments) {
    int maxIteration = -1;
    char isChanged = 0;
    for (int i = 1; i < argumentsCount; ++i) {
        if (!strcmp(arguments[i], "--max_iter") && i < argumentsCount - 1) {
            maxIteration = atoi(arguments[i + 1]);
            isChanged = 1;
            break;
        }
    }
    if (maxIteration <= 0 && isChanged)
        error(WRONG_MAX_ITERATION);
    return maxIteration;
}

int getFrequency(int argumentsCount, char **arguments) {
    int frequency = 1;
    char isChanged = 0;
    for (int i = 1; i < argumentsCount; ++i) {
        if (!strcmp(arguments[i], "--dump_freq") && i < argumentsCount - 1) {
            frequency = atoi(arguments[i + 1]);
            isChanged = 1;
            break;
        }
    }
    if (frequency <= 0 && isChanged)
        error(WRONG_FREQUENCY);
    return frequency;
}

char isCellAlive(const char *pixelsArray, int row, int column) {
    if (row >= infoHeader.height || column >= infoHeader.width || row < 0 || column < 0)
        return 0;
    if (pixelsArray[row * infoHeader.width + column] == ALIVE)
        return 1;
    return 0;
}

int getNeighborsCount(char *pixelsArray, int row, int column) {
    int neighborsCount = 0;
    neighborsCount += isCellAlive(pixelsArray, row + 1, column);
    neighborsCount += isCellAlive(pixelsArray, row + 1, column - 1);
    neighborsCount += isCellAlive(pixelsArray, row + 1, column + 1);
    neighborsCount += isCellAlive(pixelsArray, row - 1, column);
    neighborsCount += isCellAlive(pixelsArray, row - 1, column - 1);
    neighborsCount += isCellAlive(pixelsArray, row - 1, column + 1);
    neighborsCount += isCellAlive(pixelsArray, row, column - 1);
    neighborsCount += isCellAlive(pixelsArray, row, column + 1);
    return neighborsCount;
}

void gameStep(char *pixelsArray) {
    char tempArray[infoHeader.height * infoHeader.width + 4];
    for (int i = 0; i < infoHeader.height; ++i) {
        for (int j = 0; j < infoHeader.width; ++j) {
            uint32_t pixelIndex = i * infoHeader.width + j;
            int neighborsCount = getNeighborsCount(pixelsArray, i, j);
            tempArray[pixelIndex] = pixelsArray[pixelIndex];
            if (neighborsCount == 3 && pixelsArray[pixelIndex] == DEAD)
                tempArray[pixelIndex] = ALIVE;
            else if ((neighborsCount < 2 || neighborsCount > 3) && pixelsArray[pixelIndex] == ALIVE)
                tempArray[pixelIndex] = DEAD;
        }
    }
    for (int i = 0; i < infoHeader.height; ++i) {
        for (int j = 0; j < infoHeader.width; ++j) {
            uint32_t pixelIndex = i * infoHeader.width + j;
            pixelsArray[pixelIndex] = tempArray[pixelIndex];
        }
    }
}

void showBMP(char *filepath, SDL_Surface *image, SDL_Texture *texture, SDL_Renderer *render) {
    SDL_FreeSurface(image);
    image = SDL_LoadBMP(filepath);
    texture = SDL_CreateTextureFromSurface(render, image);
    SDL_RenderCopy(render, texture, NULL, NULL);
    SDL_RenderPresent(render);
}

void saveStepInBMP(char *pixelsArray, char *directory, int iteration, SDL_Surface *image, SDL_Texture *texture, SDL_Renderer *render) {
    char filepath[strlen(directory) + 10];
    sprintf(filepath, "%s/step%d.bmp", directory, iteration);
    FILE *output = getFile(filepath, "wb");

    fwrite(&fileHeader, sizeof(fileHeader), 1, output);
    fwrite(&infoHeader, sizeof(infoHeader), 1, output);

    uint32_t lineSize = ((infoHeader.width + 31) / 32) * 4;
    uint8_t bitmapData[lineSize * infoHeader.height];

    uint8_t currentByte = 0;
    int currentPosition = 0;
    for (int i = infoHeader.height - 1; i >= 0; --i) {
        for (int j = 0; j < infoHeader.width; ++j) {
            uint32_t pixelIIndex = i * infoHeader.width + j;
            uint8_t currentBit = 0x80 >> j % 8;
            if (pixelsArray[pixelIIndex] == 1)
                currentByte = currentByte | currentBit;
            if ((j + 1) % 8 == 0 || j == infoHeader.width - 1) {
                bitmapData[currentPosition++] = currentByte;
                currentByte = 0;
            }
        }
        while (currentPosition % 4 != 0)
            bitmapData[currentPosition++] = 0;
    }

    fwrite(bitmapData, 1, lineSize * infoHeader.height, output);

    fclose(output);
    showBMP(filepath, image, texture, render);
}

void checkHeader() {
    if (infoHeader.headerSize != 40)
        error(WRONG_HEADER_SIZE);
    if (infoHeader.bitCount != 1)
        error(NOT_MONOCHROME_BMP);
    if (infoHeader.colorsTable[0][0] == 0) {
        ALIVE = 0;
        DEAD = 1;
    }
}

char *getPixelsArray(FILE *file) {
    fread(&fileHeader, sizeof(fileHeader), 1, file);
    if (fileHeader.type != 19778) // 19778 - "BM" in int
        error(WRONG_FILE_FORMAT);

    fread(&infoHeader, sizeof(infoHeader), 1, file);
    fseek(file, fileHeader.offsetBits, SEEK_SET);
    checkHeader();

    uint32_t lineSize = ((infoHeader.width + 31) / 32) * 4;
    uint8_t bitmapData[lineSize * infoHeader.height];
    fread(bitmapData, lineSize * infoHeader.height, 1, file);

    char *pixelsArray = (char *) malloc(infoHeader.height * infoHeader.width);
    if (!pixelsArray)
        error(WRONG_FILE_FORMAT);

    for (int y = infoHeader.height - 1; y >= 0; y--) {
        for (int x = 0; x < infoHeader.width; x++) {
            uint32_t currentByte = x / 8;
            unsigned char pixel = bitmapData[y * lineSize + currentByte];
            unsigned char currentBit = 0x80 >> x % 8;
            if (pixel & currentBit)
                pixelsArray[(infoHeader.height - (y + 1)) * infoHeader.width + x] = 1;
            else
                pixelsArray[(infoHeader.height - (y + 1)) * infoHeader.width + x] = 0;
        }
    }

    fclose(file);
    return pixelsArray;
}

char isGameFinished(int iterationCount, int maxIteration) {
    if ((iterationCount < maxIteration) || maxIteration < 0)
        return 0;
    return 1;
}

void solve(FILE *file, char *directory, int maxIteration, int frequency) {
    int iterationCount = 0;
    char *pixelsArray = getPixelsArray(file);

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 10 * infoHeader.width,
                                          10 * infoHeader.height, SDL_WINDOW_RESIZABLE);
    SDL_Renderer *render = SDL_CreateRenderer(window, -1, 0);
    SDL_Surface *image = NULL;
    SDL_Texture *texture = NULL;

    SDL_Event e;
    char quit = 0;
    while (!quit){
        if (!isGameFinished(iterationCount, maxIteration))
            gameStep(pixelsArray);
        if ((iterationCount + 1) % frequency == 0 && !isGameFinished(iterationCount, maxIteration))
            saveStepInBMP(pixelsArray, directory, (iterationCount / frequency) + 1, image, texture, render);
        iterationCount++;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT || e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN)
                quit = 1;
        }
    }

    free(pixelsArray);
    SDL_FreeSurface(image);
    SDL_DestroyRenderer(render);
    SDL_DestroyTexture(texture);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char *argv[]) {
    checkArgumentsCount(argc);
    solve(getFileFromArgs(argc, argv), getDirectory(argc, argv), getMaxIteration(argc, argv), getFrequency(argc, argv));
    return 0;
}

// command for compile
// gcc main.c -o main -I/Library/Frameworks/SDL2.framework/Headers -F/Library/Frameworks -framework SDL2
