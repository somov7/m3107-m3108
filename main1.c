#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILEPOS 4

unsigned long long get_file_size(FILE *ptr) {
    fseek(ptr, 0, SEEK_END);
    unsigned long long output = ftell(ptr);
    fseek(ptr, 0, SEEK_SET);
    return output;
}

void w_bytes(FILE *ptr, unsigned long long number) {
    unsigned char temp = 0;
    for (int i = 0; i < 8; i++) {
        temp |= number >> ((7 - i) * 8);
        fputc(temp, ptr);
        temp = 0;
    }
}
//main header - file amount
//fileheader - filesize(8 bytes), filename_len - unsigned char, filename - char array, file itself-binary;
//big endian format

unsigned long long read_bytes(FILE *ptr, int amount) //works only on unsigned
{
    unsigned long long output = 0;
    unsigned char *buffer = calloc(amount + 1, sizeof(unsigned char));
    for (int i = 0; i < amount; i++) {
        buffer[i] = fgetc(ptr);
    }
    for (int j = (amount - 1); j >= 0; j--) {
        output |= (buffer[j] << (8 * (7 - j)));
    }
    free(buffer);
    return output;
}


void archive_a_file(FILE *archive_ptr, char *file_path) {
    FILE *read_ptr = fopen(file_path, "rb");
    if (read_ptr != NULL) {
        unsigned long long file_size = get_file_size(read_ptr); //size parse
        unsigned char temp = 0;
        w_bytes(archive_ptr, file_size);
        /*for (int i = 0; i < 8; i++) {
            temp |= file_size >> ((7 - i) * 8);
            fputc(temp, archive_ptr);
            temp = 0;
        }*/
        char *seeker = NULL;
        char *file_name = strstr(file_path, "/");

        unsigned char str_len; //header parse
        if (file_name != NULL) {
            while ((seeker = strstr(file_name, "/")) != NULL) {
                file_name = seeker + 1;
            }
            str_len = strlen(file_name);
            fputc(str_len, archive_ptr);
            fputs(file_name, archive_ptr);
        } else {
            str_len = strlen(file_path);
            fputc(str_len, archive_ptr);
            fputs(file_path, archive_ptr);
        }
        for (int i = 0; i < file_size; i++) {//file parse
            temp = fgetc(read_ptr);
            fputc(temp, archive_ptr);
        }
    } else {
        printf("%s was not found at the designated directory, file archiving was aborted\n", file_path);
    }
    fclose(read_ptr);
}

void create_archive(char *file_path, char **argv, int argc, int fvector_index) {
    char override;
    FILE *exists = fopen(file_path, "rb");
    if (exists == NULL) {
        override = 'Y';
    } else {
        printf("Do you want to override %s? Y/N:", file_path);
        scanf("%c", &override);
        getchar();
    }
    if (override == 'Y') {
        FILE *archive_ptr = fopen(file_path, "wb");
        w_bytes(archive_ptr, argc - fvector_index);
        //fputc((argc - fvector_index), archive_ptr);
        for (int i = fvector_index; i < argc; i++) {
            printf("%s\n", argv[i]);
            archive_a_file(archive_ptr, argv[i]);
        }
        fclose(archive_ptr);
    } else if (override != 'N') {
        printf("Invalid argument input. The archive was not created, all previous files remain intact\n");
    }
    fclose(exists);
}

void list(char *archive_name) {
    FILE *read_ptr = fopen(archive_name, "rb");
    if (read_ptr != NULL) {
        unsigned long file_number = read_bytes(read_ptr, 8);
        for (int i = 0; i < file_number; i++) {
            unsigned long long file_len = read_bytes(read_ptr, 8);
            unsigned char file_name = fgetc(read_ptr);
            for (int j = 0; j < file_name; j++) {
                unsigned char c = fgetc(read_ptr);
                printf("%c", c);
            }
            printf("\n");
            fseek(read_ptr, file_len, SEEK_CUR);
        }
        fclose(read_ptr);
    } else {
        printf("%s was not found at the designated directory\n", archive_name);
    }
}

