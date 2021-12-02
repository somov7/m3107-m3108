#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char smth[18];
    char width[4];
    char height[4];
    char other[28];
} header_str;

typedef struct{
    int height, width;
} bitmap_str;

typedef struct {
    int R, G, B;
} pixel;

int bytes_to_int(unsigned char *ch);
int alive(pixel cell);
void game_life(int **field, int width, int height);
