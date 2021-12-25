#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define file_name_size 25

void Exist(char file_name[file_name_size]){
    FILE *file;
    if (file = fopen(file_name, "rb") == NULL) {
        printf("File %s doesn't exist\n", file_name);
    }
    else
        printf("File %s exists\n", file_name);
    return;
}
void arc_create(int argc, char *argv[], char *archive_name)
{
    FILE *arc_file;
    int file_quantity = 0;
    /* Записываем названия  */
    char file_names[argc - 4][file_name_size];                  //первое число - кол-во файлов
    for(int i = 0; i < argc - 4; i++)
    {
        for(int j = 0; j < file_name_size; j++)
        {
            file_names[i][j] = 0;
        }
    }
    while (argv[file_quantity + 4] != NULL) {
            int InsideLengthCounter = 0;
            for (int j = 0; j < strlen(argv[file_quantity + 4]); j++) {
                file_names[file_quantity][j] = argv[file_quantity + 4][j];
                InsideLengthCounter = j + 1;
            }
            file_names[file_quantity][InsideLengthCounter] = '\0';
        //printf("Argument Value: %s\n", file_names[file_quantity]);
        file_quantity++;
    }
    // printf ("file_quantity: %d\n", file_quantity);
    for (int j = 0; j < file_quantity; j++) {
        Exist(file_names[j]);
    }
    /* Соединяем файлы воедино  */
    /* Отдельно записываем размер, название и информацию  */
    FILE *Buffer = NULL;
    printf("\nMaking an %s Archive File...\n", archive_name);
    arc_file = fopen(archive_name, "w+b");
    fclose(arc_file);
    arc_file = fopen(archive_name, "wb");
    fwrite(&file_quantity, sizeof(int), 1, arc_file);
    for (int j = 0; j < file_quantity; j++) {
        /* Запись размера и названия в конец файла  */
        long int Buffer_size = 0;
        Buffer = fopen(file_names[j], "r+b");
        fseek(Buffer, 0, SEEK_END);
        Buffer_size = ftell(Buffer);
        //  printf("%s byte quantity: %li\n", file_names[j], Buffer_size);
        fwrite(&Buffer_size, sizeof(long int), 1, arc_file); // Размер (побайтово)
        fwrite (&file_names[j], sizeof(char), file_name_size, arc_file); // Название (25 байт). Итого вышло 29.
        Buffer_size = ftell(Buffer);
        // printf("The new %s byte quantity: %li\n", file_names[j], Buffer_size);
        // printf("File Inside Quantity %d\n", file_quantity);
        /* запись в архивный файл  */
        fseek(Buffer, 0, SEEK_SET);
        char ch;
        for (int g = 0; g < Buffer_size; g++) {
            ch = fgetc(Buffer);
            //  printf("%c", ch);
            fwrite(&ch, sizeof(char), 1, arc_file);
        }
        printf("\n");
        printf("File %s was successfully archived in %s!\n", file_names[j], archive_name);
        fclose(Buffer);
    }
}
void arc_extract(char *archive_name)
{
    FILE *arc_file;
    FILE *Buffer;
    arc_file = fopen(archive_name, "rb");
    fseek(arc_file, 0, SEEK_SET);
    int FileQuantity = 0;
    fread(&FileQuantity, sizeof(int), 1, arc_file);
    // printf("%d\n", FileQuantity);
    for (int g = 0; g < FileQuantity; g++) {
        long  int arc_fileSize;
        char arc_fileName[file_name_size];
        fread(&arc_fileSize, sizeof(long int), 1, arc_file);
        fread(&arc_fileName, sizeof(char), file_name_size, arc_file);
        Buffer = fopen(arc_fileName, "wb");
        fseek(Buffer, 0, SEEK_SET);
        char Symbol;
        for (int g = 0; g < arc_fileSize; g++){
            Symbol = fgetc(arc_file);
            fwrite(&Symbol, sizeof(char), 1, Buffer);
        }
        printf("File %s has been successfully extracted!\n\n", arc_fileName);
        continue;
    }
}
void arc_list(char *archive_name)
{
    FILE *arc_file;
    arc_file = fopen(archive_name, "rb");
    fseek(arc_file, 0, SEEK_SET);
    int FileQuantity = 0;
    fread(&FileQuantity, sizeof(int), 1, arc_file);
    printf("Quantity: %d\n", FileQuantity);
    printf("\n");
    for (int g = 0; g < FileQuantity; g++) {
        long  int arc_fileSize;
        char arc_fileName[file_name_size];
        fread(&arc_fileSize, sizeof(long int), 1, arc_file);
        printf("Size of the file No. %d: %li\n", g + 1, arc_fileSize);
        fread(&arc_fileName, sizeof(char), file_name_size, arc_file);
        printf("Name of the file: %s\n", arc_fileName);
        char Symbol;
        for (int g = 0; g < arc_fileSize; g++){
            Symbol = fgetc(arc_file);
        }
        printf("\n");
        continue;
    }
}

int main(int argc, char *argv[]) {
    char *archive_name;
    archive_name = argv[2];
    for (int i = 3; argv[i] != NULL; i++) {
        if (strcmp(argv[i], "--create") == 0)
        {
            arc_create(argc, argv, archive_name);
        }
        if (strncmp(argv[i], "--extract", 9) == 0) {
            arc_extract(archive_name);
        }
        if (strncmp(argv[i], "--list", 6) == 0) {
            arc_list(archive_name);
        }
    }
    return 0;
}