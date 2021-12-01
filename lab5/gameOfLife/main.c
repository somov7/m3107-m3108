#include <string.h>
#include "game.h"
#include "bmp.h"

BitMapFileHeader *bitmapFileHeader;
BitMapInfoHeader *bitmapInfoHeader;

int main(int argc, char *argv[]){
    if (argc < 5){
        printf("Invalid amount of arguments");
        return 1;
    }

    char* input_file;
    char* output_dir;
    int dump_freq = 1;
    int max_iter = 100;
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
                output_dir = (char*)malloc(strlen(argv[i+1]));
                strcpy(output_dir, argv[i+1]);
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

    int *height, *width;
    FILE *pImage;
    bmp_to_arr(input_file, &height, &width, &pImage, &bitmapFileHeader, &bitmapInfoHeader);



    int **arr = malloc(*height * sizeof(int*));
    for (int i = 0; i < height; i++){
        arr[i] = malloc(*width * sizeof(int));
        for (int j = 0; j < width; j++){
            //как-то заполнить массив
        }
    }

    for (int day = 0; day < max_iter; day++) {
        arr = gameOfLife(arr, *height, *width);
        printf("\n");
    }

    return 0;
}
