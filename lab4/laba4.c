#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <windef.h>

int syncint_encode(int value) //функция кодирования syncsafe интов
{
    int out, mask = 0x7F;

    while (mask ^ 0x7FFFFFFF) {
        out = value & ~mask;
        out <<= 1;
        out |= value & mask;
        mask = ((mask + 1) << 8) - 1;
        value = out;
    }

    return out;
}

int syncint_decode(int value) //функция декодирования syncsafe интов
{
    unsigned int a, b, c, d, result = 0x0;
    a = value & 0xFF;
    b = (value >> 8) & 0xFF;
    c = (value >> 16) & 0xFF;
    d = (value >> 24) & 0xFF;

    result = result | a;
    result = result | (b << 7);
    result = result | (c << 14);
    result = result | (d << 21);

    return result;
}

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
//#pragma pack(pop)

ID3HEADER header;
ID3FRAME frame;

int btoi(char ch[4]) //байты в инт
{
    return syncint_decode(((int)ch[0] << 24) + ((int)ch[1] << 16)  
        + ((int)ch[2] << 8) + ((int)ch[3] << 0));
}

void itob(int x, char* ch)
{
    int t = syncint_encode(x);
    ch[0] = (t >> 24) & ((1 << 8) - 1);
    ch[1] = (t >> 16) & ((1 << 8) - 1);
    ch[2] = (t >> 8) & ((1 << 8) - 1);
    ch[3] = (t >> 0) & ((1 << 8) - 1);
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

    FILE* t;
    if (set) t = fopen("temp.mp3", "wb"); //временный файл для записи, потом удалим старый файл и переименуем этот в название старого
    FILE* f = fopen(filepath, "rb");
    if (!f)
    {
        printf("Can't open file\n");
        return 1;
    }
    fread(&header, 1, 10, f); //читаем хедер 10 байтов
    int k = btoi(header.size); //размер всей структуры ID3 включая хедер
    //printf("size struct: %d\n", k);
    int found_prop = 0;
    int pos;
    int prop_sz_diff = 0;
    char* buf;
    while (fread(&frame, 1, 11, f)) //читаем фрейм 10 байтов + 1 байт перед строкой юникода
    {
        if (frame.frameid[0] == 0) break; //больше нечего читать, т.к. первый символ в названии ид фрейма не может быть равен нулю
        if (ftell(f) >= k) break; //либо если нет паддинга нулевыми байтами, то прочитали больше, чем нужно
        unsigned int sz = btoi(frame.size); //размера значения фрейма (длина строки)
        buf = (char*)malloc(sz); //размер строки включает 1 байт для юникода в начале, поэтому все ок
        fgets(buf, sz, f); //прочитали структуру фрейма включая 1 байт юникода, теперь читаем значение
        //прочитает sz-1 байт, в конец добавит нуль терминатор
        
        if (get && !strcmp(frame.frameid, prop_name)) printf("id: %s value: %s", frame.frameid, buf); //вывод одного фрейма
        if (show) printf("pos: %d id: %s size: %d value: %s\n", ftell(f), frame.frameid, sz, buf); //выводим все фреймы
        free(buf);
        if (set && !strcmp(frame.frameid, prop_name)) //если меняем фрейм и он нашелся в файле
        {
            found_prop = 1;
            prop_sz_diff = strlen(prop_value) - (sz-1); //считаем разницу в длине строки
            char new_header_sz[4]; //далее обновляем размера всей структуры
            char new_frame_sz[4]; //и фрейма
            int oldsz = k;
            int oldframesz = sz;
            k += prop_sz_diff;
            sz += prop_sz_diff;
            itob(k, new_header_sz);
            itob(sz, new_frame_sz);
            memcpy(header.size, new_header_sz, 4); //запишем новые значения в переменные структур
            memcpy(frame.size, new_frame_sz, 4);
            pos = ftell(f); //запомним где остановились читать фреймы, чтобы вернуться к этой позиции и записать во временный файл оставшееся содержимое (начиная со след. фрейма)
            
            fwrite(&header, 1, 10, t); //запишем новый хедер
            fseek(f, 10, 0); //читаем фреймы до нашего
            buf = (char*)malloc(pos-21-(oldframesz-1));
            fread(buf, 1, pos-21-(oldframesz-1), f);
            fwrite(buf, 1, pos-21-(oldframesz-1), t); //пишем фреймы до нашего в новый файл
            fwrite(&frame, 1, 11, t); //пишем новый фрейм
            fwrite(prop_value, 1, strlen(prop_value), t); //пишем новое значение фрейма
            free(buf);
            
            fseek(f, pos, 0);
            char buff[1000];
            while (fread(buff, 1, 1000, f)) //пишем оставшееся содержимое файла после нашего фрейма (?)
                fwrite(buff, 1, 1000, t);
            break;
        }
    }
    if (!found_prop && set) //если меняем фрейм и фрейма с таким ид не нашлось, добавим в конец структуры ID3, изменим размер структуры
    {
        fseek(f, 0, 0);
        fread(&header, 1, 10, f);
        char sz[4];
        int oldsz = btoi(header.size); //размер старого хедера
        oldsz += strlen(prop_value) + 11; //размер нового это + 11 байтов структуры (включая байт юникода) + размер строки
        itob(oldsz, sz); //переведем инт в бигэндиан и закодируем syncsafe
        memcpy(header.size, sz, 4);
        fwrite(&header, 1, 10, t); //пишем размер структуры во временный файл

        while (fread(&frame, 1, 11, f) && frame.frameid[0] != 0) //читаем фреймы пока первый символ ИД фрейма не равен нулю (то есть прочитали все и нарвались на паддинг нулевыми байтами)
        {
            if (ftell(f) >= oldsz) break; //либо если нет паддинга, больше нечего читать
            fwrite(&frame, 1, 11, t); //пишем старые фреймы
            unsigned int sz = btoi(frame.size); //размера значения фрейма (длина строки)
            buf = (char*)malloc(sz); //размер строки включает 1 байт для юникода в начале, поэтому все ок
            fgets(buf, sz, f); //прочитали структуру фрейма включая 1 байт юникода, теперь читаем значение
            fwrite(buf, 1, sz-1, t); //пишем старые значения фреймов минус нулевой байт в строке, который добавляет fgets
            free(buf);
        }
        ID3FRAME tmp;
        itob(strlen(prop_value)+1, sz);
        memcpy(tmp.size, sz, 4);
        memcpy(tmp.frameid, prop_name, 4);
        fwrite(&tmp, 1, 11, t); //добавляем новый фрейм в конец
        fwrite(prop_value, 1, strlen(prop_value), t); //добавляем значение фрейма

        char buff[1000];
        while (fread(buff, 1, 1000, f)) //пишем оставшееся содержимое файла после нашего фрейма (?)
            fwrite(buff, 1, 1000, t);
    }
    if (set) fclose(t);
    fclose(f);
    if (set) //если изменяем фреймы, т.к. записывали во временный файл, удалим старый и переименуем временный в название нашего файла
    {
        remove(filepath);
        rename("temp.mp3", filepath);
    }
    return 0;
}