#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    // header 10 bytes:
    // signature (3 bytes)
    // version (2 bytes)
    // flags (1 byte)
    // size (4 bytes 7-bit values)
    unsigned char data[10];
    unsigned char signature[4];
    unsigned char version[2];
    unsigned char flags;
    unsigned int size;
} ID3Header;

typedef struct {
    // header 10 bytes:
    // identifier (3 bytes in ID3v2.2 or 4 bytes in ID2v2.3, ID3v2.4)
    // size (4 bytes)
    // flags (2 bytes)
    unsigned char data[10];
    unsigned char ident[5];
    unsigned int size;
    unsigned char flags[2];
    unsigned char *value;
} FrameHeader;

void print_id3_header(ID3Header *h)
{
    printf("ID3 header data:");
    for(int i = 0; i < 10; i++)
        printf("%02x ", h->data[i]);
    printf(" signature=%s version=%d.%d flags=%02x size=0x%08x=%u\n", h->signature, h->version[0], h->version[1], h->flags, h->size, h->size);
}

void print_frame_header(FrameHeader *fh)
{
    printf("Frame header data:");
    for(int i = 0; i < 10; i++)
        printf("%02x ", fh->data[i]);
    printf(" ident=%s size=0x%08x=%u flags=%02x-%02x\n", fh->ident, fh->size, fh->size, fh->flags[0], fh->flags[1]);
}

int read_id3_header(FILE *file, ID3Header *h)
{ 
    memset(h, 0, sizeof(ID3Header));

    if (fread(h->data, 10, 1, file) != 1)
    {
        printf("can't read file");
        return -1;
    }

    h->signature[0] = h->data[0];
    h->signature[1] = h->data[1];
    h->signature[2] = h->data[2];
    h->signature[3] = 0;
    h->version[0] = h->data[3];
    h->version[1] = h->data[4];
    h->flags = h->data[5];
    h->size = ((h->data[6] & 0x7f) << 21) | ((h->data[7] & 0x7f) << 14) | ((h->data[8] & 0x7f) << 7) | (h->data[9] & 0x7f);

    if ((h->data[0] != 'I') || (h->data[1] != 'D') || (h->data[2] != '3'))
    {
        printf("not ID3 header\n");
        print_id3_header(h);
        return -1;
    }
    //print_id3_header(&h);
    return 0;
}

void set_id3_size(ID3Header *h, int size)
{
    h->size = size;
    h->data[4] = (unsigned char)((h->size >> 21) & 0x0000007f);
    h->data[5] = (unsigned char)((h->size >> 14) & 0x0000007f);
    h->data[6] = (unsigned char)((h->size >> 7) & 0x0000007f);
    h->data[7] = (unsigned char)(h->size & 0x0000007f);
}

void set_frame_value(FrameHeader *fh, char *value)
{
    // free old value
    if (fh->value != NULL)
    {
        free(fh->value);
        fh->value = NULL;
    }

    if (value == NULL)
        return;

    // set size
    fh->size = strlen(value)+1;
    fh->data[4] = (unsigned char)((fh->size >> 21) & 0x0000007f);
    fh->data[5] = (unsigned char)((fh->size >> 14) & 0x0000007f);
    fh->data[6] = (unsigned char)((fh->size >> 7) & 0x0000007f);
    fh->data[7] = (unsigned char)(fh->size & 0x0000007f);

    // copy value
    fh->value = malloc(fh->size);
    fh->value[0] = 0;
    memcpy(fh->value+1, value, fh->size-1);
}

int read_frame(FILE *file, FrameHeader *fh, int version)
{
    memset(fh, 0, sizeof(FrameHeader));  

    if (fread(fh->data, 10, 1, file) != 1)
    {
        printf("can't read file");
        return -1;
    }

    fh->ident[0] = fh->data[0];
    fh->ident[1] = fh->data[1];
    fh->ident[2] = fh->data[2];
    if (version == 2)
    {
        fh->ident[3] = 0;        
    }
    else if ((version == 3) || (version == 4))
    {       
        fh->ident[3] = fh->data[3];
        fh->ident[4] = 0;
    }
    fh->size = ((fh->data[4] & 0x7f) << 21) | ((fh->data[5] & 0x7f) << 14) | ((fh->data[6] & 0x7f) << 7) | (fh->data[7] & 0x7f);
    fh->flags[0] = fh->data[8];
    fh->flags[1] = fh->data[9];
    fh->value = NULL;

    //print_frame_header(fh);

    if (fh->size > 0)
    {
        fh->value = malloc(fh->size);
        if (fh->value == NULL)
        {
            printf("can't allocate memory");
            return -1;
        }
        memset(fh->value, 0, fh->size);
        if (fread(fh->value, fh->size, 1, file) != 1)
        {
            printf("can't read file");
            set_frame_value(fh, NULL);
            return -1;
        }
    }
    return 0;
}

