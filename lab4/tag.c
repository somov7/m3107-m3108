#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>
#include "tag.h"
#include "utils.h"

TagHeader header;
TagFrame frame;

void get_frames(char *filename) {
    FILE *file = fopen(filename, "rb");

    if (file == NULL) {
        printf("File does not exist");
        exit(1);
    }

    fread(&header, sizeof(char), 10, file);

    int tag_size = bytes_to_int(header.size, true);
    printf("Header size: %d\n", tag_size);

    while (fread(&frame, sizeof(char), 11, file)) {
        if (frame.frame_name[0] == '\0' || ftell(file) >= tag_size) return;
        int frame_size = bytes_to_int(frame.size, (header.version[0] == 4 ? true : false));
        char *temp = read_data(frame_size, file);
        print_frame(frame, temp);
        free(temp);
    }
}

void get_frame(char *filename, char *frame_name) {
    FILE *file = fopen(filename, "rb");

    if (file == NULL) {
        printf("File does not exist");
        exit(1);
    }

    fread(&header, sizeof(char), 10, file);
    int tag_size = bytes_to_int(header.size, true);

    while (fread(&frame, sizeof(char), 11, file)) {
        if (frame.frame_name[0] == '\0' || ftell(file) >= tag_size) // доходим до \0 или дошли до коцна файла
            break;
        int frame_size = bytes_to_int(frame.size, (header.version[0] == 4 ? true : false));
        char *temp = read_data(frame_size, file);

        if (strcmp(frame_name, frame.frame_name) == 0)
            print_frame(frame, temp);

        free(temp);
    }
}

void set_frame(char *filename, char *frame_name, char *frame_value) {

}
