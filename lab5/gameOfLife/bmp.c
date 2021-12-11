
#include "bmp.h"
#include "game.h"

int **bmp_to_arr(char *filepath, int *height, int *width, FILE **pImage, char **offset, struct BITMAPFILEHEADER *bitmapFileHeader,
                 struct BITMAPINFOHEADER *bitmapInfoHeader) {
    // открываем файл
    *pImage = fopen(filepath, "rb");
    if (*pImage == NULL) {
        printf("file == null");
        exit(1);
    }

    fread(bitmapFileHeader, 1, sizeof(BitMapFileHeader), *pImage);
    fread(bitmapInfoHeader, 1, sizeof(BitMapInfoHeader), *pImage);
    *offset = calloc((int)bitmapFileHeader->OffsetBits, sizeof(char));
    fseek(*pImage, 0, SEEK_SET);
    fread(*offset, 1, bitmapFileHeader->OffsetBits, *pImage);
    *height = (int)bitmapInfoHeader->height;
    *width = (int)bitmapInfoHeader->width;
    int **arr = mallocForArr(*height, *width);
    fseek(*pImage, (int)bitmapFileHeader->OffsetBits, SEEK_SET);

    int x = 0;
    if (*width % 8 > 0) x++;
    int realWidth = (*width / 8) + x;
    int scanLineSize = realWidth;
    if (realWidth % 4 != 0){
        scanLineSize = realWidth + (4 - (realWidth % 4));
    }
    char* scanLine = calloc(scanLineSize, sizeof(char));

    for (int i = *height - 1; i >= 0; i--){
        fread(scanLine, 1, scanLineSize, *pImage);
        for (int j = 0; j < *width; j++){
            int curBytePos = j / 8;
            int curBit = 1 << (7 - j % 8);
            int pixel = scanLine[curBytePos] & curBit;
            if (pixel == 0){
                arr[i][j] = LIVE_CELL;
            }
            else{
                arr[i][j] = DEAD_inside_CELL;
            }
        }
    }
    free(scanLine);

    return arr;
}

void
arr_to_bmp(int **arr, int height, int width, int iter_num, char *dir_name, char *offset, struct BITMAPFILEHEADER *bitmapFileHeader,
           FILE **pImage) {
    char *str = calloc(10,sizeof(char));
    itoa(iter_num, str, 10);
    char *path = calloc(1000, sizeof(char));
    strcat(path, dir_name);
    strcat(path, "/generation_num_");
    strcat(path, str);
    strcat(path,".bmp");

    FILE *fout = fopen(path, "wb");
    if (fout == NULL) {
        printf("file == null");
        exit(1);
    }

    fwrite(offset, 1, (int)bitmapFileHeader->OffsetBits, fout);

    int x = 0;
    if (width % 8 > 0) x++;
    int realWidth = (width / 8) + x;
    int scanLineSize = realWidth;
    if (realWidth % 4 != 0){
        scanLineSize = realWidth + (4 - (realWidth % 4));
    }

    for (int i = height - 1; i >= 0; i--){
        char *scanLine = calloc(scanLineSize, sizeof(char));
        for (int j = 0; j < width; j++){
            int curBytePosition = j / 8;
            if (arr[i][j] == 0){
                scanLine[curBytePosition] = (char)((1 << (7 - j % 8)) bitor scanLine[curBytePosition]);
            }
        }
        fwrite(scanLine, 1, scanLineSize, fout);
        free(scanLine);
    }

    fclose(fout);
    free(path);
    free(str);
}
