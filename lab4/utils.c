#include <stdio.h>
#include <string.h>

#include "utils.h"

struct frame *getFrames(FILE *filePointer) {
    for (int i = 0; i < 10; i++) {
        fgetc(filePointer);
    }

    struct frame frames[83];

    int frameCount = 0;

    char frame[5];
    char value[1024];

    while (1) {
        memset(frame, 0, 5);
        memset(value, 0, 1024);

        for (int i = 0; i < 4; i++) {
            frame[i] = fgetc(filePointer);
        }

        for (int i = 0; i < 3; i++) {
            fgetc(filePointer);
        }

        int size = fgetc(filePointer) - 1;

        for (int i = 0; i < 3; i++) {
            fgetc(filePointer);
        }

        for (int i = 0; i < size; i++) {
            value[i] = fgetc(filePointer);
            if (value[i] == '\0') {
                value[i] = ' ';
            }
        }

        if (strcmp(frame, "") == 0) {
            break;
        }

        strcpy(frames[frameCount].name, frame);
        strcpy(frames[frameCount].value, value);
        frameCount++;
    }

    return frames;
}

void setFrame(FILE *filePointer, char *name, char *value, char *output) {
    FILE *temp = fopen("tmp.txt", "wb");
    struct frame *frames = getFrames(filePointer);

    int exists = 0;

    for (int i = 0; i < 83; i++) {
        if (strcmp(frames[i].name, name) == 0) {
            exists = 1;
            strcpy(frames[i].value, value);
            break;
        }
    }

    if (!exists) {
        for (int i = 0; i < 83; i++) {
            if (strlen(frames[i].name) == 0) {
                strcpy(frames[i].name, name);
                strcpy(frames[i].value, value);
                break;
            }
        }
    }

    for (int i = 0; i < 83; i++) {
        if (strlen(frames[i].name) != 0) {
            printf("%s: %s\n", frames[i].name, frames[i].value);
        } else {
            break;
        }
    }

    rewind(filePointer);

    for (int i = 0; i < 10; i++) {
        fputc(fgetc(filePointer), temp);
    }

    unsigned char pad[3] = {0x00, 0x00, 0x00};

    int size;

    for (int i = 0; i < 83; i++) {
        if (strcmp(frames[i].name, "") == 0) {
            break;
        }

        fprintf(temp, "%s", frames[i].name);
        fwrite(pad, sizeof(pad), 1, temp);

        size = strlen(frames[i].value) + 1;
        fprintf(temp, "%c", size);

        fwrite(pad, sizeof(pad), 1, temp);
        fprintf(temp, "%s", frames[i].value);
    }

    fprintf(temp, "\n");

    int ch;

    while ((ch = fgetc(filePointer)) != '\n') {
        // pass
    }

    while ((ch = fgetc(filePointer)) != EOF) {
        fputc(ch, temp);
    }

    fclose(temp);
    fclose(filePointer);

    FILE *tmpInput = fopen("tmp.txt", "rb");
    FILE *result = fopen(output, "wb");

    while ((ch = fgetc(tmpInput)) != EOF) {
        fputc(ch, result);
    }
}

void showFrames(FILE *filePointer) {
    int frameCount = 0;
    struct frame *frames = getFrames(filePointer);

    for (int i = 0; i < 83; i++) {
        if (strlen(frames[i].name) != 0) {
            printf("%s: %s\n", frames[i].name, frames[i].value);
        } else {
            break;
        }
    }
}

void getFrame(FILE *filePointer, char *frameName) {
    struct frame *frames = getFrames(filePointer);

    for (int i = 0; i < 83; i++) {
        if (strcmp(frames[i].name, frameName) == 0) {
            printf("%s: %s\n", frameName, frames[i].value);
        }
    }
}