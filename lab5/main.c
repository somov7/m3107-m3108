#include <stdio.h>
#include "bmp.h"
#include "game.h"
#include "argParse.h"

int main(int argc, char** argv) {

    argParse(argc, argv);

    FILE *fin = fopen(INPUT_FILE_NAME, "rb");
    openCorrect(fin);

    bitMapFile bmp;

    fread(&bmp, 1, sizeof(bitMapFile), fin);
    char *offset = calloc(bmp.bfOffs, sizeof(char));
    fseek(fin, 0, SEEK_SET);
    fread(offset, 1, bmp.bfOffs, fin);
    printf("height - %d\n  width - %d\n", bmp.biHeight, bmp.biWidth);

    int **pixelArr = bmpToPixelsArray(bmp.biHeight, bmp.biWidth, fin, bmp);

    for (int i = 0; i < 600; i++){
        pixelArr = gameLife(pixelArr, bmp.biHeight, bmp.biWidth);
        if (i % 10 == 0){
            pixelArrayToBmp(pixelArr, bmp.biHeight, bmp.biWidth, offset, i, (int)bmp.bfOffs);
        }
    }

    freePixelArray(bmp.biHeight, pixelArr);
    fclose(fin);
    free(INPUT_FILE_NAME);
    free(DIRECTORY_NAME);
}
