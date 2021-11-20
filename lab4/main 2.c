#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//unsigned char total_array[4];
int checker_frame = 2;
unsigned long bitwise_shift (unsigned char basic_size[4]) {
    unsigned long total_size = 0;
    for (int i = 0; i < 4; i++) {
        total_size = total_size | (basic_size[3 - i] << (7 * i));
    }
    return total_size;
}
//void back_bitwise_shift (unsigned long  start_number, unsigned char *total_array) {
//
//    total_array[1] = (start_number << 11) >> 25;
//    total_array[2] = (start_number << 18) >> 25;
//    total_array[3] = (start_number << 24) >> 25;
//    total_array[0] = start_number >> 21;
////    for (int i = 1; i < 4; i++) {
////        total_array[i] = start_number << (32 - 7 * i);
////    }
//}
void back_bitwise_shift(unsigned long start_number, unsigned char *total_array) {
    total_array[0] = start_number >> 21;
    for (int i = 1; i < 4; i++) {
        total_array[i] = (start_number << (7 * i + 4)) >> 25;
    }
    
}
typedef struct TAG {
    unsigned char tag_array[10];
    unsigned char signature_array[3];
    unsigned char version_array[2];
    unsigned char flag_array[1];
    unsigned char size_array[4];
} TAG;

void filling_TAG (unsigned char *basic_array, TAG *TAG) {
//    TAG TAG;
    for (int signature_count = 0; signature_count < 3; signature_count++) TAG -> signature_array[signature_count] = basic_array[signature_count];
    for (int version_count = 3; version_count < 5; version_count++) TAG -> version_array[version_count-3] = basic_array[version_count];
    for (int flag_count = 5; flag_count < 6; flag_count++) TAG -> flag_array[flag_count-5] = basic_array[flag_count];
    for (int size_count = 6; size_count < 10; size_count++) TAG -> size_array[size_count-6] = basic_array[size_count];
}
typedef struct FRAME {
    unsigned char frame_array[10];
    char id_array[4];
    unsigned char size_array[4];
    unsigned char flags_array[2];
} FRAME;

void filling_FRAME(unsigned char *basic_array, FRAME *FRAME) {
//    FRAME FRAME;
    for (int id_count = 0; id_count < 4; id_count++) FRAME -> id_array[id_count] = basic_array[id_count];
    for (int size_count = 4; size_count < 8; size_count++) FRAME -> size_array[size_count-4] = basic_array[size_count];
    for (int flags_count = 8; flags_count < 10; flags_count++) FRAME -> flags_array[flags_count-8] = basic_array[flags_count];
}

void show (char *in_file) {
    FILE *fin;
    fin = fopen (in_file, "rb");
    TAG current_tag;
    fread(current_tag.tag_array, sizeof(char), 10, fin);
    filling_TAG(current_tag.tag_array, &current_tag);
//    unsigned long a = bitwise_shift(current_tag.size_array);
    for (int i = 0; i < 3; i++) printf("%c", current_tag.signature_array[i]); printf("v2."); printf("%d\n", current_tag.version_array[0]);
    while (ftell(fin) <= bitwise_shift(current_tag.size_array)) {
//        unsigned long a = bitwise_shift(current_tag.size_array);
        FRAME current_frame;
        fread (current_frame.frame_array, sizeof(char), 10, fin);
        filling_FRAME(current_frame.frame_array, &current_frame);
        for (int i = 0; i < 4; i++) {
            printf("%c", current_frame.id_array[i]);
        }
        if ((current_frame.id_array[0] >= 65) && (current_frame.id_array[0] <= 90)) {
            printf(": ");
        }
        else break;
//        printf ("\n");
//        unsigned long a = bitwise_shift(current_frame.size_array);
        unsigned char frame_data[bitwise_shift(current_frame.size_array)];
//        unsigned char frame_data[20];
        fread (frame_data, sizeof(char), bitwise_shift(current_frame.size_array), fin);
//        fread (frame_data, sizeof(char), 20, fin);
        for (long i = 0; i < bitwise_shift(current_frame.size_array); i++) printf ("%c", frame_data[i]);
//        for (int i = 0; i < 20; i++) printf("%c", frame_data[i]);
        printf("\n");
        checker_frame--;
    }
}

