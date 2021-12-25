#include <stdio.h>
#include <string.h>

unsigned int decodeData(unsigned char size[4]) {
    unsigned int res = 0;
    for (int i = 0; i < 4; i++)
        res += size[3 - i] << (8 * i);
    return res;
}

void encodeData(unsigned int size, unsigned char *res) {
    for (int i = 0; i < 4; i++) {
        res[i] = (size << (8 * i)) >> 24;
    }
}

void arcCreate(FILE *archive, char *filename) {
    char byte;
    FILE *file;
    file = fopen(filename, "rb");
    if (file == NULL) {
        fprintf(stderr, "\nError opening file: %s", filename);
    } else {
        char codedFilenameSize[4];
        int filenameSize = strlen(filename);
        encodeData(filenameSize, codedFilenameSize);

        fseek(file, 0, SEEK_END);
        unsigned long long filesize = ftell(file);
        fseek(file, 0, SEEK_SET);
        char codedFilesize[4];
        encodeData(filesize, codedFilesize);

        fwrite(codedFilenameSize, 1, sizeof(codedFilenameSize), archive);
        fwrite(filename, 1, filenameSize, archive);
        fwrite(codedFilesize, 1, sizeof(codedFilesize), archive);
        for (int i = 0; i < filesize; i++) {
            byte = fgetc(file);
            fputc(byte, archive);
        }

        printf("\nSuccessfully added: %s", filename);
        fclose(file);
        remove(filename);
    }
}

void arcList(FILE *archive) {
    int counter = 0;
    char byte = fgetc(archive);
    fseek(archive, 0, SEEK_SET);
    while (!feof(archive)) {
        if (byte == EOF)
            break;

        char codedFilenameSize[4];
        fread(codedFilenameSize, 1, sizeof(codedFilenameSize), archive);
        unsigned filenameSize = decodeData(codedFilenameSize);

        char filename[filenameSize];
        fread(filename, 1, filenameSize, archive);

        char codedFilesize[4];
        fread(codedFilesize, 1, sizeof(codedFilesize), archive);
        unsigned long long fileSize = decodeData(codedFilesize);
        fseek(archive, fileSize, SEEK_CUR);
        counter++;
        printf("%d. %s\n", counter, filename);

        byte = fgetc(archive);
        fseek(archive, -1, SEEK_CUR);

    }
}

void arcExtract(FILE *archive) {
    FILE *file;
    char byte;
    byte = fgetc(archive);
    fseek(archive, 0, SEEK_SET);
    while (!feof(archive)) {
        if (byte == EOF)
            break;

        char codedFilenameSize[4];
        fread(codedFilenameSize, 1, sizeof(codedFilenameSize), archive);
        int filenameSize = decodeData(codedFilenameSize);

        char filename[filenameSize + 1];
        fread(filename, 1, filenameSize, archive);
        filename[filenameSize] = '\0';
        file = fopen(filename, "wb");

        char codedFilesize[4];
        fread(codedFilesize, 1, sizeof(codedFilesize), archive);
        unsigned long long filesize = decodeData(codedFilesize);

        for (int i = 0; i < filesize; i++) {
            byte = fgetc(archive);
            fputc(byte, file);
        }
        byte = fgetc(archive);
        fseek(archive, -1, SEEK_CUR);
        printf("\nSuccessfully extracted: %s", filename);
    }
    fclose(archive);
}


int main(int argc, char **argv) {
    FILE *archive ;
    char *archiveName;
    if (argc < 4) {
        fprintf(stderr, "Not enough arguments");
        return 1;
    }
    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "--file")) {
            archiveName = argv[i + 1];
        }

        if (!strcmp(argv[i], "--create")) {
            printf("Creating archive %s\n", archiveName);
            archive = fopen(archiveName, "ab");
            for (int i = 4; i < argc; i++) {
                arcCreate(archive, argv[i]);
            }
            fclose(archive);
        }
        if (!strcmp(argv[i], "--list")) {
            archive = fopen(archiveName, "rb");
            if (archive == NULL) {
                fprintf(stderr, "Error opening archive");
            } else {
                printf("Showing files in %s\n", archiveName);
                arcList(archive);
                fclose(archive);
            }
        }
        if (!strcmp(argv[i], "--extract")) {
            archive = fopen(archiveName, "rb");
            if (archive == NULL) {
                fprintf(stderr, "Error opening archive");
            } else {
                printf("Extracting files from %s\n", archiveName);
                arcExtract(archive);
                fclose(archive);
                remove(archiveName);
            }
        }
    }
    return 0;
}