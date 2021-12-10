#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <direct.h>

#define SIZE_OF_FILE_HEADER 14
#define SIZE_OF_INFO_HEADER 40
#define MAX_STRING_SIZE 260
#define DEAD 0
#define ALIVE 1

#pragma pack(push, 1)
typedef struct //14bytes
{
    uint16_t bmpType;
    uint32_t bmpSize;         //4(bytes)
    uint16_t unused1;         //2
    uint16_t unused2;         //2
    uint32_t imageDataOffset; //4(bytes)
} FILEHEADER;

typedef struct //40bytes
{
    uint32_t headerSize;     //4
    int32_t width;           //4 (pixels)
    int32_t height;          //4 (pixels)
    uint16_t planes;         //2
    uint16_t bitPerPix;      //2
    uint32_t biCompression;  //4
    uint32_t biSizeImage;    //4
    int32_t biXPelsPerMeter; //4
    int32_t biYPelsPerMeter; //4
    uint32_t biClrUsed;      //4
    uint32_t biClrImportant; //4

} INFOHEADER;
#pragma pack(pop)

typedef struct coord
{
    int x;
    int y;
} coordinate;

typedef struct grid
{
    int **field;
    coordinate size;
} grid;

grid createNewField(int width, int height)
{
    int **field = (int **)(calloc(width, sizeof(int *)));

    for (int x = 0; x < width; x++)
    {
        field[x] = (int *)(calloc(height, sizeof(int)));
    }

    grid newGrid = {field, width, height};

    return newGrid;
}

int getPoint(grid *universe, int x, int y, INFOHEADER ih)
{
    if (x < 0 || x >= ih.width || y < 0 || y >= ih.height)
        return 0;
    return universe->field[x][y];
}

int getNeighbourQuantity(grid *universe, int x, int y, INFOHEADER ih)
{
    return getPoint(universe, x + 1, y, ih) +
           getPoint(universe, x + 1, y + 1, ih) +
           getPoint(universe, x + 1, y - 1, ih) +
           getPoint(universe, x, y + 1, ih) +
           getPoint(universe, x, y - 1, ih) +
           getPoint(universe, x - 1, y - 1, ih) +
           getPoint(universe, x - 1, y, ih) +
           getPoint(universe, x - 1, y + 1, ih);
}

grid newGeneration(grid *universe, FILEHEADER fh, INFOHEADER ih)
{
    int cellStatus, neighbours;

    grid new1 = createNewField(ih.width, ih.height);

    for (int y = 0; y < ih.height; y++)
        for (int x = 0; x < ih.width; x++)
        {
            neighbours = getNeighbourQuantity(universe, x, y, ih);
            if (universe->field[x][y])
                if (neighbours < 2 || neighbours > 3)
                    cellStatus = DEAD;
                else
                    cellStatus = ALIVE;
            else if (neighbours == 3)
                cellStatus = ALIVE;
            else
                cellStatus = DEAD;
            new1.field[x][y] = cellStatus;
        }
    return new1;
}

grid readBMP(FILEHEADER *fh, INFOHEADER *ih, char *filename)
{
    FILE *img = fopen(filename, "rb");
    if (img == NULL)
    {
        printf("error occured during fileopening");
        exit(EXIT_FAILURE);
    }

    fread(fh, sizeof(FILEHEADER), 1, img);

    if (fh->bmpType != 0x4D42)
    {
        printf("wrong file format");
        exit(EXIT_FAILURE);
    }

    fread(ih, sizeof(INFOHEADER), 1, img);
    int width = ih->width;
    int height = ih->height;

    grid universe = createNewField(ih->width, ih->height);
    fseek(img, fh->imageDataOffset, SEEK_SET);

    int widthInBytes = width / 8 + ((width % 8 > 0) ? 1 : 0);
    char *byte_array;
    int byteArrayLength = (widthInBytes % 4 != 0) ? (widthInBytes + (4 - (widthInBytes % 4))) : widthInBytes;
    byte_array = (char *)calloc(byteArrayLength, sizeof(char));

    for (int y = height - 1; y >= 0; y--)
    {
        fread(byte_array, 1, byteArrayLength, img);

        for (int x = 0; x < width; x++)
        {
            int currentByte = x / 8;
            int currentBit = 1 << (7 - x % 8);

            if (byte_array[currentByte] & currentBit)
                universe.field[x][y] = DEAD;
            else
                universe.field[x][y] = ALIVE;
        }
    }
    free(byte_array);
    return universe;
}

void writeBMP(grid universe, FILEHEADER fh, INFOHEADER ih, int generation, char *directory, char *filename)
{
    char *filePath = (char *)calloc(MAX_STRING_SIZE, sizeof(char));
    char *generation_num = (char *)calloc(30, sizeof(char));
    FILE *outimg;
    int width = ih.width, height = ih.height, size_of_byte_array;

    _itoa(generation, generation_num, 30);
    strcat(filePath, directory);
    strcat(filePath, "\\");
    strcat(filePath, generation_num);
    strcat(filePath, ".bmp");

    if ((outimg = fopen(filePath, "wb")) == NULL)
    {
        printf("error during path creating\n");
        return;
    }

    FILE *inimg = fopen(filename, "rb");
    char c;
    int i = 0;
    while (i++ < fh.imageDataOffset)
    {
        c = fgetc(inimg);
        fputc(c, outimg);
    }
    fclose(inimg);

    int widthInBytes = (width / 8) + ((width % 8 > 0) ? 1 : 0);

    if (widthInBytes % 4 != 0)
        size_of_byte_array = widthInBytes + (4 - (widthInBytes % 4));
    else
        size_of_byte_array = widthInBytes;

    for (int y = height - 1; y >= 0; y--)
    {
        char *byte_array = (char *)calloc(size_of_byte_array, sizeof(char));

        for (int x = 0; x < width; x++)
        {
            int currentByte = x / 8;

            if (universe.field[x][y] == 0)
                byte_array[currentByte] |= (char)(1 << (7 - x % 8));
        }

        fwrite(byte_array, 1, size_of_byte_array, outimg);
        free(byte_array);
    }
    fclose(outimg);
    free(filePath);
    free(generation_num);
}

int main(int argc, char **argv)
// int main()
{
    FILEHEADER fh;
    INFOHEADER ih;

    char *filename = argv[2];
    grid universe = readBMP(&fh, &ih, filename);

    char *output_directory = argv[4];
    if (_mkdir(output_directory) != 0)
        printf("invalid path");

    char *help;
    long long max_iterations;
    long long dump_frequency;

    if (argc == 5)
    {
        max_iterations = strtoll(argv[6], &help, 10);
    }
    else
        max_iterations = LONG_LONG_MAX;

    printf("%ld", max_iterations);

    if (argc == 7)
    {
        dump_frequency = strtoll(argv[8], &help, 10);
    }
    else
        dump_frequency = 1;

    printf("%ld", dump_frequency);

    for (long long i = 0; ((max_iterations != LONG_LONG_MAX) ? (1) : (i < max_iterations)); i++)
        if ((dump_frequency == 1) ? (1) : (i % dump_frequency == 0))
        {
            universe = newGeneration(&universe, fh, ih);
            writeBMP(universe, fh, ih, i, output_directory, filename);
        }
    return 0;
}