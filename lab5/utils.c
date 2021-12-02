#include "utils.h"

int bytes_to_int(unsigned char *ch) {
    return (int)ch[3] << 24 | (int)ch[2] << 16 | (int)ch[1] << 8 | (int)ch[0];
}

int alive(pixel cell) {
    if (cell.B == 0 && cell.R == 0 && cell.G == 0)
        return 1;
    return 0;
}

int get_cell(int x, int y, int w, int h, int **cell) {
    x = (x + w) % w;
    y = (y + h) % h;
    return cell[y][x];
}

int get_neighbours (int x, int y, int **cells, int w, int h) {
   return get_cell(x - 1, y + 1, w, h, cells) + get_cell(x, y + 1, w, h, cells) + get_cell(x + 1, y + 1, w, h, cells) +
           get_cell(x - 1, y, w, h, cells) + get_cell(x + 1, y, w, h, cells) + get_cell(x - 1, y - 1, w, h, cells) +
           get_cell(x, y - 1, w, h, cells) + get_cell(x + 1, y - 1, w, h, cells);
}

void game_life(int **field, int width, int height) {
    int neigh;

    int **tmp = (int **)malloc(sizeof(int*) * height);
    for (int i = 0; i < height; i++)
        tmp[i] = (int*)malloc(width * sizeof(int));

    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++) {
            neigh = get_neighbours(x, y, field, width, height);
            if (field[y][x] == 1) {
                if (neigh < 2 || neigh > 3)
                    tmp[y][x] = 0;
                else
                    tmp[y][x] = 1;
            }
            else {
                if (neigh == 3)
                    tmp[y][x] = 1;
                else
                    tmp[y][x] = 0;
            }

        }
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            field[y][x] = tmp[y][x];

    for (int i = 0; i < height; i++)
        free(tmp[i]);
    free(tmp);
}


