//
//  main.c
//  Prog_4
//
//  Created by Andrew Mikhno on 12.11.2021.
//

#include <stdio.h>
#include <string.h>
#include <locale.h>

typedef struct tag {
    unsigned char idv3[3];
    unsigned char version[1];
    unsigned char SubPlusFlag[2];
    unsigned char size[4];
} tag;

typedef struct frame {
    unsigned char id[4];
    unsigned char size[4];
    unsigned char flag[2];
} frame;

void GettingNewSIZE(unsigned int length, unsigned char *size) {
    for (int i = 1; i < 4; i++)
    {
        size[i] = (length << (7 * i + 4)) >> 25;
    }
    size[0] = length >> 21;
}

int GettingSIZE(unsigned char size[4]) {
    int size_return = 0;
    for (int i = 0; i<4; i++)
        size_return+=size[3 - i] << (7 * i);
    return size_return;
}

int data(char *name) {
    FILE *fin;
    int id_counter = 0;
    fin = fopen(name, "r");
    int checker = 0;
    if (fin == NULL) {
        fprintf(stderr, "Error!\nFile did not open.");
        return 1;
    }
    fseek(fin, 0, SEEK_SET);
    tag current_tag;
    fread(current_tag.idv3, sizeof(char), 3, fin);
    fread(current_tag.version, sizeof(char), 1, fin);
    fread(current_tag.SubPlusFlag, sizeof(char), 2, fin);
    fread(current_tag.size, sizeof(char), 4, fin);
    unsigned int TagSize = GettingSIZE(current_tag.size);
    for (int i = 0; i<3; i++)
        printf("%c", current_tag.idv3[i]);
    printf("v2.");
    printf("%d\n", current_tag.version[0]);
    while (ftell(fin) <= TagSize) {
        frame current_frame;
        fread(current_frame.id, sizeof(char), 4, fin);
        fread(current_frame.size, sizeof(char), 4, fin);
        fread(current_frame.flag, sizeof(char), 2, fin);
        unsigned int FrameSize = GettingSIZE(current_frame.size);
        unsigned char temp_char[FrameSize];
            for (int i = 0; i<4; i++)
                printf("%c", current_frame.id[i]);
            if ((current_frame.id[0] >= 65) && (current_frame.id[0] <=90))
                printf(": ");
            else
                checker = 1;
            fread(temp_char, sizeof(char), FrameSize, fin);
            for (int j = 0; j < FrameSize; j++)
                printf("%c", temp_char[j]);
            if (checker == 0)
                printf ("\n");
    }
    return 0;
}

int get_data(char *name, char temp_id[4]) {
    FILE *fin;
    fin = fopen(name, "r");
    int checker = 0;
    int id_counter = 0;
    if (fin == NULL) {
        fprintf(stderr, "Error!\nFile did not open.");
        return 1;
    }
    fseek(fin, 0, SEEK_SET);
    tag current_tag;
    fread(current_tag.idv3, sizeof(char), 3, fin);
    fread(current_tag.version, sizeof(char), 1, fin);
    fread(current_tag.SubPlusFlag, sizeof(char), 2, fin);
    fread(current_tag.size, sizeof(char), 4, fin);
    unsigned int TagSize = GettingSIZE(current_tag.size);
    while (ftell(fin) <= TagSize) {
        id_counter = 0;
        frame current_frame;
        fread(current_frame.id, sizeof(char), 4, fin);
        fread(current_frame.size, sizeof(char), 4, fin);
        fread(current_frame.flag, sizeof(char), 2, fin);
        unsigned int FrameSize = GettingSIZE(current_frame.size);
        unsigned char temp_char[FrameSize];
        for (int i = 0; i<4; i++) {
            if (current_frame.id[i] == temp_id[i])
                id_counter++;
        }
        if (id_counter == 4) {
        for (int i = 0; i<4; i++)
            printf("%c", current_frame.id[i]);
        printf(": ");
        fread(temp_char, sizeof(char), FrameSize, fin);
        for (int j = 0; j < FrameSize; j++)
            printf("%c", temp_char[j]);
        if (checker == 0)
            printf ("\n");
        checker = 1;
        break;
        }
        else
            fread(temp_char, sizeof(char), FrameSize, fin);
    }
    if (checker == 0)
        printf("There is no such frame in this file.");
    printf ("\n");
    return 0;
}

