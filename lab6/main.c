#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void my_itoa(unsigned long long file_size, unsigned char *push_file_size){
    for(int i = 0; i < 4; i++){
        push_file_size[i] = (file_size << (8 * i)) >> 24;
    }
}

unsigned int my_atoi(unsigned char smth[4]){
    unsigned int size = 0;
    for(int i = 0; i < 4; i++){
        size += smth[3 - i] << (8 * i);
    }
    return size;
}

void create_arc(FILE *archive, char *file_name){
    FILE *file_ptr = fopen(file_name, "rb");

    if (archive == NULL) {
        fprintf(stderr, "%s", "ERROR: the file does not exist 1");
        return;
    }
    if (file_ptr == NULL) {
        fprintf(stderr, "%s", "ERROR: the file does not exist 2");
        return;
    }


    fseek(file_ptr, 0, SEEK_END);
    unsigned long long file_size = ftell(file_ptr);
    fseek(file_ptr, 0, SEEK_SET);

    char push_file_size[4];
    char push_file_name[4];

    my_itoa(file_size, push_file_size);
    fwrite(push_file_size, sizeof(char), 4, archive);
    
    unsigned int name_len = strlen(file_name);
    my_itoa(name_len, push_file_name);
    fwrite(push_file_name, sizeof(char), 4, archive);
    fwrite(file_name, sizeof(char), name_len, archive);

    char tmp;
    for(int i = 0; i < file_size; i++){
        tmp = fgetc(file_ptr);
        fputc(tmp,archive);
    }
}

void extract_arc(FILE *archive){
    fseek(archive, 0, SEEK_END);
    unsigned long long archive_size = ftell(archive);
    fseek(archive, 0, SEEK_SET);

    char file_size_b[4];
    char file_name_b[4];

    while(ftell(archive) < archive_size){
        fread(file_size_b, sizeof(char), 4, archive);
        fread(file_name_b, sizeof(char), 4, archive);
        unsigned long long file_size = my_atoi(file_size_b);
        unsigned long long name_size = my_atoi(file_name_b);

        char *name = calloc(sizeof(char), name_size + 1);
        fread(name, sizeof(char), name_size, archive);
        name[name_size] = '\0';
        char *file_name = name;
        FILE *file_ptr = fopen(file_name, "wb");
        char tmp;
        for(int i = 0; i < file_size; i++){
            tmp = fgetc(archive);
            fputc(tmp, file_ptr);
        }
    }
}

void list_arc(FILE *archive){
    fseek(archive, 0, SEEK_END);
    unsigned long long file_size = ftell(archive);
    fseek(archive, 0, SEEK_SET);

    while(ftell(archive) < file_size){
        char file_size_b[4];
        fread(file_size_b, sizeof(char), 4, archive);
        unsigned long long file_size = my_atoi(file_size_b);

        char file_name_b[4];
        fread(file_name_b, sizeof(char), 4, archive);
        unsigned long long name_size = my_atoi(file_name_b);

        char *name = calloc(sizeof(char), name_size + 1);
        fread(name, sizeof(char), name_size, archive);
        name[name_size] = '\0';
        for(int i = 0; i < name_size; i++){
            printf("%c", name[i]);
        }
        printf("\n");
        for(int i = 0; i < file_size; i++){
            fgetc(archive);
        }
    }
}

int main(int argc, char **argv){
    if(argc < 4){
        printf("Error, not enough arguments");
        return 1;
    }
    else{
        char *archive_name = argv[2];
        char *command = argv[3];
        FILE *archive;

        if(strcmp(command,"--create") == 0){
            int i = 4;
            while(i < argc){
                archive = fopen(archive_name, "ab");
                create_arc(archive, argv[i]);
                fclose(archive);
                i++;
            }
        }
        else if(strcmp(command,"--extract") == 0){
            archive = fopen(archive_name, "rb");
            extract_arc(archive);
            fclose(archive);
        }
        else if(strcmp(command,"--list") == 0){
            archive = fopen(archive_name, "rb");
            list_arc(archive);
            fclose(archive);
        }
    }

    return 0;
}

// arc --file data.arc --create ...