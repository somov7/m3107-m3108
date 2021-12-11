#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define HSize 62
#define PathLen 200
#define SliceLen 120

typedef struct image_tag 
{
    uint8_t* header;
    uint8_t* image;
    uint8_t* data;
    int file_size;
    int header_size;
} image;

image bmp_to_img(FILE* f, int* width, int* height) 
{
    image res;
    uint8_t* head = malloc(HSize * sizeof(uint8_t));
    res.header_size = HSize;
    
    fread(head, 1, HSize, f);

    uint32_t offset = head[10] + (((uint8_t)head[11]) << 8) + (((uint8_t)head[12]) << 16) + (((uint8_t)head[13]) << 24);

    int w = head[18] + (((uint8_t)head[19]) << 8) + (((uint8_t)head[20]) << 16) + (((uint8_t)head[21]) << 24);
    int h = head[22] + (((uint8_t)head[23]) << 8) + (((uint8_t)head[24]) << 16) + (((uint8_t)head[25]) << 24);

    if (offset > HSize) 
    {
        free(head);
        head = malloc(offset * sizeof(uint8_t));
        fseek(f, 0, SEEK_SET);
        fread(head, 1, offset, f);
        res.header_size = (int)offset;
    }

    printf("offset = %d\n", offset);

    int lineSize = (w / 32) * 4;
    if (w % 32 != 0) 
        lineSize += 4;

    int fileSize = lineSize * h;

    uint8_t* img = malloc(w * h);
    uint8_t* data = malloc(fileSize);

    fseek(f, (long)offset, SEEK_SET);
    
    fread(data, 1, fileSize, f);

    printf("w = %d; h = %d \n", w, h);
    printf("bytes in line: %d; total size: %d \n", lineSize, fileSize);

    int cur_byte;
    for (int i = 0, reversed_i = h - 1; i < h; i++, reversed_i--) 
    {
        for (int j = 0; j < w; j++) {
            cur_byte = j / 8;

            uint8_t data_byte = data[i * lineSize + cur_byte];

            uint8_t mask = 0x80 >> j % 8;
            int pos = reversed_i * w + j;
            img[pos] = (data_byte & mask) ? 1 : 0;
        }
    }

    *width = w; *height = h;
    res.header = head;
    res.image = img;
    res.data = data;
    res.file_size = fileSize;
    return res;
}

void matrix_to_data(int n, int m, uint8_t** matrix, uint8_t* data) 
{
    int cur_pos = 0;
    uint8_t cur_byte = 0;

    for (int i = n; i >= 1; i--) 
    {
        for (int j = 1; j <= m; j++) 
        {

            uint8_t mask = 0x80 >> (j - 1) % 8;

            if (matrix[i][j] == '0') 
            {
                cur_byte = cur_byte | mask;
            }

            if ((j % 8) == 0 || j == m) 
            { 
                data[cur_pos] = cur_byte;
                cur_pos++;
                cur_byte = 0;
                mask = 0x80;
            }
        }
        while ((cur_pos) % 4 != 0) 
        {
            cur_pos++;
        }
    }
}

uint8_t** life_step(int n, int m, uint8_t** cur, uint8_t** target) 
{
    for (int i = 1; i < n + 1; i++) 
    {
        for (int j = 1; j < m + 1; j++) 
        {
            if (cur[i][j] == '0') 
            {
                int cnt = 0;
                for (int k = -1; k <= 1; k++) 
                {
                    for (int p = -1; p <= 1; p++) 
                    {
                        if (cur[i + k][j + p] == '1') 
                        {
                            cnt++;
                        }
                    }
                }
                if (cnt == 3) 
                {
                    target[i][j] = '1';
                }
                else {
                    target[i][j] = '0';
                }
            }
            else if (cur[i][j] == '1') 
            {
                int cnt = -1;
                for (int k = -1; k <= 1; k++) 
                {
                    for (int p = -1; p <= 1; p++) 
                    {
                        if (cur[i + k][j + p] == '1') 
                            cnt++;
                    }
                }
                if (cnt != 2 && cnt != 3)
                    target[i][j] = '0';
                else
                    target[i][j] = '1';
            }
        }
    }

    printf("\n");
    return target;
}

