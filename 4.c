#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "notation.h"

FILE* file;
int ID3_size;

char APIC[] = "APIC";
char TXXX[] = "TXXX";

void error(char comment[])
{
    printf("%s\n", comment);
    printf("--error--\n");
    printf("____________________\n");
    exit(1);
}

bool is_frame(char* arr)
{
    if (strcmp(APIC, arr) == 0 || strcmp(TXXX, arr) == 0)
        return 0;
    for (int i = 0; i < 3; i++)
        if (arr[i] < 'A' || arr[i]>'Z')
            return 0;
    if ((arr[3] < 'A' || arr[3]>'Z') && (arr[3] < '0' || arr[3]>'9'))
        return 0;
    return 1;
}
void shift_file(int shift_it, int n)
{
    FILE* save = fopen("saving.sus", "w+b");
    int size = 0;
    fseek(file, shift_it, SEEK_SET);
    fseek(save, 0, SEEK_SET);
    while (!feof(file))
    {
        int c = getc(file);
        putc(c, save);
        size++;
    }
    fseek(file, shift_it + n, SEEK_SET);
    fseek(save, 0, SEEK_SET);
    for (int i = 0; i < size; i++)
    {
        int c = getc(save);
        putc(c, file);
    }
    fclose(save);
    remove("saving.sus");
}
int scan_size(int number)
{
    char* bits = (char*)malloc(number * 7 + 1);
    bits[number * 7] = 0;
    for (int i = 0; i < number; i++)
    {
        int data = 0;
        fread(&data, 1, 1, file);
        bits = get_10_2(data, 7, i, bits);
    }
    int data = get_2_10(bits);
    free(bits);
    return data;
}
void print_size(int data)
{
    char* bits = (char*)malloc(33);
    bits[32] = 0;
    bits = get_10_2(data, 32, 0, bits);
    char new_bits[33];
    new_bits[32] = 0;
    int j = 3;
    for (int i = 28; i >= 0; i--)
    {
        if ((i + 3) % 8 == 0)
        {
            new_bits[i + j] = '0';
            j--;
        }
        new_bits[i + j] = bits[i + 3];
    }
    free(bits);
    for (int i = 0; i < 4; i++)
    {
        char arr[9];
        arr[8] = 0;
        for (int k = i * 8; k < (i + 1) * 8; k++)
            arr[k - i * 8] = new_bits[k];
        putc(get_2_10(arr), file);
    }
}
void read_file(char* your_frame)
{
    fseek(file, 10, SEEK_SET);
    bool code = 0;
    if (strlen(your_frame) == 4)
        code = 1;

    int it = 0;
    while (1)
    {
        char frame[5];
        frame[4] = 0;
        fread(frame, 4, 1, file);

        if (code == 1)
            while (1)
            {
                if (!is_frame(frame))
                {
                    printf("*WARNING! NOT FOUND THIS FRAME\n");
                    return;
                }
                if (strcmp(frame, your_frame) == 0)
                    break;
                else
                {
                    fseek(file, 2 + scan_size(4), SEEK_CUR);
                    fread(frame, 4, 1, file);
                }
            }

        if (!is_frame(frame))
            return;
        int frame_size = 0;
        frame_size = scan_size(4);

        fseek(file, 2, SEEK_CUR);
        printf("%s:", frame);
        for (int i = 0; i < frame_size; i++)
        {
            char data = 0;
            fread(&data, 1, 1, file);
            printf("%c", data);
        }
        if (code == 1)
        {
            printf("\n");
            return;
        }
        printf("\n");
    }
}
void write_file(char* your_frame, char* data)
{
    fseek(file, 6, SEEK_SET);
    ID3_size = scan_size(4);
    int cursor = 0;
    if (strlen(your_frame) != 4)
        error("*YOUR FRAME MUST BE HAVE SIZE FOUR");
    if (!is_frame(your_frame))
    {
        printf("*YOUR FRAME MUST BE HAVE ONLY CAPITAL ENGLISH LETTERS,\n");
        printf("AND LAST CHAPTER CAN BE NUMBER\n");
        error("");
    }

    fseek(file, 10, SEEK_SET);

    char frame[5];
    frame[4] = 0;

    fread(frame, 4, 1, file);
    cursor += 14;

    while (1)
    {
        
        if (strcmp(frame, your_frame) == 0)
            break;
        else
        {
            if (!is_frame(frame))
            {
                shift_file(10, 11 + strlen(data));
                fseek(file, 10, SEEK_SET);
                for (int i = 0; i < 4; i++)
                    putc(your_frame[i], file);
                print_size(strlen(data)+1);
                fseek(file, 2, SEEK_CUR);
                putc(32, file);
                for (int i = 0; i < strlen(data); i++)
                    putc(data[i], file);
                fseek(file, 6, SEEK_SET);
                print_size(ID3_size + 11 + strlen(data));
                return;
            }
            int size = scan_size(4);
            fseek(file, 2 + size, SEEK_CUR);
            fread(frame, 4, 1, file);
            cursor += 10 + size;
        }
    }
    int size_data = scan_size(4);
    int ur_size_data = strlen(data);
    fseek(file, cursor, SEEK_SET);
    print_size(ur_size_data+1);
    shift_file(cursor + 6 + size_data, ur_size_data-size_data+1);
    fseek(file, cursor + 6, SEEK_SET);
    putc(32, file);
    for (int i = 0; i < ur_size_data; i++)
        putc(data[i], file);
    fseek(file, 6, SEEK_SET);
    print_size(ID3_size-size_data+ur_size_data+1);
    
}
void del(char your_frame[])
{
    fseek(file, 6, SEEK_SET);
    ID3_size = scan_size(4);
    if (strlen(your_frame) != 4)
        error("*YOUR FRAME MUST BE HAVE SIZE FOUR");
    char frame[5];
    frame[4] = 0;
    fread(frame, 4, 1, file);
    int cursor = 14;
    while (1)
    {

        if (strcmp(frame, your_frame) == 0)
            break;
        else
        {
            if (!is_frame(frame))
            {
                printf("*WARNING! NOT FOUND THIS FRAME");
                return;
            }
            int size = scan_size(4);
            fseek(file, 2 + size, SEEK_CUR);
            fread(frame, 4, 1, file);
            cursor += 10 + size;
        }
    }
    int frame_size = scan_size(4);
    shift_file(cursor + 2 + frame_size, -(frame_size + 10));
    printf("FRAME '%s' was DELITE\n", your_frame);
}
int main(int argc, char** argv)
{
    bool filepath = false;
    bool com = false;
    int i = 1;
    printf("____________________\n");
    while (i < argc)
    {
        char* command;
        command = strtok(argv[i], "=");
        if (strcmp(command, "--filepath") == 0)
        {
            filepath = true;
            command = strtok(NULL, "=");
            file = fopen(command, "r+b");
            if (!file)
                error("*NOT FOUND FILE PATH");
        }
        if (strcmp(command, "--show") == 0)
        {
            com = true;
            char ch[] = "*";
            read_file(ch);
        }
        if (strcmp(command, "--get") == 0)
        {
            com = true;
            command = strtok(NULL, "=");
            read_file(command);
        }
        if (strcmp(command, "--set") == 0)
        {

            if (filepath == false)
                error("* NOT WAS ENTER FILE PATH");
            com = true;
            command = strtok(NULL, "=");
            char* your_frame = command;
            i++;
            command = strtok(argv[i], "=");
            if (strcmp(command, "--value") == 0)
                command = strtok(NULL, "=");
            else
                error("*AFTER '--set' MUST BE '--value'");
            if (command == NULL)
                error("*PLEASE, DON'T WRITE SPACE TO FIRST CHAPTER");
            char* arr = (char*)malloc(strlen(command+1));
            arr[strlen(command)] = 0;
            arr = command;
            i++;
            for (int j = i; j < argc; j++)
            {
                arr = merge(arr, merge(" ", argv[i]));
                i++;
            }
            printf("%s = %s\n", your_frame, arr);
            write_file(your_frame, arr);
        }
        if (strcmp(command, "--delite")==0)
        {
            com = true;
            if (filepath == false)
                error("* NOT WAS ENTER FILE PATH");
            command=strtok(NULL, "=");
            if (command == NULL)
                error("* NOT WAS ENTER FRAME FOR DELITE");
            del(command);
        }
        i++;
    }
    if (filepath == false)
        error("* NOT WAS ENTER FILE PATH");
    if (com == false)
        printf("\n*WARNING! NOT WAS ENTER COMMAND\n");

    printf("--nice--\n");
    printf("____________________\n");
    return 0;
}