void extract_a_file(char *archive_name, char *file_name) {
    FILE *read_ptr = fopen(archive_name, "rb");

    if (read_ptr != NULL) {
        unsigned int file_num = read_bytes(read_ptr, 8);
        char *path = calloc(strlen(file_name) + 1, sizeof(unsigned char)); //path to file buffer
        char *path_to_w = calloc(strlen(archive_name), strlen(file_name) + 1); //path to file archive + file_W

        for (int j = 0; j < strlen(file_name); j++) {
            path[j] = file_name[j];
        }

        char *seeker = NULL;
        char *archive_path = strstr(archive_name, "/");
        if (archive_path != NULL) {
            while ((seeker = strstr(archive_path, "/")) != NULL) {
                archive_path = seeker + 1;
            }
        }
        archive_path--;
        memcpy(path_to_w, archive_name, archive_path - archive_name + 1);
        strcat(path_to_w, file_name);

        for (int i = 0; i < file_num; i++) {
            long long temp = 0;
            unsigned long long file_size = read_bytes(read_ptr,8);
            unsigned char file_name_len = fgetc(read_ptr);
            if (file_name_len == strlen(file_name))
            {
                for (unsigned long long i = 0; i < file_name_len; i++)
                {
                    unsigned char a = fgetc(read_ptr);
                    temp += file_name[i] - a;
                }
                if (temp == 0)
                {
                    FILE* w_ptr = fopen(path_to_w, "wb");
                    for (unsigned long long j = 0; j < file_size; j++)
                    {
                        fputc(fgetc(read_ptr), w_ptr);
                    }
                    fclose(w_ptr);
                    break;
                }
            }
            else
            {
                temp = 0;
                fseek(read_ptr, file_name_len, SEEK_CUR);
                fseek(read_ptr, file_size, SEEK_CUR);
            }
        }
    } else {
        printf("%s was not found at the designated directory\n", archive_name);
    }
    fclose(read_ptr);
}
    void extract(char *archive_name) {
        FILE *read_ptr = fopen(archive_name, "rb");
        if (read_ptr != NULL) {
            unsigned long file_number = read_bytes(read_ptr, 8);
            for (int i = 0; i < file_number; i++) {
                unsigned long long file_len = read_bytes(read_ptr, 8);//file length
                unsigned char file_name = fgetc(read_ptr); //file name length

                char *path = calloc(file_name + 1, sizeof(unsigned char)); //path to file buffer
                char *path_to_w = calloc(strlen(archive_name), file_name + 1); //path to file archive + file_W

                for (int j = 0; j < file_name; j++) {
                    path[j] = fgetc(read_ptr);
                }

                char *seeker = NULL;
                char *archive_path = strstr(archive_name, "/");
                if (archive_path != NULL) {
                    while ((seeker = strstr(archive_path, "/")) != NULL) {
                        archive_path = seeker + 1;
                    }
                }
                archive_path--;
                memcpy(path_to_w, archive_name, archive_path - archive_name + 1);
                strcat(path_to_w, path);

                FILE *exists = fopen(path_to_w, "rb");
                char override;
                if (exists == NULL) {
                    override = 'Y';
                    fclose(exists);
                } else {
                    printf("Do you want to override the file %s?", path);
                    scanf("%c", &override);
                    getchar();//scanf buffer flush
                    fclose(exists);
                }
                if (override == 'Y') {

                    FILE *file_w = fopen(path_to_w, "wb");
                    unsigned char temp;
                    for (int j = 0; j < file_len; j++) {
                        temp = fgetc(read_ptr);
                        fputc(temp, file_w);
                    }
                    fclose(file_w);
                } else if (override != 'Y') {
                    printf("File remains unchanged.\n");
                    fseek(read_ptr, file_len, SEEK_CUR);
                }
                free(path);
                free(path_to_w);
            }
            fclose(read_ptr);
        } else {
            printf("%s was not found at the designated directory\n", archive_name);
        }
    }


    int main(int argc, char **argv) {
        if (argc >= 3) {
            int fvector_index = argc; //no non initialized vars
            char *archive_name = argv[2];
            char *comm = argv[3];
            if (strcmp(comm, "--create") == 0) {
                fvector_index = FILEPOS;
                create_archive(archive_name, argv, argc, fvector_index);
            } else if (strcmp(comm, "--list") == 0) {
                list(archive_name);
            } else if (strcmp(comm, "--extract") == 0) {
                extract(archive_name);
            }
            else if (strcmp(comm, "--fextract") == 0)
            {
                extract_a_file(archive_name, argv[FILEPOS]);
            }
        } else {
            printf("Insufficient amount of arguments, dunkey says bye-bye.\n");
        }
        printf("Done!");
        return 0;
    }