long user_strcmp (unsigned char *str1[4], unsigned char *str2[4]) {
    for (int i = 0; i < 4; i++) {
        if (str1[i] != str2[i]) return 0;
    }
    return 1;
}

void get (char *in_file, char *get_id) {
    FILE *fin;
    fin = fopen (in_file, "rb");
    
    TAG current_tag;
    fread(current_tag.tag_array, sizeof(char), 10, fin);
    filling_TAG(current_tag.tag_array, &current_tag);
    while (ftell(fin) <= bitwise_shift(current_tag.size_array)) {
//        unsigned long a = bitwise_shift(current_tag.size_array);
        FRAME current_frame;
        fread (current_frame.frame_array, sizeof(char), 10, fin);
        filling_FRAME(current_frame.frame_array, &current_frame);
        unsigned char frame_data[bitwise_shift(current_frame.size_array)];
        fread (frame_data, sizeof(char), bitwise_shift(current_frame.size_array), fin);
        if (strcmp(current_frame.id_array, get_id) == 0) {
            checker_frame -= 2;;
            for (int i = 0; i < 4; i++) printf("%c", current_frame.id_array[i]); printf(": ");
            for (int i = 0; i < bitwise_shift(current_frame.size_array); i++) printf ("%c", frame_data[i]);
            printf("\n");
        }
//        for (int i = 0; i < 4; i++) printf("", )
    }
}
void set (char *in_file, char *change_frame_id, char *change_metadata) {
    FILE *fin, *fin2;
    fin = fopen (in_file, "rb");
//    fin2 = fopen("/Users/artemparfenov/Desktop/MP3_out_file.mp3", "wb");
    fin2 = fopen("./MP3_out_file.mp3", "wb");
    unsigned long size_of_new_data = strlen(change_metadata);
    TAG current_tag;
    fread(current_tag.tag_array, sizeof(char), 10, fin);
    filling_TAG(current_tag.tag_array, &current_tag);
//    fwrite()v
//    int f = ftell(fin);
//    f = bitwise_shift(current_tag.size_array);
    while (ftell(fin) <= bitwise_shift(current_tag.size_array)) {
        FRAME current_frame;
        fread (current_frame.frame_array, sizeof(char), 10, fin);
        filling_FRAME(current_frame.frame_array, &current_frame);
        
        unsigned char frame_data[bitwise_shift(current_frame.size_array)];
        
        fread (frame_data, sizeof(char), bitwise_shift(current_frame.size_array), fin);
//        char a = current_frame.id_array[0];
        if (current_frame.id_array[0] == 0) {
            break;
        }
        if (strcmp(current_frame.id_array, change_frame_id) == 0) {
            unsigned long  current_cursor = ftell(fin) - 10 - bitwise_shift(current_frame.size_array);
            fseek(fin, 0, SEEK_END);
            unsigned long end_of_file = ftell(fin);
//            int a = ftell(fin);
//            fseek(fin, 0, current_cursor);
//            a = ftell (fin);
            fseek (fin, 0, SEEK_SET);
//            a = ftell(fin);
            char current_char;
            back_bitwise_shift(bitwise_shift(current_tag.size_array) + size_of_new_data + 1 - bitwise_shift(current_frame.size_array), current_tag.size_array);
            for (int i = 0; i < 6; i++) {
                current_char = fgetc(fin);
                fputc(current_char, fin2);
            }
            for (int i = 0; i < 4; i++) {
                fputc(current_tag.size_array[i], fin2);
            }
            fseek(fin, 4, SEEK_CUR);
            while (ftell(fin) < current_cursor) {
                current_char = fgetc(fin);
                fputc(current_char, fin2);
//                a = ftell(fin);
            }
//            for (int i = 0; i < size_of_new_data; i++) {
//                frame_data[i] = change_metadata[i];
//            }
            for (int i = 0; i < 4; i++) {
                fputc(current_frame.id_array[i], fin2);
            }
            fseek(fin, 10 + bitwise_shift(current_frame.size_array), SEEK_CUR);
            back_bitwise_shift(size_of_new_data + 1, current_frame.size_array);
            for (int i = 0; i < 4; i++) {
                printf ("%c", current_frame.size_array[i]);
            }
            for (int i = 0; i < 4; i++) {
                fputc(current_frame.size_array[i], fin2);
            }
            for (int i = 0; i < 2; i++) {
                fputc(current_frame.flags_array[i], fin2);
            }
            fputc('\0', fin2);
            for (int i = 0; i < size_of_new_data ; i++) {
                fputc(change_metadata[i], fin2);
            }
            while(ftell(fin) < end_of_file) {
                current_char = fgetc(fin);
                fputc(current_char, fin2);
            }
            fclose(fin);
            fclose(fin2);
            fin = fopen(in_file, "wb");
            fin2 = fopen("/Users/artemparfenov/Desktop/MP3_out_file.mp3", "rb");
            fseek(fin2, 0, SEEK_END);
            end_of_file = ftell(fin2);
            fseek(fin2, 0, SEEK_SET);
            while (ftell(fin2) < end_of_file) {
                current_char = fgetc(fin2);
                fputc(current_char, fin);
            }
        }
    }
    checker_frame--;
}

