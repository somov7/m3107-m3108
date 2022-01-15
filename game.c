#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define garbage_bytes ((24* img.width + 31) / 32) * 4

const int inf = 1e9;

struct BITMAP_header {
    char name[2];
    uint32_t size;
    uint32_t garbage;
    uint32_t image_offset;
};

struct DIB_header {
    uint32_t header_size;
    uint32_t width;
    uint32_t height;
    uint16_t color_planes;
    uint16_t bits_per_pixel;
    uint32_t compression;
    uint32_t image_size;
    uint32_t temp[4];
};

struct PIXEL {
    uint8_t b;
    uint8_t g;
    uint8_t r;
};

struct image {
    uint32_t height;
    uint32_t width;
    struct PIXEL **pixel;
};

struct ALL_file {
    struct BITMAP_header bitmap;
    struct DIB_header DIB;
    struct image img;
};

struct image read_image(FILE *file, int height, int width) {
    struct image img;
    img.pixel = (struct PIXEL**) malloc(sizeof(struct PIXEL*) * height);
    img.height = height;
    img.width = width;

    for (int i = height - 1; i >= 0; i--) {
        img.pixel[i] = (struct PIXEL*) malloc(sizeof(struct PIXEL) * (garbage_bytes / sizeof(struct PIXEL) + 1));
        fread(img.pixel[i], 1, garbage_bytes, file);
    }

    return img;
}

void free_imagen(struct image img) {
    for (int i = img.height - 1; i >= 0; i--) {
        free(img.pixel[i]);
    }
    free(img.pixel);
}
char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

void creatr_new_file(struct BITMAP_header header, struct DIB_header dib_header, struct image img, int x, char* output_name) {
    FILE *out_file = fopen("arts\\new.bmp", "w");
    if (out_file == NULL) {
        printf("creatr file Error\n");
        return;
    }

    fwrite(header.name, 2, 1, out_file);
    fwrite(&header.size, sizeof(int32_t) * 3, 1, out_file);
    fwrite(&dib_header, sizeof(struct DIB_header), 1, out_file);


    for (int i = img.height - 1; i >= 0; i--) {
        fwrite(img.pixel[i], garbage_bytes, 1, out_file);
    }

    fclose(out_file);
}

struct ALL_file open_bmp(char* file_name) {
    FILE *file = NULL;
    file = fopen(file_name, "rb");
    if (file == NULL) {
        return;
    }


    struct BITMAP_header header;
    fread(header.name, 2, 1, file);
    fread(&header.size, sizeof(int32_t) * 3, 1, file);

    if (header.name[0] != 'B' || header.name[1] != 'M') {
        fclose(file);
        printf("    Read error 1\n");
        return;
    }

    struct DIB_header dib_header;
    fread(&dib_header, sizeof(struct DIB_header), 1, file);

    if (dib_header.header_size != 40 || dib_header.bits_per_pixel != 24 || dib_header.compression != 0) {
        fclose(file);
        printf("    Read error 2\n");
        return;
    }

    fseek(file, header.image_offset, SEEK_SET);
    struct image img = read_image(file, dib_header.height, dib_header.width);
    fclose(file);

    struct ALL_file all = {header, dib_header, img};
    return all;
}

int string_to_int(char* s) {
    int size = strlen(s);
    int k = 1, ans = 0;
    for (int i = size - 1; i >= 0; i--) {
        if ((s[i] < '0' || s[i] > '9') && s[i] > 32) {
            printf("Error!\n");
            return 0;
        }
        ans += (s[i] - '0') * k;
        k *= 10;
    }

    return ans;
}

