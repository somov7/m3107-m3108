#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <iso646.h>
#include "id3lib.h"
#include "argparse.h"

int main(int argc, char **argv) {
    char *filename = "", *to_get = "", *to_set = "";
    bool rewrite = false, extract_pictures = false;
    parse_args(argc, argv, &filename, &to_get, &to_set, &rewrite, &extract_pictures);
    FILE *f = fopen(filename, "r+b");
    if (f == NULL) {
        printf("File not opened.\n");
        return 1;
    }
    struct id3tag tag;
    
    switch (read_id3v2_tag(f, &tag)) {
    case TAG_NOT_FOUND:
        printf("ID3 tag has not been found.\n");
        return 1;
        break;
    case TAG_TOO_BIG_VERSION:
        printf("ID3 tag has too big version.\n");
        return 1;
    }
    if (strlen(to_get) == 0 and strlen(to_set) == 0 and not extract_pictures) {
        char id[4], *buffer;
        struct frame_list *list = tag.first;
        printf("Tag ID\tData\n");
        while (list != NULL) {
            if (list->frame.id[0] == 'T' or list->frame.id[0] == 'C' or memcmp(list->frame.id, "APIC", 4) == 0) {
                buffer = malloc(memcmp(list->frame.id, "APIC", 4) == 0 ? 1024 : list->frame.size);
                memcpy(id, list->frame.id, 4);
                text_frame_to_str(&list->frame, buffer);
                printf("%4s\t%s\n", id, buffer);
                free(buffer);
            }
            list = list->next;
        }
        return 0;
    }
    if (strlen(to_get)) {
        char *id, *buffer;
        struct frame *read;
        printf("Tag ID\tData\n");
        id = strtok(to_get, ",");
        do {
            if ((read = get_frame(id, &tag))) {
                buffer = malloc(read->size);
                text_frame_to_str(read, buffer);
                printf("%s\t%s\n", id, buffer);
                free(buffer);
            }
        } while ((id = strtok(NULL, ",")));
    }
    if (strlen(to_set)) {
        if (rewrite) {
            free_id3v2_tag(&tag);
            tag.size = MINIMUM_PADDING;
            tag.extended_header_size = 0;
            tag.extended_header = NULL;
            tag.flags = 0;
        }
        char *buffer, *key, *value;
        struct frame new, *existing;
        key = strtok(to_set, "=");
        value = strtok(NULL, ",");
        do {
            if (key[0] == 'T' or memcmp(key, "COMM", 4) == 0) {
                tag.size += get_frame(key, &tag) ? strlen(value) - 1 - get_frame(key, &tag)->size : strlen(value) + 11;
                put_text_frame(key, value, &tag);
            }
            else if (strcmp(key, "APIC") == 0) {
                char *token = strtok(value, ":"), *name = strtok(NULL, ":");
                int type;
                sscanf(token, "%02x", &type);
                put_picture_frame(name, token[2] != '%', type, strtok(NULL, ","), &tag);
                tag.size += get_frame("APIC", &tag)->size + 10;
            }
        } while ((key = strtok(NULL, "="), value = strtok(NULL, ",")));
        write_id3v2_tag(filename, &tag);
    }
    if (extract_pictures) {
        struct frame_list *list = tag.first;
        while (list) {
            if (memcmp(list->frame.id, "APIC", 4) == 0) {
                void *data = list->frame.data + 1;

                char *mime = (char *)data + 1, type;
                data += strlen(mime) + 2;
                type = *(char *)data;
                data += strlen(data + 1) + 2;
                char picture_name[strlen(filename) + 10];
                sprintf(picture_name, "%x_%s.%s", type, filename, strchr(mime, '/') + 1);
                FILE *picture = fopen(picture_name, "wb");
                if (picture == NULL) {
                    fprintf(stderr, "%s: Error: Could not write to file '%s'\n", argv[0], picture_name);
                    return 3;
                }
                for (char *copy = data; copy < (char *)list->frame.data + list->frame.size; copy++) {
                    fputc(*copy, picture);
                }
                fclose(picture);
            }
            list = list->next;
        }
    }
}