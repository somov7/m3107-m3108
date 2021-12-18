#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define ul unsigned long
ul files_counter = 0;
ul prev_file_pointer = 0;
void bitwise_shift(ul start_number, unsigned char *total_array) {

    for (int i = 0; i < 4; i++) {
        total_array[i] = (start_number << (8 * i)) >> 24;
    }
}
ul back_bitwise_shift (unsigned char *input_array) {
    ul total_res = 0;
    for (int i = 0; i < 4; i++) {

        total_res = (total_res << 8) + input_array[i];
    }
    return total_res;
}
void archive_add (char *adding_file, unsigned char *archive, ul filename_size) {
    files_counter++;
    FILE *add_file, *arc_file;
    ul current_file_end = 0;
    add_file = fopen(adding_file, "rb");
    arc_file = fopen(archive, "ab");
    fseek(add_file, 0, SEEK_END);
    current_file_end = ftell(add_file);
    fseek(add_file, 0, SEEK_SET);
    unsigned char temp_data[4096], bin_size_of_file[4], bin_counter_files[4], bin_filename_size[4], c;
    fseek(arc_file, prev_file_pointer, SEEK_SET);
    bitwise_shift(filename_size, bin_filename_size);
    fwrite(bin_filename_size, sizeof(char), 4, arc_file);
    for (int i = 0; i < filename_size; i++) {
        fwrite(&adding_file[i], sizeof(char), 1, arc_file);
    }
    bitwise_shift(current_file_end, bin_size_of_file);
    fwrite(bin_size_of_file, sizeof(char), 4, arc_file);
    
    bitwise_shift(files_counter, bin_counter_files);
    fwrite(bin_counter_files, sizeof(char), 4, arc_file);
    
    fseek(arc_file, 12 + filename_size, SEEK_SET);
    while (ftell(add_file) < current_file_end) {
        if (current_file_end - ftell(add_file) >= 4096) {
            fread(temp_data, sizeof(char), 4096, add_file);
            fwrite(temp_data, sizeof(char), 4096, arc_file);
        }
        else {
            c = fgetc(add_file);
            fputc(c, arc_file);
        }
    }
    fclose(add_file);
    remove(adding_file);
    fclose(arc_file);
}

void list (unsigned char *archive_file) {
    FILE *arc_file;
    arc_file = fopen(archive_file, "rb");
    fseek(arc_file, 0, SEEK_END);
    ul end_of_archive = ftell(arc_file);
    fseek(arc_file, 0, SEEK_SET);
    unsigned char arr_filename_size[4], file_size_arr[4];
    printf("Ваши файлы:\n");
    while (ftell(arc_file) < end_of_archive) {
        fread(arr_filename_size, sizeof(char), 4, arc_file);
        ul filename_size = back_bitwise_shift(arr_filename_size);
        unsigned char output_filename[filename_size];
        fread(output_filename, sizeof(char), filename_size, arc_file);
        
        for (int j = 0; j < filename_size; j++) {
            printf("%c", output_filename[j]);
        }
        printf("\n");
        for (int i = 0; i < 4; i++) {
            file_size_arr[i] = fgetc(arc_file);
        }
        ul file_size_ul = back_bitwise_shift(file_size_arr);
        
//        if (ftell(arc_file) <= end_of_archive) {
        fseek(arc_file, 4 + file_size_ul, SEEK_CUR);
        
    }
}
void extract (unsigned char *archive_file) {
    FILE *arc_file, *temp_file;
    
    arc_file = fopen(archive_file, "rb");

    fseek(arc_file, 0, SEEK_END);
    ul end_of_archive = ftell(arc_file);
    fseek(arc_file, 0, SEEK_SET);
    unsigned char arr_filename_size[4], file_size_arr[4], c, temp_data[4096];
    int y = ftell(arc_file);
    while (ftell(arc_file) < end_of_archive) {
        ul data_rewrited_counter;
        data_rewrited_counter = 0;
        fread(arr_filename_size, sizeof(char), 4, arc_file);
        ul filename_size = back_bitwise_shift(arr_filename_size);
        unsigned char output_filename[filename_size + 1];
        fread(output_filename, sizeof(char), filename_size, arc_file);
        output_filename[filename_size] = '\0';
        temp_file = fopen(output_filename, "ab");
        
        fread(file_size_arr, sizeof(char), 4, arc_file);
        ul file_size_ul = back_bitwise_shift(file_size_arr);
        fseek(arc_file, 4, SEEK_CUR);
        while (data_rewrited_counter < file_size_ul) {
            if (file_size_ul - data_rewrited_counter >= 4096) {
                fread(temp_data, sizeof(char), 4096, arc_file);
                fwrite(temp_data, sizeof(char), 4096, temp_file);
                data_rewrited_counter += 4096;
            }
            else {
                c = fgetc(arc_file);
                fputc(c, temp_file);
                data_rewrited_counter++;
            }
        }
        fclose(temp_file);
    }
    remove(archive_file);
}
int main(int argc, const char * argv[]) {
    FILE *fin;
    ul current_file_end = 0;
    if (argc > 3) {
        if (strcmp(argv[3], "--create") == 0) {
            for (int i = 4; i < argc; i++) {
                fin = fopen(argv[i], "rb");

                fseek(fin, 0, SEEK_END);
                prev_file_pointer = current_file_end;
                current_file_end = ftell(fin);
                fseek(fin, 0, SEEK_SET);
                archive_add(argv[i], argv[2], strlen(argv[i]));
            }
            
        } else if (strcmp(argv[3], "--extract") == 0) {
            extract(argv[2]);

        } else if (strcmp(argv[3], "--list") == 0) {
            list(argv[2]);
            
        } else {
            fprintf(stderr, "Проверьте параметры");
            return EXIT_FAILURE;
        }
    } else {
        fprintf(stderr, "Проверьте параметры");
        return EXIT_FAILURE;
    }
    return 0;
}
                