int main(int argc, char* argv[]) 
{

    if (argc < 5 || argc > 9) 
    {
        printf("wrong number of parameters: %d\n", argc);
        return 1;
    }

    char* file_name = argv[2];
    char* dir_name = argv[4];
    uint64_t max_iter = (uint64_t)INFINITY;
    uint32_t frequency = 1;

    for (int i = 1; i < argc; i += 2) {
        printf("%s ", argv[i]);
        if (strcmp(argv[i], "--input") == 0) {
            file_name = argv[i + 1];
            printf("%s\n", file_name);
        }
        else if (strcmp(argv[i], "--output") == 0) {
            dir_name = argv[i + 1];
            printf("%s\n", dir_name);
        }
        else if (strcmp(argv[i], "--max_iter") == 0) {
            max_iter = atoi(argv[i + 1]);
            printf("%lld\n", max_iter);
        }
        else if (strcmp(argv[i], "--dump_freq") == 0) {
            frequency = atoi(argv[i + 1]);
            printf("%u\n", frequency);
        }
        else {
            printf("wrong parameter: %s\n", argv[i]);
            return 1;
        }
    }

    FILE* input_file;
    input_file = fopen(file_name, "rb");

    if (input_file == NULL) {
        printf("can't open file %s\n", file_name);
        return 1;
    }

    int w, h;
    image bmp_struct = bmp_to_img(input_file, &w, &h);
    uint8_t* img = bmp_struct.image;
    uint8_t* bmpheighteader = bmp_struct.header;
    uint8_t* data = bmp_struct.data;

    int n = h;
    int m = w;
    uint8_t** matrix;

    matrix = (uint8_t**)malloc((n + 2) * sizeof(uint8_t*));
    for (int i = 0; i < n + 2; i++)
        matrix[i] = (uint8_t*)malloc((m + 2) * sizeof(uint8_t));

    uint8_t** arr;
    arr = (uint8_t**)malloc((n + 2) * sizeof(uint8_t*));

    for (int i = 0; i < n + 2; i++)
        arr[i] = (uint8_t*)malloc((m + 2) * sizeof(uint8_t));

    for (int i = 0; i < h; i++) 
    {
        for (int j = 0; j < w; j++) 
        {
            if (img[i * w + j]) 
            {
                matrix[i + 1][j + 1] = '0';
                arr[i + 1][j + 1] = '0';
            }
            else 
            {
                matrix[i + 1][j + 1] = '1';
                arr[i + 1][j + 1] = '1';
            }
        }
    }

    uint64_t iter = 0;
    uint32_t freq_counter = 0;
    int file_counter = 0;
    char* slice_name = "gen";
    uint32_t slice_name_len = strlen(slice_name);
    uint32_t dir_len = strlen(dir_name);
    uint8_t** cur_matrix;

    while (iter < max_iter) 
    {
        printf("000\n");
        if (iter % 2 == 0) 
        {
            life_step(n, m, arr, matrix);
            cur_matrix = matrix;
        }
        else 
        {
            life_step(n, m, matrix, arr);
            cur_matrix = arr;
        }
        freq_counter++;
        printf("freq cnt: %d frequency: %d \n", freq_counter, frequency);

        if (freq_counter == frequency) 
        {
            printf("freq cnt: %d frequency: %d 1\n", freq_counter, frequency);
            FILE* output_file;
            freq_counter = 0;
            file_counter++;

            char file_path[PathLen] = "";
            char cur_slice[SliceLen] = "";
            strcpy(cur_slice, slice_name);
            int length = snprintf(NULL, 0, "%d", file_counter);
            char* str = malloc((length + 1) * sizeof(char));

            printf("freq cnt: %d frequency: %d 2\n", freq_counter, frequency);
            printf("freq cnt: %d frequency: %d 2.5\n", freq_counter, frequency);
            snprintf(str, length + 1, "%d", file_counter);

            printf("%s\n", str);
            strcat(cur_slice, str);
            printf("cur_slice = %s\n", cur_slice);

            strcat(file_path, dir_name);
            strcat(file_path, "\\");
            
            strcat(file_path, cur_slice);
            strcat(file_path, ".bmp\0");
            printf("%s\n", file_path);
            printf("freq cnt: %d frequency: %d 3\n", freq_counter, frequency);
            output_file = fopen(file_path, "wb");
            if (output_file == NULL) {
                printf("can't open file %s\n", file_path);
                return 1;
            }
            printf("fine\n");
            matrix_to_data(n, m, cur_matrix, data);
            fwrite(bmpheighteader, sizeof(uint8_t), bmp_struct.header_size, output_file);
            fwrite(data, sizeof(uint8_t), bmp_struct.file_size, output_file);
            printf("make image slice\n");
        }
        iter++;
    }


    printf("completed successfully\n");
    fclose(input_file);
    free(matrix);
    free(arr);
    free(data);
    free(img);
    return 0;
}