#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <stdint.h>

void gameLife(int** life, int height, int width){
    int neighbors;
    int nextim[height][width];
    int x, y;
    int inf = 0;
    for(y = 0; y < height; y++)
        for(x = 0; x < width; x++)
            nextim[y][x] = life[y][x];
    for(y = 0; y < height; y++){
        for(x = 0; x < width; x++){
            neighbors = life[(y + 1)%height][(width + x - 1)%width] + life[(y + 1)%height][x] + life[(y + 1)%height][(x + 1) % width] + life[y][(width + x - 1) % width] + life[y][(x + 1) % width] + life[(height + y - 1) % height][(width + x - 1) % width] + life[(height + y - 1) % height][x] + life[(height + y - 1) % height][(x + 1) % width];
            if(nextim[y][x] == 1){
                if(neighbors == 2)
                    nextim[y][x] = life[y][x];
                if(neighbors == 3)
                    nextim[y][x] = life[y][x];
                if(neighbors > 3)
                    nextim[y][x] = 0;
                if(neighbors < 2)
                    nextim[y][x] = 0;
            }
            else
                if(neighbors == 3)
                    nextim[y][x] = 1;
                neighbors = 0;
            }
        }
    for(y = 0; y < height; y++)
        for(x = 0; x < width; x++){
            if(life[y][x] == nextim[y][x])
                inf++;
            life[y][x] = nextim[y][x];
        }
    if(inf == height * width)
        exit(0);
}
struct Bmp{
    int width;
    int height;
    int size;
};
int main(int argc, char* argv[]){
    struct Bmp image;
    unsigned char header[54];
    int i, j, k, l, m;
    int maxiter = 1;
    int dumpfreq = 1;
    char* dirname;
    FILE* filein;
    for(i = 0; i < argc; i++){
        if(!strcmp("--input", argv[i]))
            filein = fopen(argv[i + 1], "rb");
        if(!strcmp("--output", argv[i])){
            dirname = argv[i + 1];
            mkdir(dirname);
        }
        if(!strcmp("--max_iter", argv[i]))
            maxiter = strtol(argv[i + 1], 0, 10);
        if(!strcmp("--dump_freq", argv[i]))
            dumpfreq = strtol(argv[i + 1], 0, 10);
    }
    fread(header, 1, 54, filein);
    memcpy(&image.width, &header[18], sizeof(image.width));
    memcpy(&image.height, &header[22], sizeof(image.height));
    memcpy(&image.size, &header[2], sizeof(image.size));
    unsigned char imagebyte[image.size - 54];
    fread(imagebyte, 1, image.size, filein);
    int** img = (int**)malloc(image.height * sizeof(int*));
    for(i = 0; i < image.height; i++)
        img[i] = (int*)malloc(image.width * sizeof(int));
    k = image.width % 4;
    for(i = image.height - 1; i >= 0; i--){
        for(j = 0; j < image.width; j++){
            if(imagebyte[k] == 255)
                img[i][j] = 0;
            else
                img[i][j] = 1;
            k += 3;
        }
    }
    for (l = 1; l <= maxiter; l++){
        if(l % dumpfreq == 0){
            char filename[l];
            char dir_way[40];
            strcpy(dir_way, dirname);
            strcat(strcat(dir_way, "\\"), strcat(itoa(l, filename, 10), ".bmp"));
            FILE* life = fopen(dir_way, "w");
            fwrite(header, 1, 54, life);
            m = 0;
            for (i = image.height - 1; i >= 0; i--){
                for (j = 0; j < image.width; j++){
                    for (k = 0; k < 3; k++) {
                        if (img[i][j] == 1)
                            imagebyte[m] = 0;
                        else
                            imagebyte[m] = 255;
                        m++;

                    }
                }
                while (m % 4 != 0) {
                    imagebyte[m] = 0;
                    m++;
                }
            }
            fwrite(imagebyte, 1, image.size, life);
            fclose(life);
        }
        gameLife(img, image.height, image.width);

    }
    free(img);
    return 0;
}