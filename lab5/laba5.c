#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "draw_img.h"

static char* game_array = NULL;
static char* game_array_buff = NULL;
static int height, width;
static int line_size;

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

static BITMAPFILEHEADER bitmapFileHeader;
static BITMAPINFOHEADER bitmapInfoHeader;
static FILE* image_file;


void next_life(int freq) //генерируем новую жизнь наивным алгоритмом, game_array_buff временный массив для новых битов
{
    for (int i = 0; i < freq; i++) {
        for (int i = 1 + width; i < width * height - width - 1; i += 8) {
            uint64_t* sum = (uint64_t*)(game_array_buff + i);
            *sum += *(uint64_t*)(game_array + i - width - 1);
            *sum += *(uint64_t*)(game_array + i - width);
            *sum += *(uint64_t*)(game_array + i - width + 1);
            *sum += *(uint64_t*)(game_array + i - 1);
            *sum += *(uint64_t*)(game_array + i + 1);
            *sum += *(uint64_t*)(game_array + i + width - 1);
            *sum += *(uint64_t*)(game_array + i + width);
            *sum += *(uint64_t*)(game_array + i + width + 1);
        }

        for (int i = width; i < width * height - width; i += 8) {
            uint64_t cnt = *(uint64_t*)(game_array_buff+i);
            uint64_t alive = *(uint64_t*)(game_array+i);
            cnt &= 0b0000011100000111000001110000011100000111000001110000011100000111ULL;
            //важны только последние 3 бита в количестве
            uint64_t alive_cnt = cnt | (alive << 3);
            //0011 - клетка мертва и 3 соседа = 1
            //1010 - клетка жива и 2 соседа = 1
            //1011 - клетка жива и 3 соседа = 1
            //на остальных наборах 0
            //x1 && ~x2 && x3 || ~x2 && x3 && x4 (сокращенная ДНФ, что дает 1 на этих наборах, можно найти по карте Карно)
            //либо записать СДНФ и склеить
            alive_cnt ^= 0b0000010000000100000001000000010000000100000001000000010000000100ULL;
            //взяли отрицание второго бита (x2), оно нужно в обеих конъюнктах;
            uint64_t keepAlive = alive_cnt & 0b0000111000001110000011100000111000001110000011100000111000001110ULL;
            //взяли x1, x2, x3
            keepAlive &= (keepAlive >> 1);
            //теперь второй бит это x1 && x2, третий x2 && x3, осталось применить конъюнкию еще раз для x1 && x2 && x3
            keepAlive &= (keepAlive >> 1);
            keepAlive >>= 1;
            //10 -> 1

            uint64_t makeNewLife = alive_cnt & 0b0000011100000111000001110000011100000111000001110000011100000111ULL;
            //взяли x2, x3, x4
            makeNewLife &= (makeNewLife >> 1);
            //теперь третий бит это x2 && x3, четвертый x3 && x4, применим И еще раз для x2 && x3 && x4
            makeNewLife &= (makeNewLife >> 1);

            //x1 && ~x2 && x3 || ~x2 && x3 && x4
            *(uint64_t*)(game_array+i) = keepAlive | makeNewLife;
        
       }

        for (int i = 1; i < height - 1; i++) {
            int t = i*width;
            game_array[t] = 0;
            game_array[t+width-1] = 0;
        }

        memset(game_array_buff, 0, width*height);
    }
}

int bmp_to_arr(char* imgpath) //читаем 1-битовую bmp в массив game_array
{
    image_file = fopen(imgpath, "rb");

    if (!image_file) {
        printf("Can't open file");
        exit(1);
    }
    const int bmp_const = 0x4D42;
    fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER),1,image_file); //читаем файл хедер бмп
    if (bitmapFileHeader.bfType != bmp_const) {//проверим если тип файла бмп 
        fclose(image_file);
        exit(1);
    }

    fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER),1,image_file); //читаем инфо хедер
    fseek(image_file, bitmapFileHeader.bfOffBits, 0); //скипнем до начала байтов нашей картинки
    line_size = ((bitmapInfoHeader.biWidth * bitmapInfoHeader.biBitCount + 31) / 32) * 4; //размер сканлинии в байтах, байтов в строке всегда кратно 4
    //если у нас ширина картинки 20 пикселей, на пиксель нужен 1 бит, нам достаточно 3 байтов, но будет добавлен еще 1 для кратности
    int filesize = line_size * bitmapInfoHeader.biHeight;

    height = bitmapInfoHeader.biHeight + 2;
    width = bitmapInfoHeader.biWidth + 2;

    game_array = (char*)calloc(sizeof(char),width*height); //выделим память под массив w*h, вместо двумерного будем использовать одномерный, вместо game_array[i][j] будем game_array[i*w+j]

    printf("line_size: %d\n", line_size);
    char* data = (char*)malloc(filesize);
    fread(data, 1, filesize, image_file); //прочитаем все байты картинки

    for (int h = height - 2; h >= 1; h--) {
        int t = h * width;
        for (int w = 1; w <= width-2; w++) {
            int pos = (h-1) * line_size + ((w-1) >> 3); //возьмем байт
            //x / 8 - восемь раз читаем один байт, перед тем, как перейти к следующему
            //y * line_size - скипаем паддинг байты
            int bit = 1 << (7 - ((w-1) & 7)); //возьмем первый бит, потом второй, третий до 8
            game_array[t + w] = (data[pos] & bit) == 0; //если бит в нужной позиции в нашем байте равен 0, значение будет 1
        }
    }

    free(data);

    return 0;
}

void arr_to_bmp(char* folder_path, int count)
{
    char* buf = (char*)malloc(bitmapFileHeader.bfOffBits);
    fseek(image_file, 0, SEEK_SET);
    fread(buf, 1, bitmapFileHeader.bfOffBits, image_file); //структуры бмп изменять не будем, т.к. ничего не изменилось кроме положения битов 0 1

    char* path = (char*)malloc(sizeof(folder_path)+10);
    sprintf(path, "%s\\%d.bmp", folder_path, count); //форматируем путь в соответствии с папкой и count итерацией
    FILE* result_image = fopen(path, "wb");
    if (result_image == NULL) {
        printf("Failed to open file");
        exit(1);
    }

    fwrite(buf, 1, bitmapFileHeader.bfOffBits, result_image); //пишем метаданные
    free(path);
    free(buf);

    char* scan = calloc(line_size, 1); 

    for (int h = 1; h < height-1; h++) {
        int t = h*width;
        for (int w = 1; w < width-1; w++)
            scan[(w-1) >> 3] |= (game_array[t+w] << (7 - (w-1) & 7)); //составляем сканлинию/байт
        fwrite(scan, 1, line_size, result_image); //записали сканлинию
        memset(scan, 0, line_size);
    }
    free(scan);
    fclose(result_image);
}



int main(int argc, char* argv[])
{
    char* input_file;
    char* output_folder;
    int max_iter = 3000; //ограничено 500 картинками, чтобы не убить SSD
    int dump_freq = 1;

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

    game_array_buff = calloc(sizeof(char), width*height);

    for (int i = 0; i < max_iter; i++) {
        next_life(dump_freq); //получаем новый массив по правилам игры с заданной частотой
        arr_to_bmp(output_folder, i); //переведим массив в бмп и сохраняем в указанной папке
    }

    fclose(image_file);
    free(game_array);
    free(game_array_buff);
    
    draw_img(width, height);
    return 0;
}