int create_frame(FrameHeader *fh, int version, char *prop_name, char *value)
{
    memset(fh, 0, sizeof(FrameHeader));  

    fh->data[0] = prop_name[0];
    fh->data[1] = prop_name[1];
    fh->data[2] = prop_name[2];
    if ((version == 3) || (version == 4))
        fh->data[3] = prop_name[3];

    fh->ident[0] = fh->data[0];
    fh->ident[1] = fh->data[1];
    fh->ident[2] = fh->data[2];
    if (version == 2)
    {
        fh->ident[3] = 0;        
    }
    else if ((version == 3) || (version == 4))
    {       
        fh->ident[3] = fh->data[3];
        fh->ident[4] = 0;
    }
    fh->value = NULL;

    set_frame_value(fh, value);
    return 0;
}

void print_frame(FrameHeader *fh)
{
    printf("prop=%s size=%d value=", fh->ident, fh->size);
    for(int i = 0; i < fh->size; i++)
    {
        if (isprint(fh->value[i]))
            printf("%c", fh->value[i]);
        else
            printf("[%02x]", fh->value[i]);
    }
    printf("\n");
}

int get_file_size(char* file_name, long* file_size)
{
    FILE *f = fopen(file_name, "rb");

    if(f == NULL)
    {
        printf("can't open file %s\n", file_name);
        return -1;
    }

    // go to end of file
    if (fseek(f, 0, SEEK_END) != 0)
    {
        printf("read error\n");
        fclose(f);
        return -1;
    }

    // get file size
    *file_size = ftell(f);

    fclose(f);
    return 0;
}

typedef struct List {
    struct List *prev;
    struct List *next;
    FrameHeader fh;
} List;

List *first = NULL;
List *last = NULL;

int add_last(FrameHeader fh)
{
    List *list = malloc(sizeof(List));
    if (list == NULL)
        return -1;

    list->prev = last;
    list->next = NULL;
    list->fh = fh;

    if (first == NULL)
        first = list;
    if (last != NULL)
        last->next = list;
    last = list;

    return 0;
}

int del_first()
{
    List *list = first;

    if (first != NULL)
        first = first->next;
    if (first != NULL)
        first->prev = NULL;
    if (last == list)
        last = NULL;
    if (list != NULL)
    {
        set_frame_value(&list->fh, NULL);
        free(list);
    }
    return 0;
}

void list_clean()
{
    while(first != NULL)
        del_first();
}

