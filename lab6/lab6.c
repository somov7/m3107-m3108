#include <stdio.h>
#include <string.h>
void create(char* file_name, int number_of_files, char *files[]) 
{   FILE *file;
    file = fopen(file_name, "w");
    unsigned char file_name_size;
    unsigned int body_size;
    int i, j;
    for (i = 0; i < number_of_files; i++) {
        file_name_size = strlen(files[i]);
        fputc(file_name_size, file);
        int file_name_size_int = (int) file_name_size;  
        for (j = 0; j < file_name_size_int; j++)
            fputc(files[i][j], file);
        FILE *archive;
        archive = fopen(files[i], "r");
        fseek(archive, 0, SEEK_END);
        body_size = ftell(archive);
        fseek(archive, 0, SEEK_SET);
        fwrite(&body_size, sizeof(unsigned int), 1, file);
        for (j = 0; j < body_size; j++)
            fputc(getc(archive), file);  }
}
void extract(char* file_name) {
    FILE *file;
    file = fopen(file_name, "r");
    fseek(file, 0, SEEK_END);
    unsigned int end_of_file = ftell(file);
    fseek(file, 0, SEEK_SET);
    int i;
    while (ftell(file) < end_of_file) {
        unsigned int size = getc(file);
        char filename[size];
        for (i = 0; i < size; i++)
            filename[i] = getc(file);
        fread(&size, sizeof(unsigned int), 1, file);  FILE *new_file;
        new_file = fopen(filename, "w");
        for (i = 0; i < size; i++)
            fputc(getc(file), new_file);
        fclose(new_file);
    }
}
void list(char* file_name) {
    FILE *file;
    file = fopen(file_name, "r");
    fseek(file, 0, SEEK_END);
    unsigned int end_of_file = ftell(file);
    fseek(file, 0, SEEK_SET);
    int i;
    while (ftell(file) < end_of_file) {
        unsigned int size = getc(file);
        char filename[size];
        for (i = 0; i < size; i++)
            filename[i] = getc(file);
        printf("%s\n", filename);
        fread(&size, sizeof(unsigned int), 1, file);  fseek(file, size, SEEK_CUR);
    }
}
int main(int argc, char *argv[]) {
    char file_name[20];
    strcpy(file_name, argv[3]);
    if (!(strcmp(argv[4], "--create"))) {
        int number_of_files = argc - 5;
        create(file_name, number_of_files, &argv[5]);  
    } 
    else if (!(strcmp(argv[4], "--extract"))) 
    {  
        extract(file_name);
    } 
    else {
        list(file_name);

    }
    return 0;
}