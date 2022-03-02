#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct
{
    unsigned char id[3];
    unsigned char version[2];
    unsigned char flags;
    unsigned char size[4];
} Header;

typedef struct
{
    unsigned char *id;
    unsigned char *size;
    unsigned char *flags;
    unsigned char *content;
} Frame;

void clear(Frame *frame);
long header_size(unsigned char sizearr[]);
long frame_size(unsigned char sizearr[]);
void read_(unsigned char *str, long n);
void get_header();
Frame *get_frame();
Frame *init_frame();
void print(char *p, int n);
void show();
int null(char *p, int n);
void get(const char *q);
Header *header;
long pointer;
FILE *fin;

char *separate(char *str);

void copyf(char buff[], char origin[]);
void set(const char id[], char val[]);

void print_header();
void print_frame(Frame *);

char filepath[100];

int main(int argc, char *argv[]) {
    if (argc > 2) {
        char * p = separate(argv[1]);
        if (strcmp(argv[1], "--filepath") == 0) {
            strcpy(filepath, p);
        } else {
            printf("ERROR: no path");
            return -1;
        }
    }
    fin = fopen(filepath, "r+");

    if (argc < 3)
    {
        fprintf(fin, "ERROR: not enough arguments");
        return -2;
    }
    else if (argc == 3)
    {
        char *val = separate(argv[2]);
        if (strcmp(argv[2], "--show") == 0)
            show();
        else if (strcmp(argv[2], "--get") == 0)
            get(val);
        else {
            fprintf(fin, "ERROR: 1 arg, not supported");
            return -3;
        }
    }
    else if (argc == 4)
    {
        char * id = separate(argv[2]);
        char * value = separate(argv[3]);
        if (strcmp(argv[2], "--set") != 0) {
            printf("ERROR: no tag selected");
            return -4;
        }

        if (strcmp(argv[3], "--value") != 0) {
            printf("ERROR: no value selected");
            return -5;
        }

        set(id, value);
    }
    else
    {
        fprintf(fin, "ERROR: too many arguments");
        return -6;
    }

    fclose(fin);
    return 0;
}

void set(const char id[], char val[]) {   
    Frame * frames[100];
    get_header(); 
    Frame * frame; Frame *q;
    int i = 0;
    int f = 0;
    while (pointer < header_size(header->size) && (frame = get_frame())) {
        if (strcmp(frame->id, id) == 0) {
            q = frame;
            f = 1;
            long fsize = strlen(val) + 1;

            for (int i = 0; i < 4; ++i) {
                frame->size[3 - i] = fsize % (long)pow(2, 8);
                fsize /= pow(2, 8);
            } 
            char * buff = calloc(strlen(val) + 1, sizeof(char));
            buff[0] = 0;
            for (int i = 1; i < strlen(val) + 1; ++i) {
                buff[i] = val[i - 1];
            }
            frame->content = buff;
        }
        frames[i++] = frame;
    }

    if (!f) {
        printf("tag not found");
        return ;
    }

    long size1 = frame_size(q->size);
    long size2 = strlen(val) + 1;
    long dsize = size2 - size1;
    long size = header_size(header->size) + dsize;


    fseek(fin, 0, SEEK_SET);

    for (int i = 0; i < 4; ++i) {
        header->size[3 - i] = size % (long)pow(2, 7);
        size /= pow(2, 7);
    }
    print_header();

    for (int j = 0; j < i; ++j)
        print_frame(frames[j]);
    fprintf(fin, "\0");
 }


Frame *get_frame() {
    Frame *frame = init_frame();
    read_(frame->id, 4);
    if (null(frame->id, 4))
    {
        return 0;
    }
    read_(frame->size, 4);
    read_(frame->flags, 2);

    long size = frame_size(frame->size);

    unsigned char *str = malloc((size + 1L) * sizeof(unsigned char));
    read_(str, size);
    frame->content = str;

    return frame;
}


void show() {
    get_header();
    Frame *frame;
    while (pointer < header_size(header->size) && (frame = get_frame())) {
        print(frame->id, 4);
        printf("  %d  ", frame_size(frame->size));

        print(frame->content, frame_size(frame->size));
        printf("\n");
        clear(frame);
    }
}

void print(char *p, int n)
{
    while (n-- > 0)
    {
        if (*p)
            printf("%c", *p);
        *p++;
    }
}

void get(const char *q) {
    get_header();
    Frame *frame;
    while (pointer < header_size(header->size) && (frame = get_frame())) {
        if (strcmp(frame->id, q) == 0) {
            print(frame->id, 4);
            printf("  %d  ", frame_size(frame->size));

            print(frame->content, frame_size(frame->size));
            printf("\n");
        }
        clear(frame);
    }
}

void get_header()
{
    header = malloc(sizeof(Header));
    int i;
    for (i = 0; i < 3; ++i)
        header->id[i] = fgetc(fin);

    for (i = 0; i < 2; ++i)
        header->version[i] = fgetc(fin);

    header->flags = fgetc(fin);

    for (i = 0; i < 4; ++i)
    {
        header->size[i] = fgetc(fin);
    }
}

void read_(unsigned char *str, long n)
{
    while (n-- > 0)
    {
        *str++ = fgetc(fin);
        ++pointer;
    }
    *str = '\0';
}

void print_header()
{
    int i;
    for (i = 0; i < 3; ++i)
        fprintf(fin, "%c", header->id[i]);

    for (i = 0; i < 2; ++i)
        fprintf(fin, "%c", header->version[i]);

    fprintf(fin, "%c", header->flags);

    for (i = 0; i < 4; ++i)
        fprintf(fin, "%c", header->size[i]);
}

void print_frame(Frame *frame)
{
    int i;
    for (i = 0; i < 4; ++i)
        fprintf(fin, "%c", frame->id[i]);

    for (i = 0; i < 4; ++i)
    {
        fprintf(fin, "%c", frame->size[i]);
    }
    for (i = 0; i < 2; ++i)
        fprintf(fin, "%c", frame->flags[i]);

    for (i = 0; i < frame_size(frame->size); ++i)
        fprintf(fin, "%c", frame->content[i]);
}

int null(char *p, int n)
{
    int f = 1;
    while (n-- > 0)
        if (*p++ != 0)
            f = 0;
    return f;
}


char *separate(char *str)
{ //первая часть - str, вторая - separate
    while (*str != '=' && *str++ != '\0')
        ;

    if (*str == '\0')
        return str;
    else
        *str = '\0';
    return ++str;
}

Frame *init_frame() {   
    Frame *frame = malloc(sizeof(Frame));
    frame->id = malloc(5 * sizeof(unsigned char));
    frame->size = malloc(5 * sizeof(unsigned char));
    frame->flags = malloc(3 * sizeof(unsigned char));
    frame->content = malloc(sizeof(unsigned char *));
    return frame;
}

void clear(Frame *frame)
{
    free(frame->id);
    free(frame->size);
    free(frame->flags);
    free(frame->content);
    free(frame);
}

long header_size(unsigned char sizearr[])
{
    long size = 0;
    for (int i = 0; i < 4; ++i)
    {
        size += ((sizearr[4 - i - 1]) << 7 * i);
    }
    return size;
}

long frame_size(unsigned char arr[])
{
    long size = 0;
    for (int i = 0; i < 4; ++i)
    {
        size += ((arr[4 - i - 1]) << 8 * i);
    }
    return size;
}