int main(int argc, char * argv[]) {
    int current_value_counter = 0;
//    char *cmd_get = "g";
    if ((argc != 3) && (argc != 4)) {
        fprintf(stderr, "Проверьте количество параметров\n");
        return EXIT_FAILURE;
    }
    if ((strcmp(argv[2], "--show") == 0) && (argc == 3)) {
        show(argv[1]);
    }
    else {
        fprintf(stderr, "Проверьте количество параметров\n");
        return EXIT_FAILURE;
    }
    if ((memcmp(argv[2], "--set=", 6) == 0) && (argc == 4)) {
        char temp_arr_prop_name[4];
        for (int i = 6; i < 10; i++) {
            temp_arr_prop_name[current_value_counter] = argv[2][i];
            current_value_counter++;
        }
        current_value_counter = 0;
        unsigned long argv_value_len = strlen(argv[3]) - 8;
        char temp_arr_prop_value[argv_value_len];
        for (int i = 8; i < 12; i++) {
            temp_arr_prop_value[current_value_counter] = argv[2][i];
            current_value_counter++;
        }
        set(argv[1], temp_arr_prop_name, temp_arr_prop_value);
    }
    if (memcmp(argv[2], "--get=", 6)) {
        char temp_arr_arg[4];
        for (int i = 6; i < 10; i++) {
            temp_arr_arg[current_value_counter] = argv[2][i];
            current_value_counter++;
        }
        get (argv[1], temp_arr_arg);
    }

    
    
    
//    FILE *fin;
//    setlocale(LC_ALL, "Rus");
//    fin = fopen("/Users/artemparfenov/Downloads/DreamOn.mp3", "rb");
//    show("/Users/artemparfenov/Downloads/DreamOn.mp3");
//    set ("/Users/artemparfenov/Downloads/DreamOn.mp3", "TALB", "SVETA_IS_BEST");
//    show("/Users/artemparfenov/Downloads/Michael Jackson — Billie Jean.mp3");
//    show("/Users/artemparfenov/Downloads/Oxxxymiron - Кто Убил Марка_.mp3");
//    show (argv[1]);
//    get ("TIT2", argv[1]);
//    get ("/Users/artemparfenov/Downloads/DreamOn.mp3", "USLT");
    if (checker_frame == 2) {
        fprintf(stderr, "Проверьте параметры\n");
        return EXIT_FAILURE;
    }
    return 0;
}
