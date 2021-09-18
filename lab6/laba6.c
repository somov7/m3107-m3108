#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char* argv[])
{
    if (argc <= 3)
    {
        printf("Correct usage: \n--file <name.arc> --extract\n--file <name.arc> --list \n--file <name.arc> --create <file1> <file2> ... <filen>\n");
        return 1;
    }
    char** FILES = NULL; //файлы что нужно запаковать
    char* FILE_NAME = NULL; //название архива
    int extract = 0, list = 0, create = 0, k = 0;
    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "--file"))
        {
            if (i+1<argc) 
            {
                FILE_NAME = (char*)malloc(strlen(argv[i+1])+1);
                strcpy(FILE_NAME, argv[i+1]);
            }
            else 
            {
                printf("Wrong format");
                return 1;
            }
        }
        else if (!strcmp(argv[i], "--extract"))
        {
            extract = 1;
        }
        else if (!strcmp(argv[i], "--list"))
        {
            list = 1;
        }
        else if (!strcmp(argv[i], "--create"))
        {
            create = 1;
        }
        else if(!strstr(argv[i], ".arc")) //если не аргумент --create --list --extract и не название архива, что читаем название свободных файлов
        {
            if (FILES == NULL) FILES = (char**)malloc((argc-4)*sizeof(char*)); //если еще не создали, создаим массив строк с названиями файлов
            int len = strlen(argv[i]);
            FILES[k] = (char*)malloc(len);
            strcpy(FILES[k], argv[i]);
            printf("file %s\n", FILES[k]);
            k++;
        }
    }
    FILE* t;
    FILE* arc;
    if (create) //если не распаковываем и не смотрим список файлов в архиве, то будем писать
    {
    arc = fopen(FILE_NAME, "wb");
    if (!arc)
    {
        printf("Failed to open file, check name");
        return 1;
    }
    int file_count = argc-4;
    long long* files_size = (long long*)malloc(file_count*sizeof(long long)); //массив с размерами файлов
    long long total_strlen = 0; //всего байтов на строки с названиями файлов
    for (int i = 0; i < file_count; i++)
    {
        t = fopen(FILES[i], "rb");
        if (!t)
        {
            printf("Can't open file");
            return 1;
        }
        fseek(t, 0L, SEEK_END);
        files_size[i] = ftell(t); //размер файла
        total_strlen += strlen(FILES[i]); //размер строки названия
        fclose(t);
    }
    long long total_meta_size = 12 + total_strlen + 12 * file_count; 
    //Структура
    //METASIZE, FILECOUNT (METASIZE - размер всей структуры метаданных - 8 байтов) (FILECOUNT - количество файлов - 4 байта)
    //SIZEOFFILE1 STRLENOFFILE1 STR SIZEOFFILE2 STRLENOFFILE2 STR (Размера файла i-го(8 байт), длина строки i-го(4байта), строка)
    fwrite(&total_meta_size, sizeof(long long), 1, arc); //пишем размер всех метаданных
    fwrite(&file_count, sizeof(int), 1, arc); //пишем количество файлов
    for (int i = 0; i < file_count; i++)
    {
        fwrite(&files_size[i], sizeof(long long), 1, arc); //пишем размер i-го файла
        int file_len = strlen(FILES[i]);
        fwrite(&file_len, sizeof(int), 1, arc), //пишем размер строки i-го файла
        fwrite(FILES[i], 1, file_len, arc); //пишем строку названия i-го файла после
    }
    //Пишем содержимое файлов сразу после
    for (int i = 0; i < file_count; i++)
    {
        t = fopen(FILES[i], "rb");
        if (!t)
        {
            printf("Can't open file");
            return 1;
        }
        char* buf = (char*)malloc(files_size[i]);
        fread(buf, 1, files_size[i], t); //прочитали все байты файла
        fwrite(buf, 1, files_size[i], arc); //записали все байты файла
        free(buf);
    }
    fclose(arc);
    }
    if (list || extract)
    {
    arc = fopen(FILE_NAME, "rb");
    if (!arc)
    {
        printf("Can't open file");
        return 1;
    }
    long long FULL_META_SIZE;
    int FILE_COUNT;
    fread(&FULL_META_SIZE, 1, sizeof(long long), arc); //читаем размер структуры метаданных
    fread(&FILE_COUNT, 1, sizeof(int), arc); //читаем количество файлов в архиве
    long long* FILE_SIZES = (long long*)malloc(FILE_COUNT*sizeof(long long));
    int* FILE_LENS = (int*)malloc(FILE_COUNT*sizeof(int));
    char** FILE_STRS = (char**)malloc(FILE_COUNT*sizeof(char*));
    for (int i = 0; i < FILE_COUNT; i++)
    {
        fread(&FILE_SIZES[i], sizeof(long long), 1, arc); //читаем размер i-го файла
        fread(&FILE_LENS[i], sizeof(int), 1, arc); //читаем длину строки i-го файла
        FILE_STRS[i] = (char*)malloc(FILE_LENS[i]+1);
        fread(FILE_STRS[i], 1, FILE_LENS[i], arc); //читаем строку i-го файла
        FILE_STRS[i][FILE_LENS[i]] = '\0';
        printf("File #%d size: %d strlen: %d str: %s\n", i, FILE_SIZES[i], FILE_LENS[i], FILE_STRS[i]);

    }
    if (extract)
    {
        fseek(arc, FULL_META_SIZE, 0); //Пропускаем метаданные
        //Читаем байты архива и распаковываем в новые файлы
        for (int i = 0; i < FILE_COUNT; i++)
        {
            t = fopen(FILE_STRS[i], "wb"); //создаем новый файл по i-й строке с названием
            if (t == NULL)
            {
                printf("Can't open file");
                return 1;
            }
            char* buf = (char*)malloc(FILE_SIZES[i]);
            fread(buf, 1, FILE_SIZES[i], arc); //прочитали байты файла
            fwrite(buf, 1, FILE_SIZES[i], t); //записали байты файла в соответственно созданный файл
            fclose(t);
        }
    }
    for (int i = 0; i < FILE_COUNT; i++)
        free(FILE_STRS[i]);
    free(FILE_STRS);
    for (int i = 0; i < FILE_COUNT; i++)
        free(FILES[i]);
    free(FILES);
    }
    return 0;
}