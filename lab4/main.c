#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <math.h>

typedef struct {
    char id[3];
    char version[2];
    char flags;
    char size[4];
} id3Header;

typedef struct {
    char id[4];
    char size[4];
    char flags[2];
    char a;
} id3Frame;


void checkInput(unsigned short argc, FILE *file) {
    if (argc < 3 || argc > 4) {
        fprintf(stderr, "Invalid arguments");
        exit(1);
    }
    if (file == NULL) {
        fprintf(stderr, "Error opening file");
        exit(1);
    }
}

void intToBytes(int x, char* ch, int bit) {
    int mask = (int) pow(2, bit) - 1;
    ch[3] = (char)(x & mask);
    ch[2] = (char)((x >> bit) & mask);
    ch[1] = (char)((x >> 2 * bit) & mask);
    ch[0] = (char)((x >> 3 * bit) & mask);
}

int bytesToInt(char *ch, int bit) {
    return ((int)ch[0] << (3 * bit)) + ((int)ch[1] << (2 * bit)) + ((int)ch[2] << bit) + (int)ch[3];
}

void showTags(FILE* inputFile){
    id3Header header;
    id3Frame frame;
    char *frameValue;
    fread(&header, 1, 10, inputFile);
    int tagSize = bytesToInt(header.size, 7);
    while ((fread(&frame, 1, 11, inputFile)) && ftell(inputFile) <= tagSize) {
        int frameSize = bytesToInt(frame.size, 8);
        frameValue = (char *) malloc(frameSize);
        fgets(frameValue, frameSize, inputFile);
        if (bytesToInt(frame.id, 8) == 0)
            break;
        printf("Frame: %s, value: %s\n", frame.id, frameValue);
    }
}

void getTag(FILE* inputFile, char* prop_name){
    id3Header header;
    id3Frame frame;
    char *frameValue;
    fread(&header, 1, 10, inputFile);
    int tagSize = bytesToInt(header.size, 7);
    while ((fread(&frame, 1, 11, inputFile)) && ftell(inputFile) <= tagSize) {
        if (strcmp(frame.id, prop_name) == 0) {
            int frameSize = bytesToInt(frame.size, 8);
            frameValue = (char *) malloc(frameSize);
            fgets(frameValue, frameSize, inputFile);
            printf("Frame: %s, value: %s\n", frame.id, frameValue);
            return;
        }
    }
    printf("There is no %s tag in file", prop_name);
}

void setTag(FILE* inputFile, char* prop_name, char* prop_value){
    int pointPos, pointEnd, frameSizeNew, headerSizeNew;
    char *prevFrame, *nextFrame;
    id3Header header;
    id3Frame frame;

    fread(&header, 1, 10, inputFile);
    int tagSize = bytesToInt(header.size, 7);
    while ((fread(&frame, 1, 11, inputFile)) && ftell(inputFile) < tagSize) {
        char *frameValue;
        int frameSize = bytesToInt(frame.size, 8);
        frameValue = (char *) malloc(frameSize);
        fgets(frameValue, frameSize, inputFile);
        if (strncmp(frame.id, prop_name, 4) == 0) {

            pointPos = ftell(inputFile);
            fseek(inputFile, 0, SEEK_END);
            pointEnd = ftell(inputFile);

            frameSizeNew = strlen(prop_value) + 1;
            headerSizeNew = tagSize - frameSize + frameSizeNew;
            intToBytes(headerSizeNew, header.size, 7);
            intToBytes(frameSizeNew, frame.size, 8);

            fseek(inputFile, 10, SEEK_SET);
            prevFrame = (char *) malloc(pointPos - 21 - frameSize + 1);
            fread(prevFrame, 1, pointPos - 21 - (frameSize - 1), inputFile);
            fseek(inputFile, pointPos, SEEK_SET);
            nextFrame = (char *) malloc(pointEnd - pointPos + 1);
            fread(nextFrame, 1, pointEnd - pointPos + 1, inputFile);

            fseek(inputFile, 0, SEEK_SET);
            fwrite(&header, 1, 10, inputFile);
            fwrite(prevFrame, 1, pointPos - 21 - frameSize + 1, inputFile);
            fwrite(&frame, 1, 11, inputFile);
            fwrite(prop_value, 1, strlen(prop_value), inputFile);
            fwrite(nextFrame, 1, pointEnd - pointPos + 1, inputFile);

            free(prevFrame);
            free(nextFrame);
            return;
        }
    }
    id3Frame newFrame;

    fseek(inputFile, 0, SEEK_END);
    pointEnd = ftell(inputFile);
    frameSizeNew = strlen(prop_value) + 1;
    headerSizeNew = tagSize + frameSizeNew;
    intToBytes(headerSizeNew, header.size, 7);
    intToBytes(frameSizeNew, newFrame.size, 8);
    memcpy(newFrame.id, prop_name, 4);

    fseek(inputFile, 10, SEEK_SET);
    nextFrame = (char *) malloc(pointEnd - 10 + 1);
    fread(nextFrame, 1, pointEnd - 10 + 1, inputFile);

    fseek(inputFile, 0, SEEK_SET);
    fwrite(&header, 1, 10, inputFile);
    fwrite(&newFrame, 1, 11, inputFile);
    fwrite(prop_value, 1, strlen(prop_value), inputFile);
    fwrite(nextFrame, 1, pointEnd - 10 + 1, inputFile);

    free(nextFrame);
}


int main(int argc, char **argv) {
    setlocale(LC_ALL, "Russian");
    char *propName;
    char *propValue;

    if (argc == 1) {
        fprintf(stderr, "Input valid arguments");
        return 1;
    }

    strtok(argv[1], "=");
    char *filepath = strtok(NULL, "=");
    FILE* inputFile = fopen(filepath, "r+b");
    checkInput(argc, inputFile);

    if (strstr(argv[2], "--show") != NULL && argc == 3) {
        showTags(inputFile);

    } else if (strstr(argv[2], "--get") != NULL && argc == 3) {
        strtok(argv[2], "=");
        propName = strtok(NULL, "=");
        getTag(inputFile, propName);

    } else if (strstr(argv[2], "--set") != NULL && argc == 4) {
        strtok(argv[2], "=");
        propName = strtok(NULL, "=");
        strtok(argv[3], "=");
        propValue = strtok(NULL, "=");
        setTag(inputFile, propName, propValue);

    } else {
        fprintf(stderr, "Invalid arguments");
        return 1;
    }
}
