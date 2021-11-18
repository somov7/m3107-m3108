#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
const int characters_in_frame = 21;
typedef struct {
    char id[3];
    char version[2];
    char flags;
    char size[4];
}id3_header;

typedef struct {
    char id[4];
    char size[4];
    char flags[2];
    char a;
}id3_frame;

int bytes_to_int(char *ch, int bit) {
    return (int)ch[0] << (3 * bit) | (int)ch[1] << (2 * bit) | (int)ch[2] << bit | (int)ch[3];
}
void int_to_bytes(int x, char* ch, int bit) {
    ch[3] = (char)(x & ((int)pow(2, bit) - 1));
    ch[2] = (char)((x >> bit) & ((int)pow(2, bit) - 1));
    ch[1] = (char)((x >> 2 * bit) & ((int)pow(2, bit) - 1));
    ch[0] = (char)((x >> 3 * bit) & ((int)pow(2, bit) - 1));
}

int main (int argc, char *argv[]) {
    char *filepath = NULL, *prop_name = NULL, *prop_value = NULL;
    FILE *fin, *fout;
    int flag_set = 0, flag_show = 0, flag_get = 0;
    unsigned int cur_size;
    id3_header header;
    id3_frame frame;

    if (argv[1] == NULL || argv[2] == NULL) {
        printf("Not enough arguments...\n");
        return 1;
    }
    if (strstr(argv[1], "--filepath")) {
        char *tmp = strtok(argv[1], "=");
        tmp = strtok(NULL, "=");
        filepath = (char *) malloc(strlen(tmp));
        strcpy(filepath, tmp);
        free(tmp);
    } else {
        printf("Incorrect format\n");
        return 1;
    }
    if (strstr(argv[2], "--get")) {
        flag_get = 1;
        char *tmp = strtok(argv[2], "=");
        tmp = strtok(NULL, "=");
        prop_name = (char *) malloc(strlen(tmp));
        strcpy(prop_name, tmp);
        free(tmp);
    } else if (strstr(argv[2], "--set") && strstr(argv[3], "--value")) {
        flag_set = 1;
        char *tmp;
        tmp = strtok(argv[2], "=");
        tmp = strtok(NULL, "=");
        prop_name = (char *) malloc(strlen(tmp));
        strcpy(prop_name, tmp);

        tmp = strtok(argv[3], "=");
        tmp = strtok(NULL, "=");
        prop_value = (char *) malloc(strlen(tmp));
        strcpy(prop_value, tmp);
        free(tmp);

        printf("Set value: %s %s\n", prop_name, prop_value);
    } else if (strstr(argv[2], "--show")) {
        flag_show = 1;
    } else {
        printf("Incorrect format\n");
        return 1;
    }

    fin = fopen(filepath, "rb+");
    if (!fin) {
        printf("Cannot open this file");
        return 1;
    }

    fread(&header, 1, 10, fin);
    if (!(strcmp(header.id, "ID3"))) {
        printf("Not ID3 format\n");
        return 1;
    }
    cur_size = bytes_to_int(header.size, 7);

    if (flag_set) {
        fout = fopen("output.mp3", "wb");
        if (!fout) {
            printf("Impossible to change this file");
            return 1;
        }
    }
    while ((fread(&frame, 1, 11, fin)) && ftell(fin) <= cur_size) {
        char *tmp;

        int cur_frame_size = bytes_to_int(frame.size, 8);
        tmp = (char *) malloc(cur_frame_size);
        for (int i = 0; i < cur_frame_size - 1; i++)
            tmp[i] = (char) fgetc(fin);

        if (flag_show) {
            if (bytes_to_int(frame.id, 8) == 0)
                break;
            printf("frame_id: %s, frame_size: %4d ", frame.id, cur_frame_size);
            for (int i = 0; i < cur_frame_size - 1; i++)
                printf("%c", tmp[i]);
            printf("\n");

        }
        if (flag_get && !strcmp(frame.id, prop_name)) {
            printf("id: %s, size: %d ", frame.id, cur_frame_size);
            for (int i = 0; i < cur_frame_size - 1; i++)
                printf("%c", tmp[i]);
            printf("\n");
        }
        free(tmp);

        if (flag_set && !strcmp(frame.id, prop_name)) {
            int position, end, new_frame_size, new_header_size;
            char *temp;

            position = ftell(fin);
            fseek(fin, 0, SEEK_END);
            end = ftell(fin);

            new_frame_size = strlen(prop_value) + 1;
            new_header_size = cur_size - cur_frame_size + new_frame_size;
            int_to_bytes(new_header_size, header.size, 7);
            int_to_bytes(new_frame_size, frame.size, 8);

            fwrite(&header, 1, 10, fout);
            fseek(fin, 10, SEEK_SET);
            temp = (char *) malloc(position - characters_in_frame - (cur_frame_size - 1));
            fread(temp, 1, position - characters_in_frame - (cur_frame_size - 1), fin);
            fwrite(temp, 1, position - characters_in_frame - (cur_frame_size - 1), fout);
            fwrite(&frame, 1, 11, fout);
            fwrite(prop_value, 1, strlen(prop_value), fout);

            fseek(fin, position, SEEK_SET);
            temp = (char *) malloc(end - position + 1);
            fread(temp, 1, end - position + 1, fin);
            fwrite(temp, 1, end - position + 1, fout);
            free(temp);
        }
        if (flag_set && strcmp(frame.id, prop_name)) {
            int position, end, new_frame_size, new_header_size;
            char *temp;
            id3_frame new_frame;

            fseek(fin, 0, SEEK_END);
            end = ftell(fin);
            new_frame_size = strlen(prop_value) + 1;
            new_header_size = cur_size - cur_frame_size + new_frame_size;
            int_to_bytes(new_header_size, header.size, 7);
            int_to_bytes(new_frame_size, new_frame.size, 8);
            memcpy(new_frame.id, prop_name, 4);

            fwrite(&header, 1, 10, fout);
            fseek(fin, 10, SEEK_SET);
            while (fread(&frame, 1, 11, fin) && bytes_to_int(frame.id, 8) != 0) {
                fwrite(&frame, 1, 11, fout);
                temp = (char *) malloc(bytes_to_int(frame.size, 8));
                fgets(temp, bytes_to_int(frame.size, 8), fin);
                fwrite(temp, 1, bytes_to_int(frame.size, 8) - 1, fout);
                free(temp);
            }
            fwrite(&new_frame, 1, 11, fout);
            fwrite(prop_value, 1, strlen(prop_value), fout);
            position = ftell(fin);

            temp = (char *) malloc(end - position + 1);
            fread(temp, 1, end - position + 1, fin);
            fwrite(temp, 1, end - position + 1, fout);
            free(temp);
        }
    }
    fclose(fin);
    if (flag_set)
        fclose(fout);
    return 0;
}