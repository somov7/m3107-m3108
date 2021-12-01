#include "bmp.h"

void bmp_to_arr(char *filepath, int **height, int **width, FILE **pImage, struct BITMAPFILEHEADER **bitmapFileHeader, struct BITMAPINFOHEADER **bitmapInfoHeader){
    // открываем файл
    *pImage = fopen(filepath, "rb");
    if (*pImage == NULL){
        printf("file == null");
        exit(1);
    }
    // читаем fileHeader
    fread(&bitmapFileHeader, sizeof(),1)
}