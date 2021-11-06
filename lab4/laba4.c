#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <windef.h>

#pragma pack(push,1) //чтобы структуры были едиными без паддинга байтов
typedef struct tagHEADER //структура хедера ID3v2.4
{
    char idv3[3]; //ID3 tag
    char ver[2]; //Version tag.
    char flag; //flags
    char size[4]; //size of entire meta in syncsafe int
} ID3HEADER;

typedef struct tagFRAME //(фрейм 10 байтов, но после идет еще один который мол в самой строке для указания юникод/неюникод)
{
    char frameid[4]; //frame ID, ALBM, COMM etc
    char size[4]; //size of tag without frame header
    char flag[2]; //flags
    char unicode; //unicode flag right after the frame
} ID3FRAME;
#pragma pack(pop)

ID3HEADER header;
ID3FRAME frame;

int btoi(char ch[4]) //байты в инт
{
    return ch[0] << 21 | ch[1] << 14 | ch[2] << 7 | ch[3] << 0;
}

void itob(int x, char* ch)
{
    int bits = 127;
    ch[3] = x & bits;
    ch[2] = (x >> 7) & bits;
    ch[1] = (x >> 14) & bits;
    ch[0] = (x >> 21) & bits;
}

char* getval(char* str)
{
    char* split = strtok(str,"=");
    if (split == NULL) return NULL;
    split = strtok(NULL, "=");
    if (split == NULL) return NULL;
    char* val = (char*)malloc(strlen(split));
    strcpy(val, split);
    return val;
}

unsigned int readidv3(char* filepath, char* prop_name, int set, int* old_frame_size)
{
    FILE* f = fopen(filepath, "rb");
    if (f == NULL) {
        printf("Can't open file");
        exit(1);
    }
    fread(&header, 1, 10, f); //читаем хедер 10 байтов
    unsigned int k = btoi(header.size); //размер всей структуры ID3 включая хедер
    printf("size struct: %d\n", k);
    unsigned int write_pos = 0;
    while (fread(&frame, 1, 11, f)) //читаем фрейм 10 байтов + 1 байт перед строкой юникода
    {
        if (frame.frameid[0] == 0 || ftell(f) >= k) {
            write_pos = ftell(f)-11;
            break;
        }
        //больше нечего читать, т.к. первый символ в названии ид фрейма не может быть равен нулю
        //либо если нет паддинга нулевыми байтами, то прочитали больше, чем нужно
        unsigned int sz = btoi(frame.size); //размера значения фрейма (длина строки)
        char* buf = (char*)malloc(sz); //размер строки включает 1 байт для юникода в начале, поэтому все ок
        fgets(buf, sz, f); //прочитали структуру фрейма включая 1 байт юникода, теперь читаем значение
        //прочитает sz-1 байт, в конец добавит нуль терминатор
        if (prop_name == NULL) 
            printf("pos: %d id: %s size: %d value: %s\n", ftell(f), frame.frameid, sz, buf); //выводим все фреймы
        else if (!strcmp(frame.frameid, prop_name)) {
            if (set == 0) printf("id: %s value: %s", frame.frameid, buf); //вывод одного фрейма
            else {
                *old_frame_size = sz;
                write_pos = ftell(f)-10-sz;
                break;
            }
        }
        free(buf);
    }
    fclose(f);
    return write_pos;
}

void updateidv3(char* filepath, char* prop_name, char* prop_val)
{
    FILE* read = fopen(filepath, "rb");
    FILE* write = fopen("temp", "wb"); //временный для записи
    if (read == NULL || write == NULL) {
        printf("Can't open/create file");
        exit(1);
    }
    unsigned int* old_frame_size = calloc(1, sizeof(unsigned int));
    unsigned int write_pos = readidv3(filepath, prop_name, 1, old_frame_size);
    
    int size_diff = strlen(prop_val) - (*old_frame_size-1);
    fread(&header, 1, 10, read); //читаем хедер 10 байтов
    unsigned int k = btoi(header.size); //размер всей структуры ID3 включая хедер
    k += size_diff; //обновили размер структуры в соответствии с новым фреймом
    itob(k, header.size);
    fwrite(&header, 1, 10, write); //записали новый хедер
    char* buf = (char*)malloc(write_pos-10);
    fread(buf, 1, write_pos-10, read); //прочитали все байты до фрейма
    fwrite(buf, 1, write_pos-10, write); //записали все байты до фрейма, теперь пишем фрейм
    memcpy(frame.frameid, prop_name, 4);
    char frame_size[4];
    itob(strlen(prop_val)+1, frame_size);
    memcpy(frame.size, frame_size, 4);
    
    fwrite(&frame, 1, 11, write); //записали фрейм
    fwrite(prop_val, 1, strlen(prop_val), write); //записали значение фрейма
    free(buf);
    fseek(read, 0, SEEK_END);
    printf("old fr sz: %d\n", *old_frame_size);
    unsigned int read_offset = *old_frame_size ? write_pos+10+*old_frame_size : write_pos;
    unsigned int read_size = ftell(read) - read_offset;
    buf = (char*)malloc(read_size);
    fseek(read, read_offset, SEEK_SET);
    fread(buf, 1, read_size, read);
    fwrite(buf, 1, read_size, write);
    free(buf);
    
    fclose(read);
    fclose(write);
    remove(filepath);
    rename("temp", filepath);
}

int main(int argc, char* argv[])
{
    char* filepath = NULL;
    char* prop_name = NULL;
    char* prop_value = NULL;
    int show = 0, get = 0, set = 0;
    if (argc < 3)
    {
        printf("Correct usage: --filepath=<path>\n--show\n--set=<propname> --value=<propvalue>\n--get=<propname>");
        return 1;
    }
    for (int i = 1; i < argc; i++)
    {
        if (strstr(argv[i], "--filepath")) //ищем подстроку т.к. аргумент и значение идет как одна строка --filepath=path.mp3
        {
            if (!(filepath = getval(argv[i])))
            {
                printf("Wrong format");
                return 1;
            }
            printf("filepath: %s\n", filepath);
        }
        else if (strstr(argv[i], "--set"))
        {
            set = 1;
            if (!(prop_name = getval(argv[i])))
            {
                printf("Wrong format");
                return 1;
            }
        }
        else if (strstr(argv[i], "--get"))
        {
            get = 1;
            if (!(prop_name = getval(argv[i])))
            {
                printf("Wrong format");
                return 1;
            }
        }
        else if (strstr(argv[i], "--value"))
        {
           if (!(prop_value = getval(argv[i])))
           {
               printf("Wrong format");
               return 1;
           }
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
    if (show) 
        readidv3(filepath, NULL, 0, NULL);
    else if (get)
        readidv3(filepath, prop_name, 0, NULL);
    else if (set)
        updateidv3(filepath, prop_name, prop_value);
    return 0;
}
