#include "game.h"
#include "bmp.h"
#include "glut.h"

int getCell(int **pixels, int x, int y, bitMapFile bmp) {
    if (x < 0 || x >= bmp.biWidth || y < 0 || y >= bmp.biHeight)
        return 0;
    return pixels[x][y];
}

int getNeighbours(int **pixels, int x, int y, bitMapFile bmp) {
    return getCell(pixels, x + 1, y, bmp)
        + getCell(pixels, x + 1, y + 1, bmp)
        + getCell(pixels, x + 1, y - 1, bmp)
        + getCell(pixels, x, y + 1, bmp)
        + getCell(pixels, x, y - 1, bmp)
        + getCell(pixels, x - 1, y - 1, bmp)
        + getCell(pixels, x - 1, y, bmp)
        + getCell(pixels, x - 1, y + 1, bmp);
}

int **newGeneration(int **pixels, bitMapFile bmp) {
    int x, y, v, n;
    int** pixels2;

    pixels2 = (int**)calloc(bmp.biWidth, sizeof(int*));
    for (int i = 0; i < bmp.biWidth; i++)
        pixels2[i] = (int*)calloc(bmp.biHeight, sizeof(int));

    for (y = 0; y < bmp.biHeight; y++)
        for (x = 0; x < bmp.biWidth; x++) {
            n = getNeighbours(pixels, x, y, bmp);
            if (pixels[x][y])
                if (n < 2 || n > 3)
                    v = 0;
                else
                    v = 1;
            else
                if (n == 3)
                    v = 1;
                else
                    v = 0;
            pixels2[x][y] = v;
        }
    return pixels2;
}

void setCell(byte* F, int x, int y, int v, bitMapFile bmp) {
    x = (x + bmp.biHeight) % bmp.biHeight;
    y = (y + bmp.biHeight) % bmp.biHeight;
    F[y * bmp.biHeight + x] = v;
}

void fieldInit(byte* F, int **pixels, bitMapFile bmp) {
    int x, y;

    for (y = 0; y < bmp.biHeight; y++)
        for (x = 0; x < bmp.biWidth; x++)
            setCell(F, x, y, pixels[x][y], bmp);
}