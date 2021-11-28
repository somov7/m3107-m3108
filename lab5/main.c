#include <stdio.h>
#include "bmp.h"
#include "game.h"

int main() {

    FILE *fin = fopen("test.bmp", "rb");
    FILE *fout = fopen("Finish.bmp", "wb");

    bitMapFile bmp;
    fread(&bmp, 1, sizeof(bitMapFile), fin);
    printf("height - %d\n  width - %d\n", bmp.biHeight, bmp.biWidth);

    int **pixelArr = bmpToPixelsArray(bmp.biHeight, bmp.biWidth, fin, bmp);
    printArr(pixelArr, bmp.biHeight, bmp.biWidth);
    printf("\n============================================================\n");
    for (int i = 0; i < 10; i++){
        pixelArr = gameLife(pixelArr, bmp.biHeight, bmp.biWidth);
        printArr(pixelArr, bmp.biHeight, bmp.biWidth);
        printf("\n============================================================\n");
    }

    freePixelArray(bmp.biHeight, pixelArr);

    fclose(fin);
    fclose(fout);
}
