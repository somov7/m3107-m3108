#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>



unsigned int new_size_out(char size[4]) {
    unsigned int file = 0;
    for (int i = 0; i < 4; i++)
        file += size[3 - i] << (8 * i);
    return file;
}

void new_size_in(unsigned int file_in, char *new_) {
    for (int i = 0; i < 4; i++) 
        new_[i] = (file_in << (8 * i)) >> 24;
}

void extractarc(FILE *archive) {
    FILE *file_out;
    fseek(archive, 0, SEEK_END);
    unsigned long long end = ftell(archive);
    fseek(archive, 0, SEEK_SET);
    char temp;
    int checker = 0;
    char push_file_in_name[4];
    char push_file_in_size[4];
    int size_of_name;
    unsigned long long file_in_size;
    while (ftell(archive) < end) {
        fread(push_file_in_size, sizeof(char), 4, archive);
        fread(push_file_in_name, sizeof(char), 4, archive);
        file_in_size = new_size_out(push_file_in_size);
        size_of_name = new_size_out(push_file_in_name);
        char name[size_of_name + 1];
        fread(name, sizeof(char), size_of_name, archive);
        name[size_of_name] = '\0';
        char *name1 = name;
        file_out = fopen(name1, "wb");
        for (int i = 0; i < file_in_size; i++) {
            temp = fgetc(archive);
            fputc(temp,file_out);
        }
    }
}

void listarc(FILE *archive) {
    fseek(archive, 0, SEEK_END);
    unsigned long long end = ftell(archive);
    fseek(archive, 0, SEEK_SET);
    char temp;
    int checker = 0;
    char push_file_in_name[4];
    char push_file_in_size[4];
    int size_of_name;
    unsigned long long file_in_size;
    while (ftell(archive) < end) {
        fread(push_file_in_size, sizeof(char), 4, archive);
        fread(push_file_in_name, sizeof(char), 4, archive);
        file_in_size = new_size_out(push_file_in_size);
        size_of_name = new_size_out(push_file_in_name);
        char name[size_of_name];
        fread(name, sizeof(char), size_of_name, archive);
        for (int i = 0; i < size_of_name; i++) {
            printf("%c", name[i]);
        }
        printf("\n");
        for (int i = 0; i < file_in_size; i++) {
            temp = fgetc(archive);
        }
    }
}

void createarc(FILE *archive, char *file_in) {
    FILE *file = fopen(file_in, "rb");
    unsigned long long file_in_size;
    char temp;
    char push_file_in_size[4];
    char push_file_in_name[4];
    unsigned int size_name_of_file;
    if (archive == NULL) {
        fprintf(stderr, "%s", "ERROR: the file does not exist 1");
        return;
    }
    if (file == NULL) {
        fprintf(stderr, "%s", "ERROR: the file does not exist 2");
        return;
    }
    fseek(file, 0, SEEK_END);
    file_in_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    new_size_in(file_in_size, push_file_in_size);
    fwrite(push_file_in_size, sizeof(char), 4, archive);
    size_name_of_file = strlen(file_in);
    new_size_in(size_name_of_file, push_file_in_name);
    fwrite(push_file_in_name, sizeof(char), 4, archive);
    fwrite(file_in, sizeof(char), size_name_of_file, archive);
    for (int i = 0; i < file_in_size; i++) {
        temp = fgetc(file);
        fputc(temp, archive);
    }
}

int main(int argc, char **argv) {
    char *arc;
    FILE *archive;
    for (int i = 1; i < argc - 1; i++) {
        if (memcmp(argv[i], "--file",6) == 0) {
            arc = argv[i+1];
            i+=2;
        }
        if (memcmp(argv[i], "--create",8) == 0) {
            while (i<argc-1) {
                i++;
                archive = fopen(arc, "ab");
                createarc(archive, argv[i]);
                fclose(archive);
            }
        }
        if (memcmp(argv[i], "--extract",9) == 0) {
            archive = fopen(arc, "rb");
            extractarc(archive);
            fclose(archive);
        }
        if (memcmp(argv[i], "--list",6) == 0) {
            archive = fopen(arc, "rb");
            listarc(archive);
            fclose(archive);
        }    
    }
    return 0;
} 
