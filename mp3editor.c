#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

typedef struct header
{
    char idv3[3];
    uint8_t version[2];
    uint8_t flag;
    uint8_t size[4];
} mp3header;

typedef struct frame
{
    char frame_id[4];
    uint8_t size[4];
    uint8_t flag[2];
    uint8_t unicode;

} mp3frame;

mp3header header;
mp3frame frame;

int byte_to_int(uint8_t your_size[4])
{
    return (int)your_size[0] << 21 | (int)your_size[1] << 14 | (int)your_size[2] << 7 | (int)your_size[3] << 0;
}

void int_to_byte(int x, uint8_t *buffer, bool flag)
{
    int bit = 127;
    if (header.version[0] == 4 || flag)
    {
        buffer[3] = x & bit;
        buffer[2] = (x >> 7) & bit;
        buffer[1] = (x >> 14) & bit;
        buffer[0] = (x >> 21) & bit;
    }
    else
    {
        buffer[3] = x & bit;
        buffer[2] = (x >> 8) & bit;
        buffer[1] = (x >> 16) & bit;
        buffer[0] = (x >> 24) & bit;
    }
}

void show(char *file_name)
{
    FILE *file = fopen(file_name, "rb");

    if (file == NULL)
    {
        printf("\nError occurred during fileopening\n");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_SET);
    fread(&header, sizeof(uint8_t), 10, file);

    unsigned int size_of_tag = byte_to_int(header.size);

    while (fread(&frame, sizeof(uint8_t), 11, file))
    {
        if (frame.frame_id[0] < 'A' || frame.frame_id[0] > 'Z')
        {
            break;
        }

        size_of_tag = byte_to_int(frame.size);

        uint8_t *buffer = (uint8_t *)calloc(size_of_tag, 1);
        fread(buffer, 1, size_of_tag - 1, file);
        buffer[size_of_tag - 1] = 0;

        printf("id: %s value ", frame.frame_id);
        if (frame.unicode)
        {
            wprintf(L"%ls\n", buffer + 2);
        }
        else
        {
            printf("%s\n", buffer);
        }

        free(buffer);
        buffer = NULL;
    }

    fclose(file);
}

void get(char *file_name, char *frame_name)
{
    FILE *file = fopen(file_name, "rb");

    if (file == NULL)
    {
        printf("\nError occurred during fileopening\n");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_SET);
    fread(&header, sizeof(uint8_t), 10, file);

    unsigned int size_of_tag = byte_to_int(header.size);
    bool printed = false;

    while (fread(&frame, sizeof(uint8_t), 11, file))
    {
        if (frame.frame_id[0] < 'A' || frame.frame_id[0] > 'Z')
        {
            break;
        }

        size_of_tag = byte_to_int(frame.size);

        uint8_t *buffer = (uint8_t *)calloc(size_of_tag, 1);
        fread(buffer, 1, size_of_tag - 1, file);
        buffer[size_of_tag - 1] = 0;

        if (strcmp(frame.frame_id, frame_name) == 0)
        {
            printed = true;

            if (frame.unicode)
            {
                wprintf(L"%ls\n", buffer + 2);
            }
            else
            {
                printf("%s\n", buffer);
            }
        }

        free(buffer);
        buffer = NULL;
    }

    if (!printed)
    {
        printf("no such frame exists\n");
        exit(EXIT_FAILURE);
    }

    fclose(file);
}

int where_to_write = 0;
int *old_frame_size;

void set(char *file_name, char *frame_name, char *frame_value)
{
    FILE *file_in = fopen(file_name, "rb");

    if (file_in == NULL)
    {
        printf("\nError occurred during fileopening\n");
        exit(EXIT_FAILURE);
    }

    fseek(file_in, 0, SEEK_SET);
    fread(&header, sizeof(uint8_t), 10, file_in);

    old_frame_size = (unsigned int *)(malloc(1 * sizeof(int)));
    int size_of_tag = byte_to_int(header.size);
    while (fread(&frame, sizeof(uint8_t), 11, file_in))
    {
        if (frame.frame_id[0] < 'A' || frame.frame_id[0] > 'Z')
        {
            break;
        }

        size_of_tag = byte_to_int(frame.size);

        uint8_t *buffer = (uint8_t *)calloc(size_of_tag, 1);
        fread(buffer, 1, size_of_tag - 1, file_in);

        if (strcmp(frame.frame_id, frame_name) == 0)
        {
            *old_frame_size = size_of_tag;
            where_to_write = ftell(file_in) - 10 - size_of_tag;
            free(buffer);
            buffer = NULL;
            break;
        }

        free(buffer);
        buffer = NULL;
    }

    rewind(file_in);

    FILE *file_out = fopen("temp", "wb");

    int sizes_difference = strlen(frame_value) - (*old_frame_size - 1);
    uint8_t new_frame_size[4];

    fread(&header, 1, 10, file_in);
    int new_header_size = byte_to_int(header.size);
    new_header_size += sizes_difference;

    int_to_byte(new_header_size, header.size, false);
    fwrite(&header, 1, 10, file_out);

    uint8_t *buffer = (uint8_t *)malloc(where_to_write - 10);
    fread(buffer, 1, where_to_write - 10, file_in);
    fwrite(buffer, 1, where_to_write - 10, file_out);
    free(buffer);

    memcpy(frame.frame_id, frame_name, 4);

    size_of_tag = byte_to_int(header.size);
    size_of_tag += sizes_difference;

    int_to_byte(size_of_tag, header.size, false);
    int_to_byte(strlen(frame_value) + 1, new_frame_size, true);
    memcpy(frame.size, new_frame_size, 4);

    fwrite(&frame, 1, 11, file_out);
    fwrite(frame_value, 1, strlen(frame_value), file_out);

    fseek(file_in, 0, SEEK_END);

    int readoff = (*old_frame_size) ? (where_to_write+ 10 + *old_frame_size) : (where_to_write);
    int readsize = ftell(file_in) - readoff;

    buffer = (uint8_t *)malloc(readsize);
    fseek(file_in, readoff, SEEK_SET);
    fread(buffer, 1, readsize, file_in);
    fwrite(buffer, 1, readsize, file_out);
    free(buffer);

    fclose(file_in);
    fclose(file_out);
    remove(file_name);
    rename("temp", file_name);
}

int main(int argc, char **argv)
{
    bool show_command = false;
    bool get_command = false;
    bool set_command = false;

    char *file_name = NULL;
    char *frame_name = NULL;
    char *value = NULL;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--show") == 0)
        {
            show_command = true;
            continue;
        }
        if (argv[i][2] == 'f')
        {
            file_name = strpbrk(argv[i], "=") + 1;
        }
        if (argv[i][2] == 'g')
        {
            get_command = true;
            frame_name = strpbrk(argv[i], "=") + 1;
            continue;
        }
        if (argv[i][2] == 's')
        {
            set_command = true;
            frame_name = strpbrk(argv[i], "=") + 1;
            continue;
        }
        if (argv[i][2] == 'v')
        {
            value = strpbrk(argv[i], "=") + 1;
            continue;
        }
    }
    if (show_command)
    {
        printf("REQUEST RESULT:\n");
        show(file_name);
    }
    else if (get_command)
    {
        printf("REQUEST RESULT:\n");
        get(file_name, frame_name);
    }
    else if (set_command)
    {
        set(file_name, frame_name, value);
        printf("EDITED\n");
    }

    return 0;
}