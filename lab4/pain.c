#include <stdio.h>
#include <locale.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>


int btoi(uint8_t ch[4], int is_header);
void itob(int x, uint8_t* ch);
int show_func(uint8_t* file_path, uint8_t* prop_name, int set, int* old_frame_size, uint8_t* img_name);
void set_func(uint8_t* file_path, uint8_t* prop_name, uint8_t* prop_val);
void getval(uint8_t* str, uint8_t** target);

typedef struct tagHEADER
{
    uint8_t idv3[3]; 
    uint8_t ver[2]; 
    uint8_t flag;
    uint8_t size[4]; 
} ID3_HEADER;

typedef struct tagFRAME 
{
    uint8_t frameid[4]; 
    uint8_t size[4]; 
    uint8_t flag[2]; 
    uint8_t unicode; 
} ID3_FRAME;

ID3_HEADER header;
ID3_FRAME frame;

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");

    uint8_t* file_path = NULL;
    uint8_t* prop_name = NULL;
    uint8_t* prop_value = NULL;


    if (argc < 3) {
        printf("[-] Enter all arguments!");
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        if (strstr(argv[i], "--filepath")) {
            getval(argv[i], &file_path);
            
        }
        else if (strstr(argv[i], "--value")) {
            getval(argv[i], &prop_value);
        }

        else if (strstr(argv[i], "--get")) {
            getval(argv[i], &prop_name);
            show_func(file_path, prop_name, 0, NULL, NULL);
        }
        
        else if (strstr(argv[i], "--set")) {
            getval(argv[i], &prop_name);
            set_func(file_path, prop_name, prop_value);
        }
        
        else if (strstr(argv[i], "--show")) {
            show_func(file_path, NULL, 0, NULL, NULL);
        }
        else {
            printf("[-] Bad format");
            return 1;
        }
    }

    return 0;
}

int btoi(uint8_t ch[4], int is_header) 
{
    if (header.ver[0] == 4 || is_header)
        return (int)ch[0] << 21 | (int)ch[1] << 14 | (int)ch[2] << 7 | (int)ch[3] << 0;
    return (int)ch[0] << 24 | (int)ch[1] << 16 | (int)ch[2] << 8 | (int)ch[3] << 0;
}

void itob(int x, uint8_t* ch)
{
    int bits = 127;
    if (header.ver[0] == 4) {
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


int show_func(uint8_t* file_path, uint8_t* prop_name, int set, int* old_frame_size, uint8_t* img_name)
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
            write_pos = ftell(f)-11; // прочитали больше чем нужно :///
            break; 
        }
        int size_frame = btoi(frame.size, 0); 
        uint8_t* buf = (uint8_t*)calloc(size_frame,1);
        fread(buf, 1, size_frame-1, f);
        buf[size_frame-1] = 0;
        
        if (prop_name == NULL) {
            printf("pos: %-10d id: %-10s size: %-10d value: ", ftell(f), frame.frameid, size_frame); 
            if (frame.unicode) {
                wprintf(L"%ls\n", buf+2); // для юникод
            }
            else {
                printf("%s\n", buf);
            }
        }
        else if (!strcmp(frame.frameid, prop_name)) {
            if (set == 0) {
                printf("id: %-10s value: ", frame.frameid); 
                if (frame.unicode) {
                    wprintf(L"%ls\n", buf+2);
                }
                else {
                    printf("%s\n", buf);
                }
            }
            else {
                *old_frame_size = size_frame;
                write_pos = ftell(f)-10-size_frame;
                break;
            }
        }
        free(buf);
    }
    fclose(f);
    return write_pos; // удобно чтоб в сэте сразу писать после
}

void set_func(uint8_t* file_path, uint8_t* prop_name, uint8_t* prop_val)
{
    FILE* read = fopen(file_path, "rb");
    FILE* write = fopen("temp", "wb"); 
    if (read == NULL || write == NULL) {
        printf("Can't open/create file");
        exit(1);
    }
    int* old_frame_size = calloc(1, sizeof(int));
    int write_pos = show_func(file_path, prop_name, 1, old_frame_size, NULL);

    int size_diff = strlen(prop_val) - (*old_frame_size-1);
    fread(&header, 1, 10, read); 

    int di3_size= btoi(header.size, 1); // весь размер id3
    di3_size += size_diff; // + баффер
    itob(di3_size, header.size);
    fwrite(&header, 1, 10, write); // записали новый хедер

    uint8_t* buf = (uint8_t*)malloc(write_pos-10);
    
    
    fread(buf, 1, write_pos-10, read); // прочитали все байты до фрейма


    fwrite(buf, 1, write_pos-10, write); //ААААА ЗАПИСЫВАЕМ ФРЕЙМ  Я УСТАЛ ПОЧЕМУ НЕ РАБОТАЕТ

    memcpy(frame.frameid, prop_name, 4);  
    uint8_t frame_size[4];
    itob(strlen(prop_val)+1, frame_size);
    memcpy(frame.size, frame_size, 4);

    fwrite(&frame, 1, 11, write); // записали фрейм
    fwrite(prop_val, 1, strlen(prop_val), write); //записали значение фрейма
    free(buf);
    fseek(read, 0, SEEK_END);
    printf("Old frame size: %d\n", *old_frame_size); 
    printf("ПОЧЕМУ НЕ РАБОТАЕТ!!!"); //??????????????????????



    int read_offset;
    if(old_frame_size == NULL){
        read_offset = write_pos;
    } else{
        read_offset = write_pos + 10 + *old_frame_size;
    }

    int read_size = ftell(read) - read_offset;
    buf = (uint8_t*)malloc(read_size);
    fseek(read, read_offset, SEEK_SET);
    fread(buf, 1, read_size, read);
    fwrite(buf, 1, read_size, write);
    free(buf);

    fclose(read);
    fclose(write);
    remove(file_path);
    rename("temp", file_path);
}


void getval(uint8_t* str, uint8_t** target)
{
    uint8_t* split = strtok(str,"=");
    if (split != NULL)
        split = strtok(NULL, "=");
    if (split == NULL) {
        printf("BAdformat");
        exit(1);
    }
    *target = (uint8_t*)malloc(strlen(split));
    strcpy(*target, split);
}


