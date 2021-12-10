#include "game.h"
#include "bmp.h"

BitMapFileHeader bitmapFileHeader;
BitMapInfoHeader bitmapInfoHeader;

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
//                input_file = (char*)malloc(strlen(argv[i+1]));
//                strcpy(input_file, argv[i+1]);
                input_file = argv[i + 1];
            }
            else {
                printf("Wrong format");
                return 1;
            }

        }
        else if (!strcmp(argv[i], "--output")) {
            if (i + 1 < argc) {
//                output_dir = (char*)malloc(strlen(argv[i+1]));
//                strcpy(output_dir, argv[i+1]);
                output_dir = argv[i + 1];
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

    int height, width;
    FILE *pImage;
    char *offset;

    int **gameField = bmp_to_arr(input_file, &height, &width, &pImage, &offset, &bitmapFileHeader, &bitmapInfoHeader);

    for(int i = 0; i < max_iter; i++){
        gameField = gameOfLife(gameField, height, width);
        if (i % dump_freq == 0)
            arr_to_bmp(gameField, height, width, i + 1, output_dir, offset, &bitmapFileHeader, &pImage);
    }

    free(offset);
    fclose(pImage);
    free(input_file);
    free(output_dir);
    return 0;
}
