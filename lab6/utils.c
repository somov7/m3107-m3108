#include "utils.h"

const int BYTES_NUMBER = 8;
const int BUFFER_SIZE = 4096;

long int getSize(FILE *filePointer) {
    fseek(filePointer, 0L, SEEK_END);
    long int res = ftell(filePointer);
    rewind(filePointer);

    return res;
}

void create(int filesCount, FILE **files, char **filenames, char *filename) {
    FILE *resultFile = fopen(filename, "wb");
    long size;
    unsigned long filenameSize;

    fputc(filesCount, resultFile);

    for (int i = 0; i < filesCount; i++) {
        size = getSize(files[i]);
        filenameSize = strlen(filenames[i]);

        fputc((int) filenameSize, resultFile);

        fwrite(filenames[i], filenameSize, 1, resultFile);

        fwrite(&size, BYTES_NUMBER, 1, resultFile);

        char buf[BUFFER_SIZE];

        for (int j = 0; j < size / BUFFER_SIZE; j++) {
            fread(buf, BUFFER_SIZE, 1, files[i]);
            fwrite(buf, BUFFER_SIZE, 1, resultFile);
        }

        fread(buf, size % BUFFER_SIZE, 1, files[i]);
        fwrite(buf, size % BUFFER_SIZE, 1, resultFile);

        fclose(files[i]);
    }

    fclose(resultFile);
}

void extract(FILE *filePointer) {
    int filesCount = fgetc(filePointer);

    for (int i = 0; i < filesCount; i++) {
        int filenameSize = fgetc(filePointer);
        char *filename = malloc(filenameSize);

        for (int j = 0; j < filenameSize; j++) {
            filename[j] = (char) fgetc(filePointer);
        }

        FILE *extractingFile = fopen(filename, "wb");

        int fileSize = 0;

        for (int j = 0; j < BYTES_NUMBER; j++) {
            fileSize += fgetc(filePointer) << (8 * j);
        }

        char buf[BUFFER_SIZE];

        for (int j = 0; j < fileSize / BUFFER_SIZE; j++) {
            fread(buf, BUFFER_SIZE, 1, filePointer);
            fwrite(buf, BUFFER_SIZE, 1, extractingFile);
        }

        fread(buf, fileSize % BUFFER_SIZE, 1, filePointer);
        fwrite(buf, fileSize % BUFFER_SIZE, 1, extractingFile);

        fclose(extractingFile);
    }

    fclose(filePointer);
}

void add(FILE *filePointer, FILE *addFile, char *filename) {
    int filesCount = fgetc(filePointer);

    fseek(filePointer, 0, SEEK_SET);
    fprintf(filePointer, "%c", filesCount + 1);

    fseek(filePointer, 0, SEEK_END);

    long size = getSize(addFile);

    fputc((int) strlen(filename), filePointer);

    fwrite(filename, strlen(filename), 1, filePointer);

    fwrite(&size, BYTES_NUMBER, 1, filePointer);

    char buf[BUFFER_SIZE];

    for (int j = 0; j < size / BUFFER_SIZE; j++) {
        fread(buf, BUFFER_SIZE, 1, addFile);
        fwrite(buf, BUFFER_SIZE, 1, filePointer);
    }

    fread(buf, size % BUFFER_SIZE, 1, addFile);
    fwrite(buf, size % BUFFER_SIZE, 1, filePointer);

    fclose(addFile);
    fclose(filePointer);
}

void printList(FILE *filePointer) {
    int filesCount = fgetc(filePointer);

    printf("Files in archive: %d\n", filesCount);

    for (int i = 0; i < filesCount; i++) {
        int filenameSize = fgetc(filePointer);
        char *filename = malloc(filenameSize);

        for (int j = 0; j < filenameSize; j++) {
            filename[j] = (char) fgetc(filePointer);
        }

        int fileSize = 0;

        for (int j = 0; j < BYTES_NUMBER; j++) {
            fileSize += fgetc(filePointer) << (8 * j);
        }

        fseek(filePointer, fileSize, SEEK_CUR);

        printf("%s\t%d bytes\n", filename, fileSize);
    }

    fclose(filePointer);
}