int change_data(char *name, char temp_id[4], unsigned char value[]) {
    FILE *fin;
    fin = fopen(name, "r");
    FILE *fin_temp;
    char sz;
    fseek(fin, 0, SEEK_END);
    sz = ftell(fin);
    fin_temp = tmpfile();
    int id_counter = 0;
    unsigned char temp_symbol;
    int old_size_frame = 0;
    int new_size_frame = 0;
    int new_check_frame = 0;
    if (fin == NULL) {
        fprintf(stderr, "Error!\nFile did not open.");
        return 1;
    }
    fseek(fin, 0, SEEK_SET);
    tag current_tag;
    fread(current_tag.idv3, sizeof(char), 3, fin);
    fread(current_tag.version, sizeof(char), 1, fin);
    fread(current_tag.SubPlusFlag, sizeof(char), 2, fin);
    fread(current_tag.size, sizeof(char), 4, fin);
    unsigned int TagSize = GettingSIZE(current_tag.size);
    fseek(fin_temp, 0, SEEK_SET);
    fseek(fin, 0, SEEK_SET);
    for (int i = 0; i < 10; i++) {
            temp_symbol = fgetc(fin);
            fputc(temp_symbol, fin_temp);
    }
    printf("ok\n");
    while (ftell(fin) <= TagSize) {
        id_counter = 0;
        frame current_frame;
        fread(current_frame.id, sizeof(char), 4, fin);
        fread(current_frame.size, sizeof(char), 4, fin);
        fread(current_frame.flag, sizeof(char), 2, fin);
        unsigned int FrameSize = GettingSIZE(current_frame.size);
        unsigned char temp_char[FrameSize];
        for (int i = 0; i<4; i++) {
            if (current_frame.id[i] == temp_id[i])
                id_counter++;
        }
        if (id_counter == 4) {
            fread(temp_char, sizeof(char), FrameSize, fin);
            for (int i = 0; i<4; i++)
                printf("%c", current_frame.id[i]);
            printf(": ");
            old_size_frame = FrameSize;
            GettingNewSIZE((unsigned int)strlen(value) + 1, current_frame.size);
            unsigned int FrameSize = GettingSIZE(current_frame.size);
            new_size_frame = FrameSize;
            for (int i = 0; i < 4; i++)
                fputc(current_frame.id[i], fin_temp);
            for (int i = 0; i < 4; i++)
                fputc(current_frame.size[i], fin_temp);
            for (int i = 0; i<2; i++)
                fputc(current_frame.flag[i], fin_temp);
            for (int i = 0; i < FrameSize; i++) {
                fputc(value[i], fin_temp);
                printf("%c", value[i]);
            }
            printf ("\n");
            new_check_frame = 1;
            break;
        }
        else {
            fwrite(current_frame.id, sizeof(char), 4, fin_temp);
            fwrite(current_frame.size, sizeof(char), 4, fin_temp);
            fwrite(current_frame.flag, sizeof(char), 2, fin_temp);
            fread(temp_char, sizeof(char), FrameSize, fin);
            for (int i = 0; i<FrameSize; i++) {
                fputc(temp_char[i], fin_temp);
            }
        }
    }
//    if (new_check_frame == 0) {
//        char new_size_frame[4];
//        for (int i = 0; i<4; i++)
//            printf("%c", temp_id[i]);
//        printf(": ");
//        GettingNewSIZE((unsigned int)strlen(value) + 1, new_size_frame);
//        unsigned int FrameSize = GettingSIZE(new_size_frame);
//        for (int i = 0; i < 4; i++)
//            fputc(temp_id[i], fin_temp);
//        for (int i = 0; i < 4; i++)
//            fputc(new_size_frame, fin_temp);
//        for (int i = 0; i < 2; i++)
//            fputc("0", fin_temp);
//        for (int i = 0; i < FrameSize; i++) {
//            fputc(value[i], fin_temp);
//            printf("%c", value[i]);
//        }
//        for (int i = 0; i < strlen(value); i++) {
//            fputc(value[i], fin_temp);
//        }
//        printf ("\n");
//    }
    TagSize += new_size_frame-old_size_frame;
    GettingNewSIZE(TagSize, current_tag.size);
    unsigned char symbol = ' ';
    symbol = fgetc(fin);
    while (feof(fin) == 0) {
        fputc(symbol, fin_temp);
        symbol = fgetc(fin);
    }
    fseek(fin_temp, 0, SEEK_SET);
    fseek(fin, 0, SEEK_SET);
    for (int i = 0; i < 6; i++)
        temp_symbol = fgetc(fin);
    fread(current_tag.size, sizeof(char), 4, fin);
    fseek(fin, 0, SEEK_SET);
    temp_symbol = fgetc(fin);
    for (int i = 0; i < 10; i++) {
        fputc(temp_symbol, fin_temp);
        temp_symbol = fgetc(fin);
    }
    fseek(fin_temp, 0L, SEEK_END);
    sz = ftell(fin_temp);
    fseek(fin_temp, 0, SEEK_SET);
    FILE *fin_new;
    fin_new = fopen(name, "w");
    fseek(fin_new, 0, SEEK_SET);
    symbol = fgetc(fin_temp);
    while (feof(fin_temp) == 0) {
        fputc(symbol, fin_new);
        symbol = fgetc(fin_temp);
    }
    fclose(fin_temp);
    fclose(tmpfile());
    return 0;
}
    
int main(int argc, char **argv) {
    setlocale(LC_ALL, "Rus");
    if (strcmp(argv[2], "--show") == 0) {
        if (argc == 3)
            data(argv[1]);
        else {
            fprintf(stderr, "Error!\nWrong count of arguments.\n");
            return 1;
        }
    }
    else if ((argv[2][2] == 'g' ) && (argv[2][3] == 'e' ) && (argv[2][4] == 't' )) {
        if (argc == 3) {
            char Asked_Frame[4];
            for (int i = 6; i<10; i++) {
                Asked_Frame[i-6] = argv[2][i];
            }
            get_data(argv[1], Asked_Frame);
        }
        else {
            fprintf(stderr, "Error!\nWrong count of arguments.\n");
            return 1;
        }
    }
    else if ((argv[2][2] == 's' ) && (argv[2][3] == 'e' ) && (argv[2][4] == 't' )) {
        if (argc == 4) {
            char Asked_Frame[4];
            unsigned long size;
            for (int i = 6; i<10; i++) {
                Asked_Frame[i-6] = argv[2][i];
            }
            size = strlen(argv[3]);
            char *value;
            value = strpbrk(argv[3], "=") + 1;
            change_data(argv[1], Asked_Frame, value);
        }
        else {
            fprintf(stderr, "Error!\nWrong count of arguments.\n");
            return 1;
        }
    }
    else {
        printf("Error!\nWrong argument.");
    }
    return 0;
}

