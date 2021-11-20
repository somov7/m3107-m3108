#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include <locale.h>
unsigned int rever_byt(unsigned int n)
{
    return ((n >> 24) & 0x000000ff) | 
    ((n >> 8) & 0x0000ff00) | 
    ((n << 8) & 0x00ff0000) |
    ((n << 24) & 0xff000000);
}

typedef union tag_tagsect
{
    char buf[12]; struct {
        unsigned short  empty; 
        unsigned char   version[3]; 
        unsigned char   v1; 
        unsigned char   v2; 
        unsigned char   flags; 
        unsigned int    size;
} data;
} tagsect;

typedef union tag_framesect
{
    char buf[10]; 
    struct {
        unsigned char name[4]; 
        unsigned int size; 
        unsigned short flags;
    } data;
} framesect;

void show(char* songname)
{
    FILE* file;
    file = fopen(songname, "rb"); 
    if (file == NULL)
    {
        printf("Reading error\n"); 
        exit(404);
    }
    fseek(file, 0, SEEK_SET); 
    tagsect tag_sect;
    fread(tag_sect.buf + 2, sizeof(char), 10, file); 
    unsigned int tag_size = rever_byt(tag_sect.data.size);
    printf("%sv%d.%d\n", tag_sect.data.version, tag_sect.data.v1, tag_sect.data.v2); 
    while (ftell(file) < tag_size + 10)
    {
        framesect frame_sect; 
        fread(frame_sect.buf, sizeof(char), 10, file); 
        if (frame_sect.data.name[0] == 0)
            break;
        printf("%s: ", frame_sect.data.name);
        unsigned int frame_size = rever_byt(frame_sect.data.size); 
        unsigned char* frame_data;
        frame_data = malloc(sizeof(char) * frame_size); 
        fread(frame_data, sizeof(char), frame_size, file); 
        unsigned int i;
        for (i = 0; i < frame_size; ++i)
        {
            printf("%c", frame_data[i]);
        }
        printf("\n"); 
        free(frame_data);
    }
    fclose(file);
}



void copy_file(FILE* input, FILE* output)
{
    int c;
    while ((c = getc(input)) != EOF) 
        putc(c, output);
}

void set(char* songname, char frame_name[4], char* frame_value)
{

    FILE* file;
    file = fopen(songname, "rb"); 
    if (file == NULL)
    {
        printf("Reading error\n"); 
        exit(404);
    }
    tagsect tag_sect;
    fread(tag_sect.buf + 2, sizeof(char), 10, file); 
    unsigned int tag_size = rever_byt(tag_sect.data.size); 
    unsigned int old_frame_pos = 0;
    unsigned int old_frame_size = 0; 
    while (ftell(file) < tag_size + 10)
    {
        framesect frame_secter; 
        fread(frame_secter.buf, sizeof(char), 10, file);
        unsigned int frameSize = rever_byt(frame_secter.data.size); 
        if (strcmp(frame_secter.data.name, frame_name) == 0)
        {
            old_frame_pos = ftell(file) - 10; 
            old_frame_size = frameSize; 
            break;
        }
        fseek(file, frameSize, SEEK_CUR);
    }

    unsigned int value_size = strlen(frame_value);
    unsigned int new_tag_size = tag_size - old_frame_size + value_size + 10 * (old_frame_pos != 0);
    if (old_frame_pos == 0)
    {
        old_frame_pos = ftell(file);
    }
    if (value_size == 0)
    {
        new_tag_size -= 10;
    }
    FILE* file_copy;
    file_copy = fopen("tmp.mp3", "wb"); 
    fseek(file, 0, SEEK_SET);
    fseek(file_copy, 0, SEEK_SET); 
    copy_file(file, file_copy); 
    fclose(file);
    fclose(file_copy);
    file_copy = fopen("tmp.mp3", "rb"); 
    file = fopen(songname, "wb");
    tag_sect.data.size = rever_byt(new_tag_size); 
    fwrite(tag_sect.buf + 2, sizeof(char), 10, file);
    fseek(file_copy, 10, SEEK_SET); 
    unsigned int i;
    for (i = 0; i < old_frame_pos - 10; ++i)
    {
        int c;
        c = getc(file_copy); 
        putc(c, file);
    }
    if (value_size > 0)
    {
        framesect frame_sect; 
        unsigned int i;
        for (i = 0; i < 4; ++i)
        {
            frame_sect.data.name[i] = frame_name[i];
        }
        frame_sect.data.size = rever_byt(value_size); 
        frame_sect.data.flags = 0; 
        fwrite(frame_sect.buf, sizeof(char), 10, file);
    }

    fwrite(frame_value, sizeof(char), value_size, file);
    fseek(file_copy, old_frame_pos + 10 + old_frame_size, SEEK_SET); 
    for (i = ftell(file); i < new_tag_size; ++i)
    {
        unsigned short int c; 
        c = getc(file_copy); 
        putc(c, file);
    }
    printf("New value%s: %s\n", frame_name, frame_value); 
    copy_file(file_copy, file);
    fclose(file); 
    fclose(file_copy); 
    remove("tmp.mp3");
}

void get(char* songname, char frame_name[4])
{
    FILE* file;
    file = fopen(songname, "rb"); 
    if (file == NULL)
    {
        printf("Reading error\n"); 
        exit(404);
    }
    tagsect tag_sect;
    fread(tag_sect.buf + 2, sizeof(char), 10, file); 
    unsigned int tag_size = rever_byt(tag_sect.data.size); 
    while (ftell(file) < tag_size + 10)
    {
        framesect frame_sect; 
        fread(frame_sect.buf, sizeof(char), 10, file);
        unsigned int frame_size = rever_byt(frame_sect.data.size); 
        if (strcmp(frame_sect.data.name, frame_name) == 0)
        {
            printf("%s: ", frame_sect.data.name); 
            unsigned char* frame_data;
            frame_data = malloc(sizeof(char) * frame_size); 
            fread(frame_data, sizeof(char), frame_size, file); 
            unsigned int i;
            for (i = 0; i < frame_size; ++i)
            {
                printf("%c", frame_data[i]);
            }
            printf("\n"); 
            free(frame_data); 
            fclose(file); 
            return;
        }

        fseek(file, frame_size, SEEK_CUR);
    }
    printf("Value error %s!", frame_name);
    fclose(file);
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Rus"); 
    char* songname;
    char* frame_name; 
    char* value;
    int j;
    for (j = 1; j < argc; j++) {
        if (argv[j][2] == 's' && argv[j][3] == 'h') 
        {   
            show(songname);
            continue;
        }
        if (argv[j][2] == 'f')
        {
            songname = strpbrk(argv[j], "=") + 1;
        }
        if (argv[j][2] == 'g') 
        { 
            frame_name = strpbrk(argv[j], "=") + 1; 
            get(songname, frame_name);
            continue;
        }
        if (argv[j][2] == 's') 
        { 
            frame_name = strpbrk(argv[j], "=") + 1; 
            continue;
        }

        if (argv[j][2] == 'v') 
        {
            value = strpbrk(argv[j], "=") + 1;
            set(songname, frame_name, value); 
            continue;
        }
    }
    return 0;
}
