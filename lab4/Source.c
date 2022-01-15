#include<stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TAG 
{
    char ver[3];
    char v[2];
    char flag;
    char size[4];
} TAG;

typedef union bytesize 
{
    int val;
    char byte[4];
};

typedef struct FRAME 
{
    char name[4];
    char size[4];
    char flag[2];
    char unicode;
} FRAME;

TAG TagHeader;
FRAME FrameHeader;

int str_cnt(char* str) 
{
    int i = 0;

    while (str[i] != '\0') 
    {
        i++;
    }

    return i;
}

int byte_to_int(char byte[4])
{
    return byte[0] << 24 | byte[1] << 16 | byte[2] << 8 | byte[3];
}

char* int_to_byte(int val)
{
    union bytesize x;
    char byte[4];

    x.val = val;

    byte[0] = x.byte[0] & 0x7f;
    byte[1] = (x.byte[1] >> 7) & 0x7f;
    byte[2] = (x.byte[2] >> 14) & 0x7f;
    byte[3] = (x.byte[3] >> 21) & 0x7f;

    return byte;
}

int int_header(char byte[4]) 
{
    return byte[0] << 21 | byte[1] << 14 | byte[2] << 7 | byte[3];
}

int size_of_file(const char* file_name) 
{

    int file_size = 0;
    FILE* new = fopen(file_name, "rb");

    while (fgetc(new) != EOF)
        file_size++;

    fclose(new);

    return file_size;
}

void show(char* file_name) 
{

    FILE* file = fopen(file_name, "rb");

    if (file == NULL) 
    {
        printf("Cannot open file");
        exit(1);
    }

    fread(&TagHeader, 1, 10, file);
    printf("%sv%d.%d\n", TagHeader.ver, TagHeader.v[0], TagHeader.v[1]);


    int tag_size = int_header(TagHeader.size);

    printf("tag_size = %d\n", tag_size);

    int file_size = size_of_file(file_name);

    printf("file_size = %d\n", file_size);

    while (ftell(file) < tag_size) 
    {

        fread(&FrameHeader, 1, 11, file);

        if (FrameHeader.name[0] == 0) 
            break;

        int frame_size = byte_to_int(FrameHeader.size);

        if (FrameHeader.name[0] == 'A') 
        {
            fseek(file, frame_size, SEEK_CUR);
            continue;
        }
        char* frame_cont = calloc(frame_size, 1);

        fgets(frame_cont, frame_size, file);
        printf("id: %5s || size: %5d || value: ", FrameHeader.name, frame_size);
        printf("%s\n", frame_cont);

        free(frame_cont);
    }
    fclose(file);
}

void get(char* file_name, char* frame) 
{

    FILE* file = fopen(file_name, "rb");

    if (file == NULL) 
    {
        printf("Cannot open file");
        exit(1);
    }

    fread(&TagHeader, 1, 10, file);
    printf("%sv%d.%d\n", TagHeader.ver, TagHeader.v[0], TagHeader.v[1]);

    int tag_size = int_header(TagHeader.size);

    printf("tag size = %d\n", tag_size);

    while (ftell(file) < tag_size) 
    {

        fread(&FrameHeader, 1, 11, file);

        int frame_size = byte_to_int(FrameHeader.size);

        if (strcmp(frame, FrameHeader.name) == 0) 
        {

            int frame_size = byte_to_int(FrameHeader.size);
            char* frame_cont = calloc(frame_size, 1);

            fgets(frame_cont, frame_size, file);
            printf("id: %5s || size: %5d || value: ", FrameHeader.name, frame_size);
            printf("%s\n", frame_cont);

            free(frame_cont);
            break;
        }
        fseek(file, frame_size - 1, SEEK_CUR);
    }
    fclose(file);
}

void set_value(char* file_name, char* frame, char* val) 
{

    FILE* file = fopen(file_name, "rb");

    if (file == NULL) 
    {
        printf("Cannot open file");
        exit(1);
    }

    int file_size = size_of_file(file_name);

    fread(&TagHeader, 1, 10, file);
    printf("%sv%d.%d  ", TagHeader.ver, TagHeader.v[0], TagHeader.v[1]);

    int tag_size = int_header(TagHeader.size);

    printf("tag size = %d\n", tag_size);

    while (ftell(file) < tag_size) 
    {

        fread(&FrameHeader, 1, 11, file);

        if (strcmp(frame, FrameHeader.name) == 0) 
        {
            int frame_size = byte_to_int(FrameHeader.size);
            int left_ptr = ftell(file) - 7;
            int right_ptr = left_ptr + frame_size + 6;
            int val_size = str_cnt(val);
            char* left_buf = calloc(1, left_ptr);
            char* right_buf = calloc(1, file_size - right_ptr);

            fseek(file, 0, 0);
            fread(left_buf, 1, left_ptr, file);

            fseek(file, right_ptr, 0);
            fread(right_buf, 1, file_size - right_ptr, file);

            FILE* new = fopen("new.mp3", "wb");

            fwrite(left_buf, 1, left_ptr, new);

            union bytesize x;
            char info_buf[7];

            x.val = val_size + 1;

            info_buf[0] = x.byte[3];
            info_buf[1] = x.byte[2];
            info_buf[2] = x.byte[1];
            info_buf[3] = x.byte[0];
            info_buf[4] = FrameHeader.flag[0];
            info_buf[5] = FrameHeader.flag[1];
            info_buf[6] = FrameHeader.unicode;

            fwrite(info_buf, 1, 7, new);

            fwrite(val, 1, val_size, new);

            fwrite(right_buf, 1, file_size - right_ptr, new);

            fclose(new);

            free(left_buf);
            free(right_buf);

            printf("\nEdited\n");
            break;
        }
    }
    fclose(file);
}

int main(int argc, char* argv[])
{

    char* get_val;
    char* name = strpbrk(argv[1], "=") + 1;

    if (argv[argc - 1][2] == 's' && argv[argc - 1][3] == 'h' && argv[argc - 1][4] == 'o' && argv[argc - 1][5] == 'w')
        show(name);

    else if (argv[argc - 1][2] == 'g' && argv[argc - 1][3] == 'e' && argv[argc - 1][4] == 't')
    {
        get_val = strpbrk(argv[argc - 1], "=") + 1;
        get(name, get_val);
    }

    else if (argc >= 4)
    {
        char* frame = strpbrk(argv[2], "=") + 1;
        char* set_val = strpbrk(argv[3], "=") + 1;
        set_value(name, frame, set_val);
    }
    return 0;
}
