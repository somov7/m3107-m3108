#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
typedef struct bmp
{
    unsigned char header[62];
    int width;
    int height;
    int size;
    int  bfOffBits;
} bmp;
int find_size(unsigned char *size, int from_where)

{
int result = 0;
for (int i = 0; i < 4; i++)
{
result |= (size[i + from_where] << (8 * i));
}
return result;
}
void output(char *dir, int number, int **data, unsigned char *image, int height, int width, unsigned char header[62], int size, int  bfOffBits)
{
    char file[50];
    itoa(number, file, 10);
    int sizename = strlen(dir) + strlen(file) + 6;
    char *name = (char *)malloc(sizename * sizeof(char));
    int k = 0;
    for (int i = 0; i < strlen(dir); i++)
    {
        name[i] = dir[i];
        k++;
    }
    name[k] = '\\';
    k++;
    for (int i = 0; i < strlen(file); i++)
    {
        name[k] = file[i];
        k++;
    }
    name[k] = '.';
    name[k + 1] = 'b';
    name[k + 2] = 'm';
    name[k + 3] = 'p';
    name[k + 4] = '\0';
    strncpy(&name[k], ".bmp", 5);

    k = 0;
    for (int i = height - 1; i >= 0; i--)
    {
        int j = 0;
        while (j < width)
        {
            unsigned char byte = 0;
            unsigned int st = 128;
            for (int p = 0; p < 8; p++)
            {
                if (j < width)
                {
                    byte += data[i][j] * st;
                    st = st / 2;
                    j++;
                }
            }
            image[k] = byte;
            k++;
        }
        while (k % 4 != 0)
        {
            image[k] = 0;
            k++;
        }
    }
    FILE *file_data = fopen(name, "wb");
    fwrite(header, sizeof(char),  bfOffBits, file_data);
    fwrite(image, sizeof(char), size -  bfOffBits, file_data);
    fclose(file_data);
    free(name);
}
void Game_of_life(int **map, int height, int width)
{
    int count;
    int **new = (int **)malloc(height * sizeof(int *));
    for (int i = 0; i < height; i++)
    {
        new[i] = (int *)malloc(width * sizeof(int));
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            new[i][j] = map[i][j];
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            count = -map[i][j];
            for (int x = -1; x <= 1; x++)
            {
                for (int y = -1; y <= 1; y++)
                {
                    int i1;
                    int j1;
                    if (i + x == -1)
                    {
                        i1 = height - 1;
                    }
                    else
                    {
                        i1 = i + x;
                    }
                    if (j + y == -1)
                    {
                        j1 = width - 1;
                    }
                    else
                    {
                        j1 = j + y;
                    }
                    count = count + map[i1 % height][j1 % width];
                }
            }
            if (map[i][j] == 1)
            {
                if (count == 5)
                {
                    new[i][j] = 0;
                }
            }
            if (map[i][j] == 0)
            {
                if ((count != 6) && (count != 5))
                {
                    new[i][j] = 1;
                }
            }
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            map[i][j] = new[i][j];
        }
    }
    for (int i = 0; i < height; i++)
    {
        free(new[i]);
    }
    free(new);
}
int main(int argc, char **argv)
{
    bmp bmp;
    char *file;
    char *dir = NULL;
    int maxiter = 0;
    int dumpfreq = 1;
    if (argc % 2 == 0)
    {
        printf("% s", "ERROR: invalid number of arguments");
        return 1;
    }
    for (int i = 1; i < argc - 1; i = i + 2)
    {
        if (strcmp(argv[i], "--input") == 0)
        {
            file = argv[i + 1];
        }
        else if (strcmp(argv[i], "--output") == 0)
        {
            dir = argv[i + 1];
        }
        else if (strcmp(argv[i], "--max_iter") == 0)
        {
            maxiter = atoi(argv[i + 1]);
        }
        else if (strcmp(argv[i], "--dump_freq") == 0)
        {
            dumpfreq = atoi(argv[i + 1]);
        }
    }
    if (dir == NULL)
    {
        printf("% s", "ERROR: directory not declared");
        return 1;
    }
    FILE *file_data;
    file_data = fopen(file, "rb");
    if (file_data == NULL)
    {
        printf("% s", "ERROR: the file does not exist");
        return 1;
    }
    fseek(file_data, 10, SEEK_SET);
    char temp[4];
    fread(temp, sizeof(char), 4, file_data);
    bmp.bfOffBits = find_size(temp, 0);
    fseek(file_data, 0, SEEK_SET);
    fread(bmp.header, sizeof(char), bmp.bfOffBits, file_data);
    bmp.size = find_size(bmp.header, 2);
    unsigned char *image = (char *)malloc((bmp.size - bmp.bfOffBits) * sizeof(char));
    fread(image, sizeof(char), bmp.size - bmp.bfOffBits, file_data);
    bmp.height = find_size(bmp.header, 22);
    bmp.width = find_size(bmp.header, 18);
    int **map = (int **)malloc(bmp.height * sizeof(int *));
    for (int i = 0; i < bmp.height; i++)
    {
        map[i] = (int *)malloc(bmp.width * sizeof(int));
    }
    int k = 0;
    for (int i = bmp.height - 1; i >= 0; i--)
    {
        int j = 0;
        while (j < bmp.width)
        {
            for (int p = 0; p < 8; p++)
            {
                if (j < bmp.width)
                {
                    unsigned int bit = 1 & (image[k] >> 7 - p);
                    map[i][j] = bit;
                    j++;
                }
            }
            k++;
        }
        while ((k) % 4 != 0)
        {
            k++;
        }
    }
    if (maxiter != 0)
    {
        for (int i = 1; i <= maxiter; i++)
        {
            Game_of_life(map, bmp.height, bmp.width);
            if (i % dumpfreq == 0)
            {
                output(dir, i, map, image, bmp.height, bmp.width, bmp.header, bmp.size, bmp.bfOffBits);
            }
        }
    }
    else
    {
        for (int i = 1; i > maxiter; i++)
        {
            Game_of_life(map, bmp.height, bmp.width);
            if (i % dumpfreq == 0)
            {

                output(dir, i, map, image, bmp.height, bmp.width, bmp.header, bmp.size, bmp.bfOffBits);
            }
        }
    }
    fclose(file_data);
    return 0;
}