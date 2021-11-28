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

void rewriteHeadder(FILE *fin, FILE *fout){
    fread(&bmp, 1, sizeof(bitMapFile), fin);
    fwrite(&bmp, 1, sizeof(bitMapFile), fout);
}

int** bmpToPixelsArray(int height, int width){
    int **resArr = mallocPixelArray(height, width);

    
}

int main() {
    FILE *fin = fopen("test.bmp", "rb");
    FILE *fout = fopen("Finish.bmp", "wb");
    rewriteHeadder(fin, fout);

    int **pixelArr;



    fclose(fin);
    fclose(fout);
}
