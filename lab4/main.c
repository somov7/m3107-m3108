#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <stdint.h>

#pragma pack(push,1) //      
typedef struct tagHEADER //  ID3v2.4
{
    uint8_t idv3[3]; //ID3 tag
    uint8_t ver[2]; //Version tag.
    uint8_t flag; //flags
    uint8_t size[4]; //size of entire meta in syncsafe int
} ID3HEADER;

typedef struct tagFRAME //( 10 ,             /)
{
    uint8_t frameid[4]; //frame ID, ALBM, COMM etc
    uint8_t size[4]; //size of tag without frame header
    uint8_t flag[2]; //flags
    uint8_t unicode; //unicode flag right after the frame
} ID3FRAME;
#pragma pack(pop)

ID3HEADER header;
ID3FRAME frame;

int btoi(uint8_t ch[4], int is_header) //  
{
    return (int)ch[0] << 21 | (int)ch[1] << 14 | (int)ch[2] << 7 | (int)ch[3] << 0;
}

void itob(int x, uint8_t* ch, int is_header)
{
    int bits = 127;
    if (header.ver[0] == 4 || is_header) {
        ch[3] = x & bits;
        ch[2] = (x >> 7) & bits;
        ch[1] = (x >> 14) & bits;
        ch[0] = (x >> 21) & bits;
    }
    else {
        ch[3] = x & bits;
        ch[2] = (x >> 8) & bits;
        ch[1] = (x >> 16) & bits;
        ch[0] = (x >> 24) & bits;
    }
}

int readidv3(uint8_t* file_path, uint8_t* prop_name, int set, int* old_frame_size)
{
    FILE* f = fopen(file_path, "rb");
    if (f == NULL) {
        printf("Can't open file");
        exit(1);
    }
    fread(&header, 1, 10, f); 
    int k = btoi(header.size, 1);
    printf("size struct: %d\n", k);
    int write_pos = 0;
    while (fread(&frame, 1, 11, f)) 
    {
        if (frame.frameid[0] == 0 || ftell(f) >= k) { 
            write_pos = ftell(f) - 11; 
            break; 
        }
        int sz = btoi(frame.size, 0); 
        uint8_t* buf = calloc(sz, sizeof(uint8_t));
        fread(buf, 1, sz - 1, f);
        if (prop_name == NULL) {
            printf("pos: %-10d id: %-10s size: %-10d value: ", ftell(f), frame.frameid, sz); 
            if (frame.unicode) {
                wprintf(L"%ls\n", buf + 2);
            }
            else {
                printf("%s\n", buf);
            }
        }
        else if (!strcmp(frame.frameid, prop_name)) {

            if (set == 0) {
                printf("id: %-10s value: ", frame.frameid); 
                if (frame.unicode) {
                    wprintf(L"%ls\n", buf + 2);
                }
                else {
                    printf("%s\n", buf);
                }
                return -1;
            }
            else {
                *old_frame_size = sz;
                write_pos = ftell(f) - 10 - sz;
                break;
            }
        }
        free(buf);
    }
    fclose(f);
    return write_pos;
}

void updateidv3(uint8_t* file_path, uint8_t* prop_name, uint8_t* prop_val)
{
    FILE* read = fopen(file_path, "rb");
    char* temp_file = calloc(1, strlen(file_path) + 5);
    sprintf(temp_file, "%s.temp", file_path);
    FILE* write = fopen(temp_file, "wb"); //  
    if (read == NULL || write == NULL) {
        printf("Can't open/create file");
        exit(1);
    }
    int* old_frame_size = calloc(1, sizeof(int));
    int write_pos = readidv3(file_path, prop_name, 1, old_frame_size);
    int size_diff = strlen(prop_val) - (*old_frame_size - 1);
    fread(&header, 1, 10, read); //  10 
    int k = btoi(header.size, 1); //   ID3  
    k += size_diff; //       
    itob(k, header.size, 1);
    fwrite(&header, 1, 10, write); //  

    uint8_t* buf = (uint8_t*)malloc(write_pos - 10);
    fread(buf, 1, write_pos - 10, read); //    
    fwrite(buf, 1, write_pos - 10, write); //    ,   
    memcpy(frame.frameid, prop_name, 4);
    uint8_t frame_size[4];
    itob(strlen(prop_val) + 1, frame_size, 0);
    memcpy(frame.size, frame_size, 4);

    fwrite(&frame, 1, 11, write); // 
    fwrite(prop_val, 1, strlen(prop_val), write); //  
    free(buf);
    fseek(read, 0, SEEK_END);
    printf("old fr sz: %d\n", *old_frame_size);
    int read_offset = *old_frame_size ? write_pos + 10 + *old_frame_size : write_pos;
    int read_size = ftell(read) - read_offset;
    buf = (uint8_t*)malloc(read_size);
    fseek(read, read_offset, SEEK_SET);
    fread(buf, 1, read_size, read);
    fwrite(buf, 1, read_size, write);
    free(buf);

    fclose(read);
    fclose(write);
    remove(file_path);
    rename(temp_file, file_path);
}


void getval(uint8_t* str, uint8_t** target)
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
    //setlocale(LC_ALL, "Russian");

    uint8_t* file_path = NULL;
    uint8_t* prop_name = NULL;
    uint8_t* prop_value = NULL;
    int show = 0, get = 0, set = 0;

    if (argc < 3) {
        printf("Correct usage: --filepath=<path>\n--show\n--set=<propname> --value=<propvalue>\n--get=<propname>\n");
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        if (strstr(argv[i], "--filepath")) {
            getval(argv[i], &file_path);
            printf("file_path: %s\n", file_path);
        }
        else if (strstr(argv[i], "--set")) {
            set = 1;
            getval(argv[i], &prop_name);
        }
        else if (strstr(argv[i], "--get")) {
            get = 1;
            getval(argv[i], &prop_name);
        }
        else if (strstr(argv[i], "--value")) {
            getval(argv[i], &prop_value);
        }
        else if (!strcmp(argv[i], "--show")) {
            show = 1;
        }
        else {
            printf("Wrong format");
            return 1;
        }
    }
    if (prop_name && strlen(prop_name) != 4) {
        printf("Wrong frame size: %d", strlen(prop_name));
        return 1;
    }

    if (show)
        readidv3(file_path, NULL, 0, NULL);
    else if (get) {
        if (readidv3(file_path, prop_name, 0, NULL) != -1)
            printf("Can't find frame: %s\n", prop_name);
    }
    else if (set)
        updateidv3(file_path, prop_name, prop_value);
    return 0;
}