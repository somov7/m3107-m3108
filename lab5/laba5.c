#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "draw_img.h"

char* v = NULL;
char* new_v = NULL;
int h, w;
int linesize;

#pragma pack(push,1)
typedef struct tagBITMAPFILEHEADER
{
    uint16_t bfType;  //specifies the file type
    uint32_t bfSize;  //specifies the size in bytes of the bitmap file
    uint16_t bfReserved1;  //reserved; must be 0
    uint16_t bfReserved2;  //reserved; must be 0
    uint32_t bfOffBits;  //specifies the offset in bytes from the bitmapfileheader to the bitmap bits
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
    uint32_t biSize;  //specifies the number of bytes required by the struct
    int32_t biWidth;  //specifies width in pixels
    int32_t biHeight;  //specifies height in pixels
    uint16_t biPlanes;  //specifies the number of color planes, must be 1
    uint16_t biBitCount;  //specifies the number of bits per pixel
    uint32_t biCompression;  //specifies the type of compression
    uint32_t biSizeImage;  //size of image in bytes
    int32_t biXPelsPerMeter;  //number of pixels per meter in x axis
    int32_t biYPelsPerMeter;  //number of pixels per meter in y axis
    uint32_t biClrUsed;  //number of colors used by the bitmap
    uint32_t biClrImportant;  //number of colors that are important
} BITMAPINFOHEADER;
#pragma pack(pop)

BITMAPFILEHEADER bitmapFileHeader;
BITMAPINFOHEADER bitmapInfoHeader;
FILE* img;

void next_life(int freq) //генерируем новую жизнь наивным алгоритмом, new_v временный массив для новых битов
{
    for (int i = 0; i < freq; i++) {
        for (int x = 0; x < h; x++) {
            for (int y = 0; y < w; y++) {
                int c = 0;
                for (int t = -1; t <= 1; t++)
                    for (int j = -1; j <= 1; j++)
                        if (!(t == 0 && j == 0) && (x+t) >= 0 && (x+t) < h && (y+j) >= 0 
                        && (y+j) < w && v[(x+t)*w + y+j]) 
                            c++;
                int live = 0, t = x*w + y;
                if (v[t]) live = 1;

                if (live && c < 2) new_v[t] = 0;
                else if (live && c > 3) new_v[t] = 0;
                else if (live && (c == 2 || c == 3)) new_v[t] = 1;
                else if (!live && c == 3) new_v[t] = 1;
                else new_v[t] = 0;
            }
        }
        memcpy(v, new_v, w*h);
        memset(new_v, 0, w*h);
    }
}

int bmp_to_arr(char* imgpath) //читаем 1-битовую bmp в массив v
{
    img = fopen(imgpath, "rb");
    if (!img) {
        printf("Can't open file");
        exit(1);
    }
    fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER),1,img); //читаем файл хедер бмп
    if (bitmapFileHeader.bfType != 0x4D42) {//проверим если тип файла бмп 
        fclose(img);
        exit(1);
    }

    fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER),1,img); //читаем инфо хедер

    h = bitmapInfoHeader.biHeight;
    w = bitmapInfoHeader.biWidth;
    v = (char*)malloc(w*h); //выделим память под массив w*h, вместо двумерного будем использовать одномерный, вместо v[i][j] будем v[i*w+j]
    fseek(img, bitmapFileHeader.bfOffBits, 0); //скипнем до начала байтов нашей картинки
    linesize = ((w * bitmapInfoHeader.biBitCount + 31) / 32) * 4; //размер сканлинии в байтах, байтов в строке всегда кратно 4
    //если у нас ширина картинки 20 пикселей, на пиксель нужен 1 бит, нам достаточно 3 байтов, но будет добавлен еще 1 для кратности
    int filesize = linesize * h;
    printf("linesize: %d\n", linesize);
    char* data = (char*)malloc(filesize);
    fread(data, 1, filesize, img); //прочитаем все байты картинки
    for(int y = h - 1; y >= 0; y--) {
        for(int x = 0; x < w; x++) {
            int pos = y * linesize + x / 8; //возьмем первый байт (первый с последней по формату бмп)
            //x / 8 - восемь раз читаем один байт, перед тем, как перейти к следующему
            //y * linesize - скипаем паддинг байты
            int bit = 1 << (7 - x % 8); //возьмем первый бит, потом второй, третий до 8
            int vx = (data[pos] & bit) > 0; //если бит в нужной позиции в нашем байте равен 1, значение будет 1
            v[y * w + x] = vx; //v[y][x] = vx;
        }
    }

    free(data);

    return 0;
}