int set_prop(char *filepath, char *prop_name, char *value)
{
    printf("filepath=%s prop_name=%s value=%s\n", filepath, prop_name, value);

    ID3Header h;
    FrameHeader fh;
    long file_size = 0;

    if (get_file_size(filepath, &file_size) != 0)
        return -1;

    FILE *file = fopen(filepath, "r+b");
 
    if (read_id3_header(file, &h) != 0)
    {
        fclose(file);
        return -1;
    }

    // read all frames
    long end_frames = ftell(file) + h.size;
    while(ftell(file) < end_frames)
    {
        if (read_frame(file, &fh, h.version[0]) != 0)
        {
            list_clean();
            fclose(file);
            return -1;
        }
        if (fh.ident[0] == 0)
            break;

        add_last(fh);
    }

    int found = 0;
    List *p = first;
    while (p != NULL)
    {
        // prop_name is found, change value
        if (strcmp(p->fh.ident, prop_name) == 0)
        {
            printf("### found: ");
            print_frame(&p->fh);
            printf("\n");

            set_frame_value(&p->fh, value);
            found = 1;

            printf("### changed: ");
            print_frame(&p->fh);
            printf("\n");
            break;
        }
        p = p->next;
    }
    if (found == 0)
    {
        // prop_name if not found, add new frame
        printf("creating new frame\n");
        create_frame(&fh, h.version[0], prop_name, value);
        add_last(fh);
    }

    // get new frames size
    unsigned int new_size = 0;
    p = first;
    while (p != NULL)
    {
        new_size += 10 + p->fh.size;
        p = p->next;
    }

    // check that frames size fits to id3 header body size
    if (h.size < new_size)
    {
        printf("new value does not fit frames size=%d new=%d\n", h.size, new_size);

        // go to file data start
        if (fseek(file, 10+h.size, SEEK_SET) != 0)
        {
            printf("fseek error\n");
            list_clean();
            fclose(file);
            return -1;
        }

        // allocate memory for file data
        long data_size = file_size-10-h.size;
        char *data = malloc(data_size);
        if (data != NULL)
        {
            printf("can't allocate memory size=%ld\n", data_size);
            list_clean();
            fclose(file);
            return -1;
        }

        // read file data
        if (fread(data, data_size, 1, file) != 1)
        {
            printf("data read error\n");
            free(data);
            list_clean();
            fclose(file);
            return -1;
        }

        // go to file start
        if (fseek(file, 0, SEEK_SET) != 0)
        {
            printf("fseek error\n");
            free(data);
            list_clean();
            fclose(file);
            return -1;
        }

        // write id3 header
        set_id3_size(&h, new_size); // change frames size
        if (fwrite(h.data, 10, 1, file) != 1)
        {
            printf("id3 header write error\n");
            free(data);
            list_clean();
            fclose(file);
            return -1;
        }
    
        // write all frames
        p = first;
        while (p != NULL)
        {
            if (fwrite(p->fh.data, 10, 1, file) != 1)
            {
                printf("frame header write error\n");
                free(data);
                list_clean();
                fclose(file);
                return -1;
            }
            if(fwrite(p->fh.value, p->fh.size, 1, file) != 1)
            {
                printf("frame value write error\n");
                free(data);
                list_clean();
                fclose(file);
                return -1;
            }
            p = p->next;
        }

        // write file data
        if (fwrite(data, data_size, 1, file) != 1)
        {
            printf("data write error\n");
            free(data);
            list_clean();
            fclose(file);
            return -1;
        }

        free(data);
    }
    else
    {
        // go to frames start
        if (fseek(file, 10, SEEK_SET) != 0)
        {
            printf("fseek error\n");
            list_clean();
            fclose(file);
            return -1;
        }

        // clean frames
        char *x = malloc(h.size);
        memset(x, 0, h.size);
        fwrite(x, h.size, 1, file);

        // go to frames start
        if (fseek(file, 10, SEEK_SET) != 0)
        {
            printf("fseek error\n");
            list_clean();
            fclose(file);
            return -1;
        }

        // write all frames
        p = first;
        while (p != NULL)
        {
            fwrite(p->fh.data, 10, 1, file);
            fwrite(p->fh.value, p->fh.size, 1, file);
            p = p->next;
        }
    }

    list_clean();
    fclose(file);
    return 0;
}

int get_prop(char *filepath, char *prop_name)
{
    ID3Header h;
    FrameHeader fh;

    FILE *file = fopen(filepath, "rb");
 
    if (read_id3_header(file, &h) != 0)
    {
        fclose(file);
        return -1;
    }

    long end_frames = ftell(file) + h.size;
    while(ftell(file) < end_frames)
    {
        if (read_frame(file, &fh, h.version[0]) != 0)
            break;
        if (fh.ident[0] == 0)
            break;
        if (strcmp(fh.ident, prop_name) == 0)
        {
            print_frame(&fh);
            set_frame_value(&fh, NULL);
            break;
        }
        set_frame_value(&fh, NULL);
    }

    fclose(file);
    return 0;
}

int show(char *filepath)
{
    ID3Header h;
    FrameHeader fh;

    FILE *file = fopen(filepath, "rb");
 
    if (read_id3_header(file, &h) != 0)
    {
        fclose(file);
        return -1;
    }

    long end_frames = ftell(file) + h.size;
    while(ftell(file) < end_frames)
    {
        if (read_frame(file, &fh, h.version[0]) != 0)
            break;
        if (fh.ident[0] == 0)
            break;
        print_frame(&fh);
        set_frame_value(&fh, NULL);
    }

    fclose(file);
    return 0;
}

int arg_check(char *argv, char *name)
{
    return strncmp(argv, name, strlen(name)) == 0;
}

char* arg_val(char *argv)
{
    return strchr(argv,'=')+1;
}

// --filepath=Song.mp3 --show
// --filepath=Song.mp3 --get=TIT2
// --filepath=Song.mp3 --set=COMM --value=Test

int main(int argc, char *argv[])
{
    if ((argc == 3) && arg_check(argv[1], "--filepath=") && arg_check(argv[2], "--show"))
        show(arg_val(argv[1]));
    else if ((argc == 3) && arg_check(argv[1], "--filepath=") && arg_check(argv[2], "--get="))
        get_prop(arg_val(argv[1]), arg_val(argv[2]));
    else if ((argc == 4) && arg_check(argv[1], "--filepath=") && arg_check(argv[2], "--set=") && arg_check(argv[3], "--value="))
        set_prop(arg_val(argv[1]), arg_val(argv[2]), arg_val(argv[3]));
    else
        printf("Invalid args\n");

    return 0;
}
