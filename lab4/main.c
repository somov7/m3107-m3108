#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>
#include <uchar.h>


#define ID3_MAX_SIZE 128

typedef struct ID3TAG {
    char signature[3]; // "TAG"
    char name[30]; // имя песни
    char artist[30]; // исполнитль
    char album[30]; // альбом
    char year[4]; // год
    char description[30]; // описание
    unsigned short number_in_album[1];  //номер трека в альбоме
    unsigned short null_byte[1]; // нулевой байт, проверка номера в альбоме
    char genre; // Жанр
} ID3TAG;

/* Возвращает кол-во байт на 128 меньше от общего размера файла */
long file_offset(FILE* fp) {
    fseek(fp, 0, SEEK_END); // Устанавливает индикатор на 0 байт от  конца
    //ftell(fp) - кол-во байт от начала файла
    return ftell(fp) - ID3_MAX_SIZE; // на 128 от конца
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Rus");
    FILE* Filein = NULL; // Изначальный mp3 файл
    FILE* Fileout = NULL; // Изменённый mp3 файл
    char* mp3_name; // Название mp3 файла
    char* tag; // Название тэга
    char* val; // Испрльзуется в --value=val
    ID3TAG* TAGS = NULL;

    /* Если 11 первых символов argv[1] == 11 первым символам '--filepath=',
       то strncmp() вернёт 0 */
    if (!strncmp(argv[1],"--filepath=",11)) {
        // Ссылка на следующий эл-т после '='
        mp3_name = strpbrk(argv[1], "=") + 1;
        // Если файл не открывается то вывести сообщение об ошибке
        if ((Filein = fopen(mp3_name, "rb")) == NULL) {
            printf("Failed to open file %s for reading.\n", mp3_name);
            return 0;
        }
    }
    else {
        printf("Команда --filepath= не распознана.\n");
        return 0;
    }


    // Здесь временно будет храниться вся информация из mp3 файла
    char* temp = (char*) malloc(sizeof(char) * ID3_MAX_SIZE);
    // Весь массив (все байты) заполняются 0
    memset(temp, 0, ID3_MAX_SIZE);
    // Устанавливает индикатор на 0 байт от  конца
    fseek(Filein, file_offset(Filein), SEEK_SET);
    //
    fread(temp, 1, ID3_MAX_SIZE, Filein);
    // Копируем тэги
    TAGS = (ID3TAG*)(temp);


    /* --show */
    if(!strcmp("--show", argv[2])) {
        printf("\nSong Name:     %s\n",TAGS->name);
        printf("Artist:        %s\n",TAGS->artist);
        printf("Album:             %s\n",TAGS->album);
        printf("Year:                %s\n",TAGS->year);
        printf("Description:           %s\n",TAGS->description);
        printf("Number in album:           %s\n",TAGS->number_in_album);
        printf("null_byte:           %s\n",TAGS->null_byte);
        printf("Genre:               %d\n\n",TAGS->genre);
    }
        /* --get */
    else if (!strncmp("--get=", argv[2], 6)) {
        tag = strpbrk(argv[2], "=") + 1;
        if ( !strcmp(tag, "name") )
            printf("Song Name:     %s\n",TAGS->name);

        else if ( !strcmp(tag, "artist") )
            printf("Artist:        %s\n",TAGS->artist);

        else if ( !strcmp("album", tag) )
            printf("Album:             %s\n",TAGS->album);

        else if ( !strcmp("year", tag) )
            printf("Year:                %s\n",TAGS->year);

        else if ( !strcmp("description", tag))
            printf("Description:           %s\n",TAGS->description);

        else if ( !strcmp("number_in_album", tag) )
            printf("Number in album:               %d\n\n",TAGS->number_in_album);

        else if ( !strcmp("null_byte", tag) )
            printf("null_byte:               %d\n\n",TAGS->null_byte);

        else if ( !strcmp("genre", tag) )
            printf("Genre:               %d\n\n",TAGS->genre);

        else {
            printf("Unknown TAG.\n");
            return 0;
        }
    }
        /* --set= */
    else if (!strncmp("--set=", argv[2], 6)) {
        // Название тэга
        tag = strpbrk(argv[2], "=") + 1;
        if (!strncmp("--value=", argv[3], 8)) {
            val = strpbrk(argv[3], "=") + 1;
            if ( !strcmp(tag, "name") )
                memcpy(TAGS->name, val, 30);

            else if ( !strcmp(tag, "artist") )
                memcpy(TAGS->artist, val, 30);

            else if ( !strcmp(tag, "album") )
                memcpy(TAGS->album, val, 30);

            else if ( !strcmp(tag, "year") )
                memcpy(TAGS->year, val, 4);

            else if ( !strcmp(tag, "description") )
                memcpy(TAGS->description, val, 30);

            else if ( !strcmp("number_in_album", tag) )
                memcpy(TAGS->number_in_album, val, 1);

            else if ( !strcmp("null_byte", tag) )
                memcpy(TAGS->null_byte, val, 1);

            else if ( !strcmp("genre", tag) )
                memcpy(TAGS->genre, val, 1);

        }
        else {
            printf("Error with command --value=\n");
            return 0;
        }
    }

    else {
        printf("Unknown command: %s\n", argv[2]);
        return 0;
    }

    // Закрытие изначального mp3 файла
    fclose(Filein);

    if ((Fileout = fopen(mp3_name, "wb+")) == NULL) {
        printf("Failed to open file %s  for writing.\n", mp3_name);
        return 0;
    }

    // В temp2[] будут временно хранится изменённые тэги
    char* temp2 =(char*) malloc(sizeof(char) * ID3_MAX_SIZE);
    // Весь массив (все байты) заполняются 0
    memset(temp2, 0, ID3_MAX_SIZE);
    // Записываем изменённые тэги
    temp2 = (char*)TAGS;
    fseek(Fileout, file_offset(Fileout), SEEK_SET);
    // Записываем 128 байт из temp2[] 1 раз в Fileout
    fwrite(temp2, 1, ID3_MAX_SIZE, Fileout);
    free(temp2);
    fclose(Fileout);

    return 0;
}