void arr_to_bmp(char* folderpath, int count)
{
    char* buf = (char*)malloc(bitmapFileHeader.bfOffBits);
    fseek(img, 0, SEEK_SET);
    fread(buf, 1, bitmapFileHeader.bfOffBits, img); //структуры бмп изменять не будем, т.к. ничего не изменилось кроме положения битов 0 1

    char* path = (char*)malloc(sizeof(folderpath)+6);
    sprintf(path, "%s\\%d.bmp", folderpath, count); //форматируем путь в соответствии с папкой и count итерацией
    FILE* resimg = fopen(path, "wb");
    if (resimg == NULL) {
        printf("Failed to open file");
        exit(1);
    }
    fwrite(buf, 1, bitmapFileHeader.bfOffBits, resimg);
    free(path);
    free(buf);
    char* scan = calloc(linesize, 1); //строки байтов размера linesize что мы будем писать в файл
    for(int y = 0; y < h; y++) {
        for(int x = 0; x < w; x++) {
            int pos = x / 8; //скипаем паддинг байты в строке байтов. если ширина 20 пикселей, будет от 0 до 2, всего 3 байта, в которых и есть все нужны биты
            if (v[y*w+x]) scan[pos] |= 1 << (7 - x % 8); //составляем байт
        }
        fwrite(scan, 1, linesize, resimg); //записали сканлинию
        memset(scan, 0, linesize);
    }
    free(scan);
    fclose(resimg);
}



int main(int argc, char* argv[])
{
    char* input_file;
    char* output_folder;
    int max_iter = 300; //ограничено 500 картинками, чтобы не убить SSD
    int dump_freq = 1;
    int random = 0;

    if (argc <= 4) {
        printf("Correct usage: --input <filename> --output <filedir>\n--max_iter <> --dump_freq <>");
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--input")) {
            if (i + 1 < argc) {
                input_file = (char*)malloc(strlen(argv[i+1]));
                strcpy(input_file, argv[i+1]);
            }
            else {
                printf("Wrong format");
                return 1;
            }

        }
        else if (!strcmp(argv[i], "--output")) {
            if (i + 1 < argc) {
                output_folder = (char*)malloc(strlen(argv[i+1]));
                strcpy(output_folder, argv[i+1]);
            }
            else {
                printf("Wrong format");
                return 1;
            }
        }
        else if (!strcmp(argv[i], "--max_iter")) {
            if (i + 1 < argc) {
                max_iter = atoi(argv[i+1]);
            }
            else {
                printf("Wrong format");
                return 1;
            }
        }
        else if (!strcmp(argv[i], "--dump_freq")) {
            if (i + 1 < argc) {
                dump_freq = atoi(argv[i+1]);
            }
            else
            {
                printf("Wrong format");
                return 1;
            }
        }
    }

    bmp_to_arr(input_file); //прочитаем бмп в массив, чтобы могли генеририровать новые картинки

    new_v = calloc(1,w*h);
    for (int i = 0; i < max_iter; i++) {
        next_life(dump_freq); //получаем новый массив по правилам игры с заданной частотой
        arr_to_bmp(output_folder, i); //переведим массив в бмп и сохраняем в указанной папке
    }

    fclose(img);
    free(v);
    free(new_v);
    
    draw_img(w, h);
    return 0;
}