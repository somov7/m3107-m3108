#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>


int sizeofn(int n){
    int v =  0;
    while (n > 9)
    {
        v++;
        n = n / 10;
    }
    v++;
    return v;
}
char* conversetochar(int n){
    char* c = (char *)malloc(10 * sizeof(char));
    int v = 0;
    while (n > 9)
    {
        c[v] = (n % 10) + '0';
        v++;
        n = n / 10;
    }
    c[v++] = n + '0';
    c[v] = '\0';
    char t;
    for (int i = 0; i < v / 2; i++)
    {
        t = c[i];
        c[i] = c[v - 1 - i];
        c[v - 1 - i] = t;
    }
    return c;
}
void extractarc(FILE *archive) {
    FILE *file_out;
    fseek(archive, 0, SEEK_END);
    unsigned long long end = ftell(archive);
    fseek(archive, 0, SEEK_SET);
    char temp;
    int size_of_name = 0;
    unsigned long long file_in_size = 0;
    while (ftell(archive) < end) {
        file_in_size = 0;
        size_of_name = 0;
        while(1){
            temp = fgetc(archive);
            if (temp == ' ')
                break;
            file_in_size = file_in_size * 10;
            file_in_size = file_in_size + temp - '0';
        }
        while(1){
            temp = fgetc(archive);
            if (temp == ' ')
                break;
            size_of_name = size_of_name * 10;
            size_of_name = size_of_name + temp - '0';
        }
        char name[size_of_name + 1];
        fgetc(archive);
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
    char push_file_in_name[4];
    char push_file_in_size[4];
    int size_of_name;
    unsigned long long file_in_size;
    while (ftell(archive) < end) {
        file_in_size = 0;
        size_of_name = 0;
        while(1){
            temp = fgetc(archive);
            if (temp == ' ')
                break;
            file_in_size = file_in_size * 10;
            file_in_size = file_in_size + temp - '0';
        }
        while(1){
            temp = fgetc(archive);
            if (temp == ' ')
                break;
            size_of_name = size_of_name * 10;
            size_of_name = size_of_name + temp - '0';
        }
        fgetc(archive);
        char name[size_of_name];
        fread(name, sizeof(char), size_of_name, archive);
        for (int i = 0; i < size_of_name; i++) {
            printf("%c", name[i]);
        }
        printf("\n");
        fseek(archive,file_in_size,SEEK_CUR);
        
    }
}

void createarc(FILE *archive, char *file_in) {
    FILE *file = fopen(file_in, "rb");
    unsigned long long file_in_size;
    char temp;
    char* push_file_in_size;
    char* push_file_in_name;
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
    push_file_in_size = conversetochar(file_in_size);
    fwrite(push_file_in_size, sizeof(char), sizeofn(file_in_size), archive);
    fputc(' ', archive);
    size_name_of_file = strlen(file_in);
    push_file_in_name = conversetochar(size_name_of_file);
    fwrite(push_file_in_name, sizeof(char), sizeofn(size_name_of_file), archive);
    fputc(' ', archive);
    fputc('\n', archive);
    fwrite(file_in, sizeof(char), size_name_of_file, archive);
    for (int i = 0; i < file_in_size; i++) {
        temp = fgetc(file);
        fputc(temp, archive);
    }
}

int main(int argc, char *argv[]) {
    char *arc;
    FILE *archive;
    for (int i = 1; i < argc; i++) {
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