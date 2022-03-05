#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>
#include "tag.h"
#include "utils.h"

TagHeader head;
TagFrame frame;

void get_frames(char *filename, char *frame_name) {
    FILE *file = fopen(filename, "rb");

    bool all_frames = false;
    if (frame_name == NULL) {
        all_frames = true;
    }

    if (file == NULL) {
        printf("File does not exist");
        exit(1);
    }

    fread(&head, sizeof(char), 10, file);

    int tag_size = bytes_to_int(head.size, true);
    printf("Header size: %d\n", tag_size);

    while (fread(&frame, sizeof(char), 11, file)) {
        if (frame.frame_name[0] == '\0' || ftell(file) >= tag_size) return;
        int frame_size = bytes_to_int(frame.size, (head.version[0] == 4 ? true : false));
        char *temp = read_data(frame_size, file);
        if (all_frames) {
            print_frame(frame, temp);
        } else {
            if (strcmp(frame_name, frame.frame_name) == 0)
                print_frame(frame, temp);
        }
        free(temp);
    }
}

void set_frame(char *filename, char *frame_name, char *frame_value) {

}
