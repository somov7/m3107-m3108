#include <stdio.h>
#include <inttypes.h>

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


int main() {


}
