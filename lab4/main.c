#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>

typedef struct tag_header{
    unsigned char id[3];
    unsigned char version;
    unsigned char subversion;
    unsigned char flags;
    unsigned char size[4];
} tag_header;

typedef struct frame_header{
    unsigned char id[4];
    unsigned char size[4];
    unsigned char flags[2];
    unsigned char a;
} frame_header;

int bytes_to_int(unsigned char *tmp){
    return (int)(tmp[0] << 24 | tmp[1] << 16 | tmp[2] << 8 | tmp[3]);
}

int check_on_number_of_arguments(int argc)
{
    if (argc < 3)
    {
        printf("Error, not enough arguments");
        return -1;
    }
    else if (argc > 4)
    {
        printf("Error, too much arguments");
        return -2;
    }
    else {
        return 0;
    }
}

char *get_argument(char *argv, char *arg){
    arg = strtok(argv, "=");
    arg = strtok(NULL, "=");
    return arg;
}

void show(char *file_path){
    FILE *file = fopen(file_path, "rb");
    if(file == NULL){
        printf("Error, cannot open the file\n");
        return;
    }
    fseek(file, 0, SEEK_SET);
    tag_header tag_header;

    fread(&tag_header, 10 ,sizeof(char), file);
    char header_id[3];
    for(int i = 0; i < 3; i++){
        header_id[i] = tag_header.id[i];
    }
    header_id[3] = '\0';
    if(strcmp(header_id, "ID3") != 0){
        printf("Error, not id3 format");
        return;
    }
    unsigned int file_size = bytes_to_int(tag_header.size);

    while(ftell(file) < file_size){
        frame_header frame_header;
        fread(&frame_header, 11, sizeof(char), file);
        unsigned int frame_size = bytes_to_int(frame_header.size) - 1;
        printf("%s ", frame_header.id);
        char *frame_content = malloc((frame_size+1)*sizeof(char));
        fread(frame_content, frame_size, sizeof(char), file);
        frame_content[frame_size] = '\0';
        printf("%s\n", frame_content);
        free(frame_content);
    }
}

void get(char *file_path, char *prop_name){
    FILE *file = fopen(file_path, "rb");
    if(file == NULL){
        printf("Error, cannot open the file\n");
        return;
    }
    fseek(file,0,SEEK_SET);
    tag_header tag_header;
    fread(&tag_header, 10, sizeof(char), file);
    char header_id[3];
    for(int i = 0; i < 3; i++){
        header_id[i] = tag_header.id[i];
    }
    header_id[3] = '\0';
    if(strcmp(header_id, "ID3") != 0){
        printf("Error, not id3 format");
        return;
    }

    unsigned int file_size = bytes_to_int(tag_header.size);

    while(ftell(file) < file_size){
        frame_header frame_header;
        fread(&frame_header, 11, sizeof(char), file);
        unsigned int frame_size = bytes_to_int(frame_header.size) - 1;
        char frame_name[5];
        for(int i = 0; i < 4; i++){
            frame_name[i] = frame_header.id[i];
        }
        frame_name[5] = '\0';

        char *frame_content = malloc((frame_size+1)*sizeof(char));
        fread(frame_content, frame_size, sizeof(char), file);
        frame_content[frame_size] = '\0';

        if(strcmp(frame_name, prop_name) == 0){
            printf("%s", frame_content);
        }

        free(frame_content);
    }
}

void set(char *file_path, char *prop_name, char *prop_value){
    FILE *file = fopen(file_path, "rb");
    if(file == NULL){
        printf("Error, cannot open the file\n");
        return;
    }
    fseek(file,0,SEEK_SET);
    tag_header tag_header;
    fread(&tag_header, 10, sizeof(char), file);
    char header_id[3];
    for(int i = 0; i < 3; i++){
        header_id[i] = tag_header.id[i];
    }
    header_id[3] = '\0';
    if(strcmp(header_id, "ID3") != 0){
        printf("Error, not id3 format");
        return;
    }

    unsigned int file_size = bytes_to_int(tag_header.size);
    
    
}

int main(int argc, char **argv)
{
    setlocale(LC_ALL, "Russian");

    printf("hello \n");
    char *file_path = NULL;
    int show_flag = 0;
    int get_flag = 0;
    char *prop_name = NULL;
    char *prop_value = NULL;
    int set_flag = 0;


    if(check_on_number_of_arguments(argc) != 0){
        return - 1;
    }

    for(int i = 1; i < argc; i++){
        if(strncmp("--filepath=", argv[i], 11) == 0){
            file_path = get_argument(argv[i], file_path);
            if(file_path == NULL){
                printf("Error, wrong format\n");
                return -1;
            }
            printf("%s\n", file_path);
        }
        else if(strncmp("--get=", argv[i], 6) == 0){
            printf("get= ");
            get_flag = 1;
            prop_name = get_argument(argv[i], prop_name);
            if(prop_name == NULL){
                printf("Error, worng format\n");
                return -1;
            }
            printf("%s\n", prop_name);
        }
        else if(strncmp("--show", argv[i], 6) == 0){
            printf("show\n");
            show_flag = 1;
        }
        else if(strncmp("--set=", argv[i], 6) == 0){
            printf("set= ");
            set_flag = 1;
            prop_name = get_argument(argv[i], prop_name);
            if(prop_name == NULL){
                printf("Error, wrong format\n");
                return -1;
            }
            printf("%s\n", prop_name);
            i++;
            if(i >= argc){
                printf("Error, worng format\n");
                return -1;
            }
            if(strncmp("--value=", argv[i], 8) == 0){
                printf("value= ");
                prop_value = get_argument(argv[i], prop_value);
                if(prop_value == NULL){
                    printf("Error, wrong format\n");
                    return -1;
                }
                printf("%s\n", prop_value);
            }
            else{
                printf("Error, wrong format\n");
                return -1;
            }
        }
    }

    if(show_flag){
        show(file_path);
        return 0;
    }
    if(get_flag){
        get(file_path, prop_name);
        return 0;
    }
    if(set_flag){
        set(file_path, prop_name, prop_value);
        return 0;
    }
    return 0;
}