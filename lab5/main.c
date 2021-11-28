#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <iso646.h>

#pragma pack(push, 1)
typedef struct stBitMapFile {

    uint16_t bfType;            //Метка BM
    uint32_t bfSize;            //Длина в байтах
    uint16_t bfRes1;            //Резерв1
    uint16_t bfRes2;           //Резерв2
    uint32_t bfOffs;            //Смещение области данных

    //Начало BITMAP_INFO:
    uint32_t biSize;            //Длина BITMAP_INFO
    int32_t biWidth;                //Ширина картинки
    int32_t biHeight;              //высота картинки
    uint16_t biPlanes;            //Число плоскостей
    uint16_t biBitCnt;            //Бит на пиксел
    uint32_t biCompr;            //Тип сжатия
    uint32_t biSizeIm;            //Размер изображения в байтах
    int32_t biXPels;                //Разрешение по горизонтали
    int32_t biYPels;                //Разрешение по вертикали
    uint32_t biClrUsed;            //Количество используемых цветов
    uint32_t biClrImp;            //Количество основных цветов
} bitMapFile;
#pragma pack(pop)

bitMapFile bmp;

int** mallocPixelArray(int height, int width){
    int** resArr = (int**) calloc(height, sizeof (int));
    for (int i = 0; i < height; i++){
        resArr[i] = (int*)calloc(width, sizeof(int));
    }
    return resArr;
}

void freePixelArray(int height, int** arr){
    for (int i = 0; i < height; i++){
        free(arr[i]);
    }
    free(arr);
}

void rewriteHeadder(FILE *fin, FILE *fout){
    fread(&bmp, 1, sizeof(bitMapFile), fin);
    fwrite(&bmp, 1, sizeof(bitMapFile), fout);
}

int** bmpToPixelsArray(int height, int width, FILE* fin){
    int **resArr = mallocPixelArray(height, width);
    fseek(fin, (int)bmp.bfOffs, SEEK_SET);  // перемещаем указатель на начало считываения пикселей

    int realWidth = (width / 8) + ((width % 8 > 0) ? 1 : 0);    // действительная ширина где находятся значащие биты
    int scanLineSize = realWidth;
    if (realWidth % 4 != 0){
        scanLineSize = realWidth + (4 - (realWidth % 4));
    }
    char* scanLine = calloc(scanLineSize, sizeof(char));

    for (int y = height - 1; y >= 0; y--){  // записываем снизу вверх, т.к. пиксели считываются с левого нижнего угла
        fread(scanLine, 1, scanLineSize, fin);  // cчитываем строку с учётом паддинга
        for (int x = 0; x < width; x++){
            int curBytePos = x / 8;
            int curBit = 1 << (7 - x % 8);
            int pixel = scanLine[curBytePos] & curBit;
            if (pixel == 0){        // если бит равен 1(белый цвет), то будем помечать его как 1, если равен нулю - нулём
                resArr[y][x] = 1;
            }
            else{
                resArr[y][x] = 0;
            }
        }
    }

    free(scanLine);

    return resArr;
}

void printArr(int** arr, int height, int width){
    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            printf("%d", arr[y][x]);
        }
        printf("\n");
    }
}



int main() {

    FILE *fin = fopen("test.bmp", "rb");
    FILE *fout = fopen("Finish.bmp", "wb");

    fread(&bmp, 1, sizeof(bitMapFile), fin);
    printf("height - %d\n  width - %d", bmp.biHeight, bmp.biWidth);
    printf("\n");

    int **pixelArr = bmpToPixelsArray(bmp.biHeight, bmp.biWidth, fin);

    printArr(pixelArr, bmp.biHeight, bmp.biWidth);

    freePixelArray(bmp.biHeight, pixelArr);

    fclose(fin);
    fclose(fout);
}
