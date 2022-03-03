#include <stdio.h>
#include <stdlib.h>
#define uc unsigned char
#define ul unsigned long

void byte_to_bits (FILE *fin, char *result_array) {
    char byte = '\0';
    byte = fgetc(fin);
    for (int i = 0; i < 8; i++) {
        if (((byte >> i) & 1) == 1) result_array[i] = 1;
        else result_array[i] = 0;
    }
}
char bits_to_byte(char *input_array, count_of_bits) {
    char result = '\0';
    char temp = '\0';
    for (int i = 0; i < count_of_bits - 1; i++) {
        temp = input_array[count_of_bits - 1 - i];
//        result = (result || input_array[7 - i]) << 1;
        result = result + temp;
        result = result << 1;
    }
    
    temp = 0 || input_array[7];
    result = result + temp;
    
    return result;
}
unsigned long bitwise_shift_4 (unsigned char basic_size[4]) {
    unsigned long total_num = 0;
    total_num = total_num | basic_size[0];
    total_num = total_num | basic_size[1] << 8;
    total_num = total_num | basic_size[2] << 16;
    total_num = total_num | basic_size[3] << 24;
    return total_num;
}
unsigned long bitwise_shift_2 (unsigned char basic_size[2]) {
    unsigned long total_num = 0;
    total_num = total_num | basic_size[0];
    total_num = total_num | basic_size[1] << 8;
    return total_num;
}
void foo(int x) {
    printf("%d", x);
    
}
typedef struct tag_BM_file_header {
    uc bf_type[2];
    uc bf_size[4];
    uc bf_reserved_1[2];
    uc bf_reserved_2[2];
    uc bf_off_bits[4]; // оффсет, с которого начинаются данные
} tag_BM_file_header;