int main(int argc, char *argv[])
{
    if(argc > 9 || argc % 2 == 0)
    {
        printf("Something superfluous\n");
        return 0;
    }
    if(argc < 5)
    {
        printf("something is missing\n");
        return 0;
    }

    char *file_name = "file.bmp", output_name = NULL;
    int max_iter = inf, dump_freq = 1;
    for(int i = 1; i < argc - 1; i++)
    {
        int fl = 0;
        char *arg = argv[i];

        if(strcmp("--input", arg) == 0) {
            file_name = argv[i + 1];
            i++;
        }
        else if(strcmp("--output", arg) == 0) {
            output_name = argv[i + 1];
            i++;
        }
        else if(strcmp("--max_iter", arg) == 0) {
            max_iter = string_to_int(argv[i + 1]);
            i++;
        }
        else if(strcmp("--dump_freq", arg) == 0) {
            dump_freq = string_to_int(argv[i + 1]);
            i++;
        }
        else {
            printf("%s\n", argv[i]);
            printf("ERROR!\n");
            return 0;
        }
    }

    struct ALL_file all = open_bmp(file_name);
    struct image img = all.img;
    struct image img_buff;
    img_buff.pixel = (struct PIXEL**) malloc(sizeof(struct PIXEL*) * img.height);
    img_buff.height = img.height;
    img_buff.width = img.width;

    for (int i = img.height - 1; i >= 0; i--) {
        img_buff.pixel[i] = (struct PIXEL*) malloc(sizeof(struct PIXEL) * (garbage_bytes / sizeof(struct PIXEL) + 1));
        for(int j = 0; j < img.width; j++) {
            img_buff.pixel[i][j].r = 255;
            img_buff.pixel[i][j].g = 255;
            img_buff.pixel[i][j].b = 255;
        }
    }

    int k = 0;
    while (max_iter--) {
        printf("max_iter: %d\n" ,max_iter);
        if (k % dump_freq == 0 && k > 0) {
            system("pause");
        }
        k++;
        for (int i = 0; i < img.height; i++) {
            for (int j = 0; j < img.width; j++) {
                int k = 0, r = 0, g = 0, b = 0;

                if (i - 1 >= 0) {
                    if (img.pixel[i - 1][j].r != 255 || img.pixel[i - 1][j].g != 255 || img.pixel[i - 1][j].b != 255){
                        k++;
                        r += img.pixel[i - 1][j].r;
                        g += img.pixel[i - 1][j].g;
                        b += img.pixel[i - 1][j].b;
                    }
                    if (j - 1 >= 0) {
                        if (img.pixel[i - 1][j - 1].r != 255 || img.pixel[i - 1][j - 1].g != 255 || img.pixel[i - 1][j - 1].b != 255){
                            k++;
                            r += img.pixel[i - 1][j - 1].r;
                            g += img.pixel[i - 1][j - 1].g;
                            b += img.pixel[i - 1][j - 1].b;
                        }
                    }
                    if (j + 1 < img.width) {
                        if (img.pixel[i - 1][j + 1].r != 255 || img.pixel[i - 1][j + 1].g != 255 || img.pixel[i - 1][j + 1].b != 255){
                            k++;
                            r += img.pixel[i - 1][j + 1].r;
                            g += img.pixel[i - 1][j + 1].g;
                            b += img.pixel[i - 1][j + 1].b;
                        }
                    }
                }
                if (i + 1 < img.height) {
                    if (img.pixel[i + 1][j].r != 255 || img.pixel[i + 1][j].g != 255 || img.pixel[i + 1][j].b != 255){
                        k++;
                        r += img.pixel[i + 1][j].r;
                        g += img.pixel[i + 1][j].g;
                        b += img.pixel[i + 1][j].b;
                    }
                    if (j - 1 >= 0) {
                        if (img.pixel[i + 1][j - 1].r != 255 || img.pixel[i + 1][j - 1].g != 255 || img.pixel[i + 1][j - 1].b != 255){
                            k++;
                            r += img.pixel[i + 1][j - 1].r;
                            g += img.pixel[i + 1][j - 1].g;
                            b += img.pixel[i + 1][j - 1].b;
                        }
                    }
                    if (j + 1 < img.width) {
                        if (img.pixel[i + 1][j + 1].r != 255 || img.pixel[i + 1][j + 1].g != 255 || img.pixel[i + 1][j + 1].b != 255){
                            k++;
                            r += img.pixel[i + 1][j + 1].r;
                            g += img.pixel[i + 1][j + 1].g;
                            b += img.pixel[i + 1][j + 1].b;
                        }
                    }
                }
                if (j - 1 >= 0) {
                    if (img.pixel[i][j - 1].r != 255 || img.pixel[i][j - 1].g != 255 || img.pixel[i][j - 1].b != 255){
                        k++;
                        r += img.pixel[i][j - 1].r;
                        g += img.pixel[i][j - 1].g;
                        b += img.pixel[i][j - 1].b;
                    }
                }
                if (j + 1 < img.width) {
                    if (img.pixel[i][j + 1].r != 255 || img.pixel[i][j + 1].g != 255 || img.pixel[i][j + 1].b != 255){
                        k++;
                        r += img.pixel[i][j + 1].r;
                        g += img.pixel[i][j + 1].g;
                        b += img.pixel[i][j + 1].b;
                    }
                }

                if (img.pixel[i][j].r != 255 || img.pixel[i][j].g != 255 || img.pixel[i][j].b != 255) {
                    if(k == 2 || k == 3) {
                        img_buff.pixel[i][j] = img.pixel[i][j];
                    }
                    else {
                        img_buff.pixel[i][j].r = 255;
                        img_buff.pixel[i][j].g = 255;
                        img_buff.pixel[i][j].b = 255;
                    }
                }
                else {
                    if (k == 3) {
                        img_buff.pixel[i][j].r = r / k;
                        img_buff.pixel[i][j].g = g / k;
                        img_buff.pixel[i][j].b = b / k;
                    }
                    else {
                        img_buff.pixel[i][j].r = 255;
                        img_buff.pixel[i][j].g = 255;
                        img_buff.pixel[i][j].b = 255;
                    }
                }
            }
        }
        for(int i = 0; i < img.height; i++) {
            for(int j = 0; j < img.width; j++) {
                img.pixel[i][j] = img_buff.pixel[i][j];
            }
        }

        if (k % dump_freq == 0) {
            printf("GO\n");
            creatr_new_file(all.bitmap, all.DIB, img, k / dump_freq, output_name);
            printf("Ok\n");
        }
    }
    printf("OK\n");

    free_imagen(img);
    printf("OK\n");
    free_imagen(img_buff);
    return 0;
}
