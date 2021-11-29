#include <stdio.h>
#include "bmp.h"
#include "game.h"

int main() {

    FILE *fin = fopen("test.bmp", "rb");
    FILE *fout = fopen("Finish.bmp", "wb");

    bitMapFile bmp;
    fread(&bmp, 1, sizeof(bitMapFile), fin);

    char *offset = calloc(bmp.bfOffs, sizeof(char));
    fseek(fin, 0, SEEK_SET);
    fread(offset, 1, bmp.bfOffs, fin);

    printf("height - %d\n  width - %d\n", bmp.biHeight, bmp.biWidth);

    int **pixelArr = bmpToPixelsArray(bmp.biHeight, bmp.biWidth, fin, bmp);
    printArr(pixelArr, bmp.biHeight, bmp.biWidth);
    printf("\n============================================================\n");

    for (int i = 0; i < 10; i++){
        pixelArr = gameLife(pixelArr, bmp.biHeight, bmp.biWidth);
        printArr(pixelArr, bmp.biHeight, bmp.biWidth);
        pixelArrayToBmp(pixelArr, bmp.biHeight, bmp.biWidth, offset, i, (int)bmp.bfOffs);
        printf("\n============================================================\n");
    }

    freePixelArray(bmp.biHeight, pixelArr);

    fclose(fin);
    fclose(fout);
}
