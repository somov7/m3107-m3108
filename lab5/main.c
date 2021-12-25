#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <direct.h>

#pragma pack(push, 1)
typedef struct {
    uint16_t  bfType;
    uint32_t  bfSize;
    uint16_t  bfReserved1;
    uint16_t  bfReserved2;
    uint32_t  bfOffBits;
} bitmapFileHeader;

typedef struct {
    uint32_t  biSize;
    int32_t   biWidth;
    int32_t   biHeight;
    uint16_t  biPlanes;
    uint16_t  biBitCount;
    uint32_t  biCompression;
    uint32_t  biSizeImage;
    int32_t   biXPelsPerMeter;
    int32_t   biYPelsPerMeter;
    uint32_t  biClrUsed;
    uint32_t  biClrImportant;
} bitmapInfoHeader;

typedef struct {
    unsigned char    rgbBlue;
    unsigned char    rgbGreen;
    unsigned char    rgbRed;
    unsigned char    rgbReserved;
} RGBQUAD;
#pragma pack(pop)

int countLiveNeighbors(int** field, int row, int column, int height, int width){
    int count = 0;
    for (int i = row - 1; i <= row + 1; ++i) {
        for (int j = column - 1; j <= column + 1; ++j) {
            if ((i == row && j == column) || (i < 0 || j < 0) || (i >= height || j >= width)){
                continue;
            }
            if (field[i][j] == 1){
                ++count;
            }
        }
    }
    return count;
}

void gameOfLife(int** field, int height, int width){
    int neighbors;
    int** nextIterField = (int**)calloc(height, sizeof(int*));
    for(int i = 0; i < height; i++)
        nextIterField[i] = (int*)calloc(width, sizeof(int));
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            neighbors = countLiveNeighbors(field, i, j, height, width);
            if (field[i][j] == 1 && (neighbors == 2 || neighbors == 3)){
                nextIterField[i][j] = 1;
            } else if (field[i][j] == 0 && neighbors == 3){
                nextIterField[i][j] = 1;
            } else {
                nextIterField[i][j] = 0;
            }
        }
    }
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            field[i][j] = nextIterField[i][j];
            int x = 1;
        }
    }
}

int main(int argc, char **argv){
    bitmapFileHeader fileHeader;
    bitmapInfoHeader infoHeader;
    RGBQUAD palette;
    FILE* inputFile;
    char* outputDir;
    int maxIter = 100, dumpFreq = 11;
    bool getInput, getOutput;


    for(int i = 0; i < argc; i += 1){
        if(!strcmp("--input", argv[i])) {
            inputFile = fopen(argv[i + 1], "rb");
            if (inputFile == NULL) {
                fprintf(stderr, "Error opening file");
                return 1;
            }
            getInput = true;
        }
        if(!strcmp("--output", argv[i])){
            outputDir = argv[i + 1];
            if (_mkdir(outputDir) != 0){
                printf ("%s", "Destination directory already exists\n");
            }
            getOutput = true;
        }
        if(!strcmp("--max_iter", argv[i])) {
            maxIter = atoi(argv[i + 1]);
        }
        if(!strcmp("--dump_freq", argv[i])) {
            dumpFreq = atoi(argv[i + 1]);
        }
    }
    if (getInput && getOutput != true){
        fprintf(stderr, "Not enough arguments");
        return 1;
    }
    fread(&fileHeader, 1, 14, inputFile);
    fread(&infoHeader, 1, 40, inputFile);

    fread(&palette, 1, fileHeader.bfOffBits - 54, inputFile);

    unsigned char* imageBytes = (unsigned char*)malloc((fileHeader.bfSize - fileHeader.bfOffBits) * sizeof(unsigned char));
    fread(imageBytes, 1, fileHeader.bfSize, inputFile);

    int** img = (int**)calloc(infoHeader.biHeight, sizeof(int*));
    for(int i = 0; i < infoHeader.biHeight; i++)
        img[i] = (int*)calloc(infoHeader.biWidth, sizeof(int));

    int k = 1;
    for(int i = infoHeader.biHeight - 1; i >= 0; i--){
        for(int j = 0; j < infoHeader.biWidth; j++){
            if(imageBytes[k] == 255)
                img[i][j] = 0;
            else
                img[i][j] = 1;
            k += 3;
        }
        k += infoHeader.biWidth % 4;
    }

    /*for (int i = 0; i < infoHeader.biHeight; i++) {
        for (int j = 0; j < infoHeader.biWidth; j++) {
            //img[i][j] = '.';
            printf(" %d ", img[i][j]);
        }
        printf("\n");
    }
    printf("\n");*/

    char resultName[strlen(outputDir) + 20];
    memset(resultName, 0, sizeof resultName);
    for (int i = 1; i <= maxIter; ++i) {
        gameOfLife(img, infoHeader.biHeight, infoHeader.biWidth);

        /*print array matrix
        printf("Initial Stage:");
        printf("\n");
        for(int k=0; k<infoHeader.biHeight; k++){
            for(int j=0;j<infoHeader.biWidth;j++){
                printf(" %d ",img[k][j]);
            }
            printf("\n");
        }*/

        if (i % dumpFreq == 0){
            char newname[100] = {0};
            sprintf(newname, "%s\\iter%d.bmp", outputDir, i);

            FILE *outputPic;
            outputPic = fopen(newname, "w");

            fwrite(&fileHeader, 1, 14, outputPic);
            fwrite(&infoHeader, 1, 40, outputPic);
            fwrite(&palette, 1, fileHeader.bfOffBits - 54, outputPic);
            int pixel = 0;
            for (int j = infoHeader.biHeight - 1; j >= 0; --j) {
                for (int l = 0; l < infoHeader.biWidth; ++l) {
                    for (k = 0; k < 3; k++) {
                        if (img[j][l] == 1){
                            imageBytes[pixel] = 0;
                        } else {
                            imageBytes[pixel] = 255;
                        }
                        pixel++;
                    }
                }
                while (pixel % 4 != 0){
                    imageBytes[pixel] = 0;
                    pixel++;
                }
            }
            fwrite(imageBytes, 1, fileHeader.bfSize - fileHeader.bfOffBits, outputPic);
            fclose(outputPic);
        }
    }
    free(imageBytes);
    free(img);
    return 0;
}
