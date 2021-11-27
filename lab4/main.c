#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <stdint.h>

#pragma pack(push,1) 

typedef struct tagHEADER
{
    uint8_t idv3[3];
    uint8_t version[2];
    uint8_t flag;
    uint8_t size[4];
} HEADER;

typedef struct tagFRAME
{
    uint8_t frame_id[4];
    uint8_t size[4];
    uint8_t flag[2];
    uint8_t unicode;
} FRAME;

#pragma pack(pop)

HEADER header;
FRAME frame;

int byte_to_int(uint8_t ch[4])
{
    return (int)ch[0] << 21 | (int)ch[1] << 14 | (int)ch[2] << 7 | (int)ch[3] << 0;
}

void int_to_byte(int x, uint8_t* ch)
{
    //for version 2.4
    int bits = 127;

    ch[3] = x & bits;
    ch[2] = (x >> 7) & bits;
    ch[1] = (x >> 14) & bits;
    ch[0] = (x >> 21) & bits;
}

int get_show(uint8_t* file_path, uint8_t* frame_name, int set, int* old_frame_size)
{
    FILE* f = fopen(file_path, "rb");

    if (f == NULL)
    {
        printf("Can't open file");
        exit(1);
    }

    fread(&header, 1, 10, f);
    int k = byte_to_int(header.size, 1);
    printf("size struct: %d\n", k);

    int pos_write = 0;

    while (fread(&frame, 1, 11, f))
    {
        if (frame.frame_id[0] == 0 || ftell(f) >= k)
        {
            pos_write = ftell(f) - 11;
            break;
        }

        int sz = byte_to_int(frame.size, 0);
        uint8_t* buff = calloc(sz, sizeof(uint8_t));
        fread(buff, 1, sz - 1, f);

        if (frame_name == NULL)
        {
            printf("pos: %-10d id: %-10s size: %-10d value: ", ftell(f), frame.frame_id, sz);

            if (frame.unicode)
            {
                wprintf(L"%ls\n", buff + 2);
            }
            else
            {
                printf("%s\n", buff);
            }
        }
        else if (!strcmp(frame.frame_id, frame_name))
        {
            if (set == 0)
            {
                printf("id: %-10s value: ", frame.frame_id);

                if (frame.unicode)
                {
                    wprintf(L"%ls\n", buff + 2);
                }
                else
                {
                    printf("%s\n", buff);
                }
                return -1;
            }
            else
            {
                *old_frame_size = sz;
                pos_write = ftell(f) - 10 - sz;
                break;
            }
        }
        free(buff);
    }
    fclose(f);

    return pos_write;
}

void set(uint8_t* file_path, uint8_t* frame_name, uint8_t* prop_value)
{
    FILE* read = fopen(file_path, "rb");

    char* temp_file = calloc(1, strlen(file_path) + 5);
    sprintf(temp_file, "%s.temp", file_path);

    FILE* write = fopen(temp_file, "wb");

    if (read == NULL || write == NULL)
    {
        printf("Can't open/create file");
        exit(1);
    }

    int* old_frame_size = calloc(1, sizeof(int));
    int pos_write = get_show(file_path, frame_name, 1, old_frame_size);
    int size_diff = strlen(prop_value) - (*old_frame_size - 1);

    fread(&header, 1, 10, read);

    int k = byte_to_int(header.size, 1);
    k += size_diff;

    int_to_byte(k, header.size);
    fwrite(&header, 1, 10, write);

    uint8_t* buff = (uint8_t*)malloc(pos_write - 10);

    fread(buff, 1, pos_write - 10, read);
    fwrite(buff, 1, pos_write - 10, write);
    memcpy(frame.frame_id, frame_name, 4);

    uint8_t frame_size[4];
    int_to_byte(strlen(prop_value) + 1, frame_size);
    memcpy(frame.size, frame_size, 4);

    fwrite(&frame, 1, 11, write);
    fwrite(prop_value, 1, strlen(prop_value), write);
    free(buff);
    fseek(read, 0, SEEK_END);
    printf("old fr sz: %d\n", *old_frame_size);

    int read_offset = *old_frame_size ? pos_write + 10 + *old_frame_size : pos_write;
    int read_size = ftell(read) - read_offset;

    buff = (uint8_t*)malloc(read_size);
    fseek(read, read_offset, SEEK_SET);
    fread(buff, 1, read_size, read);
    fwrite(buff, 1, read_size, write);
    free(buff);

    fclose(read);
    fclose(write);
    remove(file_path);
    rename(temp_file, file_path);
}


void get_value(uint8_t* str, uint8_t** target)
{
    uint8_t* split = strtok(str, "=");

    if (split != NULL)
        split = strtok(NULL, "=");
    if (split == NULL) {
        printf("Wrong format");
        exit(1);
    }

    *target = malloc(strlen(split) + 1);
    strcpy(*target, split);
}


int main(int argc, uint8_t* argv[])
{
    uint8_t* file_path = NULL;
    uint8_t* frame_name = NULL;
    uint8_t* prop_value = NULL;
    int show = 0, get = 0, set = 0;

    if (argc < 3)
    {
        printf("Correct usage: --filepath=<path>\n--show\n--set=<propname> --value=<propvalue>\n--get=<propname>\n");
        return 1;
    }

    for (int i = 1; i < argc; i++)
    {
        if (strstr(argv[i], "--filepath"))
        {
            get_value(argv[i], &file_path);
            printf("file_path: %s\n", file_path);
        }
        else if (strstr(argv[i], "--set"))
        {
            set = 1;
            get_value(argv[i], &frame_name);
        }
        else if (strstr(argv[i], "--get"))
        {
            get = 1;
            get_value(argv[i], &frame_name);
        }
        else if (strstr(argv[i], "--value"))
        {
            get_value(argv[i], &prop_value);
        }
        else if (!strcmp(argv[i], "--show"))
        {
            show = 1;
        }
        else
        {
            printf("Wrong format");
            return 1;
        }
    }

    if (frame_name && strlen(frame_name) != 4)
    {
        printf("Wrong frame size: %d", strlen(frame_name));
        return 1;
    }

    if (show)
        get_show(file_path, NULL, 0, NULL);
    else if (get)
    {
        if (get_show(file_path, frame_name, 0, NULL) != -1)
            printf("Can't find frame: %s\n", frame_name);
    }
    else if (set)
        set(file_path, frame_name, prop_value);

    return 0;
}