void game_of_life (int height, int width, char *tmp_result_array[]) {
    
    int real_height = height + 2;
    int real_width = width + 2;
    int counter_death_cells = 0;
    int counter_living_cells = 0;
    char current_situation[real_height][real_width];
//    char tmp_result_array[real_height][real_width];
    
}
int main(int argc, const char * argv[]) {
//    int game_box[10][10];
    FILE *fin, *fout, *input_file, *output_file;
    uc buffer_array[4];
    int game_times = 1;
    if (argc < 5) {
        fprintf(stderr, "Проверьте количество параметров\n");
        return EXIT_FAILURE;
    }
    if (argv[1] == "--input") {
        input_file = argv[2];
    }
    if (argv[3] == "--output") {
        output_file = argv[4];
    }
    if (argv[5] == "max_iter") {
        game_times = argv[6];
    }
                          
                          
    fin = fopen (input_file, "rb");
    fout = fopen (output_file, "wb");
    
//    fin = fopen ("/Users/artemparfenov/Downloads/8_on_8_px.bmp", "rb");
//    fout = fopen ("/Users/artemparfenov/Downloads/square.bmp", "wb");
//
    
    tag_BM_file_header current_header;
    fread(current_header.bf_type, sizeof(char), 2, fin);
    fread(current_header.bf_size, sizeof(char), 4, fin);
    fread(current_header.bf_reserved_1, sizeof(char), 2, fin);
    fread(current_header.bf_reserved_2, sizeof(char), 2, fin);
    fread(current_header.bf_off_bits, sizeof(char), 4, fin);
    ul size_of_file = bitwise_shift_4(current_header.bf_size);
    
    ul data_offSet = bitwise_shift_4(current_header.bf_off_bits);
    fread(buffer_array, sizeof(char), 4, fin);
    fread(buffer_array, sizeof(char), 4, fin);
    ul image_width = bitwise_shift_4(buffer_array);
    int insignificant_bytes;
    
    
    int num_of_bytes_per_string = (image_width / 8) + 1;
    int size_of_block = (num_of_bytes_per_string / 4 + 1) * 4;
    fread(buffer_array, sizeof(char), 4, fin);
    ul image_height = bitwise_shift_4(buffer_array);
    fread(buffer_array, sizeof(char), 2, fin);
    fread(buffer_array, sizeof(char), 2, fin);
    ul j = bitwise_shift_2(buffer_array);
//    printf("%lu уваолцрцаи \n\n", j);
//    char temp_arrr[20];
    fseek(fin, data_offSet, SEEK_SET);
//
//    int service_ptr = ftell(fin);
    fseek(fin, 0, SEEK_SET);
    char service_symbol;
    char array_of_service_information[data_offSet];
    for (int i = 0; i < data_offSet; i++) {
        service_symbol = fgetc(fin);
        fputc(service_symbol, fout);
    }
    fseek(fin, data_offSet, SEEK_SET);
    char array_of_bits[8];
    int important_bits_in_last_byte = image_width % 8;
    int counter_elements = 0;
//    int real_height = image_height + 2;
//    int real_width = image_width + 2;
    char tmp_result_array[image_height][image_width];
    int current_ptr = ftell(fin);
    for (int k = image_height - 1; k >= 0; k--) {
        counter_elements = 0;
        current_ptr = ftell(fin);
        for (int i = 0; i < num_of_bytes_per_string; i++) {
            if (i == num_of_bytes_per_string - 1) {
                byte_to_bits(fin, array_of_bits);
                for (int i = 0; i < important_bits_in_last_byte; i++) {
                    tmp_result_array[k][counter_elements] = array_of_bits[7 - i];
                    counter_elements++;
                }
            }
            else {
                byte_to_bits(fin, array_of_bits);
//                for (int o = 0; o < 8; o++) {
//                    printf("%d ", array_of_bits[o]);
//                }
//                printf("\n");
                for (int d = 0; d < 8; d++) {
                    tmp_result_array[k][counter_elements] = array_of_bits[7 - d];
                    counter_elements++;
                }
            }
            
        }
        fseek(fin, current_ptr + size_of_block, SEEK_SET);
        

    }
    printf("\n");
    for (int t = 0; t < image_height; t++) {
        printf("\n");
        for (int n = 0; n < image_width; n++) {
            printf("%d ", tmp_result_array[t][n]);
        }
    }
    char current_data;

    
    int counter_death_cells = 0;
    int counter_living_cells = 0;
    char current_situation[image_height][image_width];
    for (int n = 0; n < game_times; n++) {
        for (int i = 0; i < image_height; i++) {
            for (int j = 0; j < image_width; j++) {
                counter_death_cells = 0;
                counter_living_cells = 0;
                if ((i == 0) && (j==0)) {
                    if (tmp_result_array[0][1] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    if (tmp_result_array[1][0] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    if (tmp_result_array[1][1] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    if ((tmp_result_array[i][j] == 0) && ((counter_living_cells == 2) || (counter_living_cells == 3))) {
                        current_situation[i][j] = 0;

                    }
                    else if ((tmp_result_array[i][j] == 1) &&(counter_living_cells == 3)) {
                        current_situation[i][j] = 0;
                    }
                    else {
                        current_situation[i][j] = 1;
                    }
                    
                    
                }
                else if ((i == 0) && (j == image_width - 1)) {
                    if (tmp_result_array[0][image_width - 2] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    if (tmp_result_array[1][image_width - 1] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    if (tmp_result_array[1][image_width - 2] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    if ((tmp_result_array[i][j] == 0) && ((counter_living_cells == 2) || (counter_living_cells == 3))) {
                        current_situation[i][j] = 0;

                    }
                    else if ((tmp_result_array[i][j] == 1) &&(counter_living_cells == 3)) {
                        current_situation[i][j] = 0;
                    }
                    else {
                        current_situation[i][j] = 1;
                    }
                }
                else if ((i == image_height - 1) && (j == 0)) {
                    if (tmp_result_array[image_height - 2][0] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    if (tmp_result_array[image_height - 2][1] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    if (tmp_result_array[image_height - 1][1] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    if ((tmp_result_array[i][j] == 0) && ((counter_living_cells == 2) || (counter_living_cells == 3))) {
                        current_situation[i][j] = 0;

                    }
                    else if ((tmp_result_array[i][j] == 1) &&(counter_living_cells == 3)) {
                        current_situation[i][j] = 0;
                    }
                    else {
                        current_situation[i][j] = 1;
                    }
                }
                else if ((i == image_height - 1) && (j == image_width - 1)) {
                    if (tmp_result_array[image_height - 2][image_width - 1] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    if (tmp_result_array[image_height - 2][image_width - 2] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    if (tmp_result_array[image_height - 1][image_width - 2] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    if ((tmp_result_array[i][j] == 0) && ((counter_living_cells == 2) || (counter_living_cells == 3))) {
                        current_situation[i][j] = 0;

                    }
                    else if ((tmp_result_array[i][j] == 1) &&(counter_living_cells == 3)) {
                        current_situation[i][j] = 0;
                    }
                    else {
                        current_situation[i][j] = 1;
                    }
                }
                else if ((i > 0) && (i < image_height - 1) && (j == 0)) {
                    if (tmp_result_array[i - 1][j] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    if (tmp_result_array[i - 1][j + 1] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    if (tmp_result_array[i][j + 1] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    if (tmp_result_array[i + 1][j] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    if (tmp_result_array[i + 1][j + 1] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    if ((tmp_result_array[i][j] == 0) && ((counter_living_cells == 2) || (counter_living_cells == 3))) {
                        current_situation[i][j] = 0;

                    }
                    else if ((tmp_result_array[i][j] == 1) &&(counter_living_cells == 3)) {
                        current_situation[i][j] = 0;
                    }
                    else {
                        current_situation[i][j] = 1;
                    }
                }
                else if ((i > 0) && (i < image_height - 1) && (j == image_width - 1)) {
                    if (tmp_result_array[i - 1][j] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    if (tmp_result_array[i - 1][j - 1] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    if (tmp_result_array[i][j - 1] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    if (tmp_result_array[i + 1][j] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    if (tmp_result_array[i + 1][j - 1] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    if ((tmp_result_array[i][j] == 0) && ((counter_living_cells == 2) || (counter_living_cells == 3))) {
                        current_situation[i][j] = 0;

                    }
                    else if ((tmp_result_array[i][j] == 1) &&(counter_living_cells == 3)) {
                        current_situation[i][j] = 0;
                    }
                    else {
                        current_situation[i][j] = 1;
                    }
                }
                else if ((j > 0) && (j < image_width - 1) && (i == 0)) {
                    if (tmp_result_array[i][j - 1] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    if (tmp_result_array[i][j + 1] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    if (tmp_result_array[i + 1][j - 1] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    if (tmp_result_array[i + 1][j] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    if (tmp_result_array[i + 1][j + 1] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    if ((tmp_result_array[i][j] == 0) && ((counter_living_cells == 2) || (counter_living_cells == 3))) {
                        current_situation[i][j] = 0;

                    }
                    else if ((tmp_result_array[i][j] == 1) &&(counter_living_cells == 3)) {
                        current_situation[i][j] = 0;
                    }
                    else {
                        current_situation[i][j] = 1;
                    }
                }
                else if ((j > 0) && (j < image_width - 1) && (i == image_height - 1)) {
                    if (tmp_result_array[i][j - 1] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    if (tmp_result_array[i][j + 1] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    if (tmp_result_array[i - 1][j + 1] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    if (tmp_result_array[i - 1][j] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    if (tmp_result_array[i - 1][j - 1] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    if ((tmp_result_array[i][j] == 0) && ((counter_living_cells == 2) || (counter_living_cells == 3))) {
                        current_situation[i][j] = 0;

                    }
                    else if ((tmp_result_array[i][j] == 1) &&(counter_living_cells == 3)) {
                        current_situation[i][j] = 0;
                    }
                    else {
                        current_situation[i][j] = 1;
                    }
                }
                else {
                    if (tmp_result_array[i - 1][j - 1] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    
                    if (tmp_result_array[i - 1][j] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    
                    if (tmp_result_array[i - 1][j + 1] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    
                    if (tmp_result_array[i][j - 1] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    
                    if (tmp_result_array[i][j + 1] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    
                    if (tmp_result_array[i + 1][j - 1] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    
                    if (tmp_result_array[i + 1][j] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    
                    if (tmp_result_array[i + 1][j + 1] == 1) counter_death_cells++;
                    else counter_living_cells++;
                    
                    if ((tmp_result_array[i][j] == 0) && ((counter_living_cells == 2) || (counter_living_cells == 3))) {
                        current_situation[i][j] = 0;

                    }
                    else if ((tmp_result_array[i][j] == 1) &&(counter_living_cells == 3)) {
                        current_situation[i][j] = 0;
                    }
                    else {
                        current_situation[i][j] = 1;
                    }
                }
            }
        }
        for (int p = 0; p < image_height; p++) {
            for (int l = 0; l < image_width; l++) {
                tmp_result_array[p][l] = current_situation[p][l];
            }
        }
    }
    printf("\n");
    for (int t = 0; t < image_height; t++) {
        printf("\n");
        for (int n = 0; n < image_width; n++) {
            printf("%d ", current_situation[t][n]);
        }
    }
    printf ("\n");
    
    // блок записывания результата в картинку
    int current_pix = 0;
    int num_of_4_bytes_blocks;
    if (image_width < 32) {
        num_of_4_bytes_blocks = 1;
    } else num_of_4_bytes_blocks = (image_width / 32) + 1;
    int how_bytes_per_string = 0;
    how_bytes_per_string = (image_width / 8) + 1;
    char current_byte;
    int num_of_8_blocks = 0;
    char empty_byte = '\0';
    int free_bits = 0;
    if (image_width >= 8) {
        free_bits = image_width % 8;
    }
    else free_bits = image_width;
//    int free_bits = image_width % 8;
    if (image_width > 8) {
         num_of_8_blocks = image_width / 8;
    }
    else num_of_8_blocks = 0;
    char array_of_8_bits[8];
    for (int i = image_height - 1; i >= 0; i--) {
        current_pix = 0;
        for (int j = 0; j < num_of_8_blocks; j++) {
            for (int b = 0; b < 8; b++) {
                array_of_8_bits[b] = current_situation[i][current_pix];
                current_pix++;
            }
            current_byte = bits_to_byte(array_of_8_bits, 8);
//            printf("%c\n", current_byte);
            fputc(current_byte, fout);
        }
        for (int k = 0; k < free_bits; k++) {
            array_of_8_bits[k] = current_situation[i][current_pix];
//            printf("%d", current_situation[i][current_pix]);
            current_pix++;
        }
        current_byte = bits_to_byte(array_of_8_bits, free_bits);
        fputc(current_byte, fout);
        for (int empty = 0; empty < num_of_4_bytes_blocks * 4 - how_bytes_per_string; empty++) {
            fputc(empty_byte, fout);
        }
    }
    
    
    
    return 0;
}
