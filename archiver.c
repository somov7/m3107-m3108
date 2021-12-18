#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#define BufferSize 4096

typedef struct ArcHeaderTag{
    uint64_t name_len;
    char *f_name;
    uint64_t f_size;
} ArcHeader;

void list_files(FILE *file_arc) {
    uint64_t number_of_bytes;
    ArcHeader file_header;
    while(1) {
        // read header
        number_of_bytes = fread(&file_header.name_len, sizeof(file_header.name_len), 1, file_arc);
        if (number_of_bytes == 0) break;
        file_header.f_name = calloc(file_header.name_len + 1, sizeof(char));
        number_of_bytes = fread(file_header.f_name, sizeof (char), file_header.name_len, file_arc);
        if (number_of_bytes == 0) break;
        number_of_bytes = fread(&file_header.f_size, sizeof (file_header.f_size), 1, file_arc);
        if (number_of_bytes == 0) break;
        printf("%s %llu bytes\n", file_header.f_name, file_header.f_size);
        // skip file body to read next header
        fseek(file_arc, file_header.f_size, SEEK_CUR);
        free(file_header.f_name);
    }
}

void add_file_to_arc(char *file_name, FILE *file_arc) {
    char buffer[BufferSize] = {'\0'};
    uint64_t number_of_bytes;
    FILE *cur_file;   // input file

    cur_file = fopen(file_name, "rb");
    ArcHeader file_header;
    if (cur_file != NULL) {
        // put name into header
        file_header.f_name = file_name;

        // put size into header
        fseek(cur_file, 0L, SEEK_END);
        file_header.f_size = ftell(cur_file);
//         fseek(cur_file, 0, SEEK_SET);
        rewind(cur_file);

        printf("adding file %s %llu bytes\n", file_header.f_name, file_header.f_size);

        // write header into arc file
        file_header.name_len = strlen(file_header.f_name);
        fwrite(&file_header.name_len, sizeof (file_header.name_len), 1, file_arc);
        fwrite(file_header.f_name, sizeof (char), file_header.name_len, file_arc);
        fwrite(&file_header.f_size, sizeof (file_header.f_size), 1, file_arc);
        while(1)
        {
            // read max BufferSize bytes per loop
            number_of_bytes = fread(buffer, sizeof(char), sizeof(buffer), cur_file);
            if (number_of_bytes == 0) break;
            fwrite(buffer, sizeof(char), number_of_bytes, file_arc);
        }
    }
    else {
        printf("can't open file %s for reading\n", file_name);
    }
    fclose(cur_file);
}

void extract_files(FILE *file_arc){
    char buffer[BufferSize] = {'\0'};
    uint64_t number_of_bytes;
    uint64_t num_of_bytes_to_read;
    uint64_t need_to_read;

    FILE *file_out;
    ArcHeader file_header;
    while(1) {
        // read header (file name and size)
        number_of_bytes = fread(&file_header.name_len, sizeof(file_header.name_len), 1, file_arc);
        if (number_of_bytes == 0) break;
        file_header.f_name = calloc(file_header.name_len + 1, sizeof(char));
        number_of_bytes = fread(file_header.f_name, sizeof (char), file_header.name_len, file_arc);
        if (number_of_bytes == 0) break;
        number_of_bytes = fread(&file_header.f_size, sizeof (file_header.f_size), 1, file_arc);
        if (number_of_bytes == 0) break;

        printf("extracting %s %llu bytes\n", file_header.f_name, file_header.f_size);

        // create new file from arc
        file_out = fopen(file_header.f_name,"wb");
        if (file_out == NULL) {
            printf("can't open file %s to extract\n", file_header.f_name);
            fseek(file_arc, file_header.f_size, SEEK_CUR);
            continue;
        }
        num_of_bytes_to_read = file_header.f_size;
        // printf("write %d bytes\n", num_of_bytes_to_read);

        while(num_of_bytes_to_read > 0)
        {
            // reading min from BufferSize and bytes_to_read in one loop iteration
            need_to_read = ( num_of_bytes_to_read >= sizeof(buffer)) ? sizeof(buffer) : num_of_bytes_to_read;
            number_of_bytes = fread(buffer,sizeof(char),need_to_read,file_arc);
            if (number_of_bytes == 0 || need_to_read == 0) break;
            fwrite(buffer, sizeof(char), number_of_bytes, file_out);
            num_of_bytes_to_read = num_of_bytes_to_read - need_to_read;
        }
        free(file_header.f_name);
        fclose(file_out);
    }
}

int main(int argc, char *argv[]) {

    if (argc < 4 || ((strcmp(argv[3], "--create") != 0) && argc > 4)){
        printf("wrong number of parameters\n");
        return 1;
    }

    uint16_t creation_flag = 0;
    uint16_t extraction_flag = 0;
    uint16_t list_flag = 0;

    char *arch_name;
    char *file_list[argc];
    uint32_t number_of_files = 0;
    for (int i = 1; i < argc; i++){
//        printf("%s\n", argv[i]);
        if (strcmp(argv[i], "--file") == 0){
            arch_name = argv[++i];
        }
        else if (strcmp(argv[i], "--create") == 0){
            creation_flag = 1;
        }
        else if (strcmp(argv[i], "--extract") == 0){
            extraction_flag = 1;
        }
        else if (strcmp(argv[i], "--list") == 0){
            list_flag = 1;
        }
        else if (creation_flag == 1){
            file_list[number_of_files] = argv[i];
            number_of_files++;
        }
    }

    printf("flags: %d %d %d\n", creation_flag, extraction_flag, list_flag);
    printf("%s\n", arch_name);
//    for (int i = 0; i < number_of_files; i++){
//        printf("%s\n", file_list[i]);
//    }

    FILE *arc_file;
    if (creation_flag){
        arc_file = fopen(arch_name, "wb");
        if (arc_file == NULL){
            printf("can't open file %s\n", arch_name);
            return 1;
        }
        for (int i = 0; i < number_of_files; i++) {
            add_file_to_arc(file_list[i], arc_file);
        }
        fclose(arc_file);
    }
    else if (extraction_flag){
        arc_file = fopen(arch_name, "rb");
        if (arc_file == NULL){
            printf("can't open file %s\n", arch_name);
            return 1;
        }
        extract_files(arc_file);
        fclose(arc_file);
    }
    else if (list_flag){
        arc_file = fopen(arch_name, "rb");
        if (arc_file == NULL){
            printf("can't open file %s\n", arch_name);
            return 1;
        }
        list_files(arc_file);
        fclose(arc_file);
    }

    printf("completed successfully\n");
    return 0;
}
