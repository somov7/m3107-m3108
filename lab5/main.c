#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

typedef struct BmpHeader
{
    int width;
    int height;
    int size;
    int position;
} BmpHeader;

typedef struct RawBmpHeader
{
    unsigned char hm[2];
    unsigned char size[4];
    unsigned char reserved[4];
    unsigned char position[4];
    unsigned char headerSize[4];
    unsigned char width[4];
    unsigned char height[4];
} RawBmpHeader;

typedef struct bit
{
    unsigned char blue;
    unsigned char green;
    unsigned char red;
    unsigned char alpha;
} bit;

int bytes_to_int(unsigned char *ch)
{
    return (int)(ch[3] << 24 | ch[2] << 16 | ch[1] << 8 | ch[0]);
}

int size_from_char(unsigned char *size, int offset)
{

    int result = 0;
    for (int i = 0; i < 4; i++)
    {
        result |= (size[i + offset] << (8 * i));
    }
    return result;
}

void output(char *dir, int number, int **data, unsigned char *image, RawBmpHeader rawHeader, BmpHeader header, unsigned char *headerArr)
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
    strncpy(&name[k], ".bmp", 5);
    k = 0;
    for (int i = header.height - 1; i >= 0; i--)
    {
        int j = 0;
        while (j < header.width)
        {
            unsigned char byte = 0;
            unsigned int st = 128;
            for (int p = 0; p < 8; p++)
            {
                if (j < header.width)
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
    FILE *fin = fopen(name, "wb");
    fwrite(headerArr, sizeof(char), header.position, fin);
    // fwrite(&rawHeader, sizeof(char), 26, fin);
    // fseek(fin, header.position, SEEK_SET);
    // fwrite(0x55, 1, header.position - 26, fin);
    fwrite(image, sizeof(char), header.size - header.position, fin);
    fclose(fin);
    free(name);
}

void game_of_life(int **old, int height, int width)
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
            new[i][j] = old[i][j];
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            count = -old[i][j];
            for (int x = -1; x <= 1; x++)
            {
                for (int y = -1; y <= 1; y++)
                {
                    count = count + old[(i + x + height) % height][(j + y + width) % width];
                }
            }
            if (old[i][j] == 1)
            {
                if (count == 5)
                {
                    new[i][j] = 0;
                }
            }
            if (old[i][j] == 0)
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
            old[i][j] = new[i][j];
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
    if (argc < 5)
    {
        printf("Error, not enought args");
        return 1;
    }
    else
    {
        int i = 1;
        int max_iter = 0;
        int dump_freq = 1;
        char *file;
        char *dir = NULL;

        while (i < argc)
        {
            if (strcmp(argv[i], "--input") == 0)
            {
                if (i + 1 < argc)
                {
                    file = argv[i + 1];
                }
                else
                {
                    printf("Error");
                    return 1;
                }
            }
            else if (strcmp(argv[i], "--output") == 0)
            {
                if (i + 1 < argc)
                {
                    dir = argv[i + 1];
                }
                else
                {
                    printf("Error");
                    return 1;
                }
            }
            else if (strcmp(argv[i], "--max_iter") == 0)
            {
                if (i + 1 < argc)
                {
                    max_iter = atoi(argv[i + 1]);
                }
            }
            else if (strcmp(argv[i], "--dump_freq") == 0)
            {
                if (i + 1 < argc)
                {
                    dump_freq = atoi(argv[i + 1]);
                }
            }

            i++;
        }
        if (dir == NULL)
        {
            printf("Error, no dir");
            return 1;
        }
        FILE *fin = fopen(file, "rb");
        if (fin == NULL)
        {
            printf("Error, file doesn`t exist");
            return 1;
        }

        RawBmpHeader rawHeader;
        fread(&rawHeader, sizeof(char), 26, fin);
        int position = bytes_to_int(rawHeader.position);
        // printf("%d\n", position);
        int size = bytes_to_int(rawHeader.size);
        // printf("%d\n", size);
        int width = bytes_to_int(rawHeader.width);
        int height = bytes_to_int(rawHeader.height);
        // printf("%d %d", width, height);

        fseek(fin, 0, SEEK_SET);
        unsigned char *headerArr = (char *)malloc(position * sizeof(char));
        fread(headerArr, sizeof(char), position, fin);

        BmpHeader header;
        header.size = size;
        header.position = position;
        header.width = width;
        header.height = height;

        fseek(fin, position, SEEK_SET);
        unsigned char *image = (char *)malloc((header.size - header.position) * sizeof(char));
        fread(image, sizeof(char), header.size - header.position, fin);

        int **start = (int **)malloc(header.height * sizeof(int *));
        for (int i = 0; i < header.height; i++)
        {
            start[i] = (int *)malloc(header.width * sizeof(int));
        }
        int k = 0;
        for (int i = header.height - 1; i >= 0; i--)
        {
            int j = 0;
            while (j < header.width)
            {
                for (int p = 0; p < 8; p++)
                {
                    if (j < header.width)
                    {
                        unsigned int bit = 1 & (image[k] >> 7 - p);
                        start[i][j] = bit;
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
        
        if (max_iter != 0)
        {
            for (int i = 2; i <= max_iter; i++)
            {
                game_of_life(start, header.height, header.width);
                if (i % dump_freq == 0)
                {
                    output(dir, i, start, image, rawHeader, header, headerArr);
                }
            }
        }
        else
        {
            for (int i = 2;; i++)
            {
                game_of_life(start, header.height, header.width);
                if (i % dump_freq == 0)
                {
                    output(dir, i, start, image, rawHeader, header, headerArr);
                }
            }
        }
        fclose(fin);
        return 0;
    }
}

//.\main.exe --input 1.bmp --output C:\Users\dmitr\Desktop\cppandc\prog\lab61 --max_iter 100