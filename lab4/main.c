#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char mask = 0x7f;

enum Options {
    SHOW_OPTION = 1,
    GET_OPTION,
    SET_OPTION
};

enum ErrorsTypes {
    INCORRECT_OPTION_COUNT = 1,
    INCORRECT_FILEPATH,
    INCORRECT_OPTION,
    INCORRECT_GET_OPTION,
    INCORRECT_SET_OPTION,
    INCORRECT_TAG,
    TAG_NOT_EXIST,
    FILE_NOT_FOUND,
    TAG_HAS_NOT_FRAMES,
    FRAME_NOT_EXIST,
    WRITE_ERROR
};

void error(char errorType) {
    printf("Error: ");
    switch (errorType) {
        case INCORRECT_OPTION_COUNT:
            printf("Incorrect options count\n");
            exit(INCORRECT_OPTION_COUNT);
        case INCORRECT_FILEPATH:
            printf("Incorrect filepath\n");
            exit(INCORRECT_FILEPATH);
        case INCORRECT_OPTION:
            printf("Incorrect options. Use --filepath <filepath> --show/--get=XXXX/(--set=XXXX --value=<value>)\n");
            exit(INCORRECT_OPTION);
        case INCORRECT_GET_OPTION:
            printf("Incorrect --get option\n");
            exit(INCORRECT_GET_OPTION);
        case INCORRECT_SET_OPTION:
            printf("Incorrect --set option\n");
            exit(INCORRECT_SET_OPTION);
        case INCORRECT_TAG:
            printf("Incorrect tag. Use tags: TXXX, WXXX, COMM\n");
            exit(INCORRECT_TAG);
        case TAG_NOT_EXIST:
            printf("ID3 Tag not exist\n");
            exit(TAG_NOT_EXIST);
        case FILE_NOT_FOUND:
            printf("File not found\n");
            exit(FILE_NOT_FOUND);
        case TAG_HAS_NOT_FRAMES:
            printf("ID3 tag hasn't any frames\n");
            exit(TAG_HAS_NOT_FRAMES);
        case FRAME_NOT_EXIST:
            printf("Frame with this tag not exist\n");
            exit(FRAME_NOT_EXIST);
        case WRITE_ERROR:
            printf("Can't write tag\n");
            exit(WRITE_ERROR);
        default:
            printf("404");
            exit(404);
    }
}

FILE *getFile(char *filename) {
    FILE *input;
    input = fopen(filename, "r+b");
    if (input == NULL)
        error(FILE_NOT_FOUND);
    return input;
}

unsigned char* toSyncSize(unsigned size) {
    unsigned char *bytes = malloc(4);
    bytes[0] = size >> 21 & mask;
    bytes[1] = size >> 14 & mask;
    bytes[2] = size >> 7 & mask;
    bytes[3] = size & mask;
    return bytes;
}

unsigned findFrames(FILE *input, const char *frameName) {
    char nameInBytes[5], sizeInBytes[6];
    nameInBytes[4] = '\0';
    fread(nameInBytes, sizeof (nameInBytes)-1, 1, input);
    fread(sizeInBytes, sizeof (sizeInBytes), 1, input);
    if (!nameInBytes[0])
        return 0;
    int size = (sizeInBytes[0] << 21) + (sizeInBytes[1] << 14) + (sizeInBytes[2] << 7) + sizeInBytes[3];
    unsigned char buf[size];
    char isFind = 1;
    for (int i = 0; i < 4; ++i) {
        if (frameName[i] != nameInBytes[i]) {
            isFind = 0;
            break;
        }
    }
    if (isFind)
        return size;
    else
        fread(buf, size, 1, input);
    return findFrames(input, frameName);
}

unsigned showFrames(FILE *input, unsigned count) {
    char byte[10];
    fread(byte, sizeof (byte), 1, input);
    if (!byte[0])
        return count;
    int size = (byte[4] << 21) + (byte[5] << 14) + (byte[6] << 7) + byte[7];
    printf("%c%c%c%c:\t", byte[0], byte[1], byte[2], byte[3]);
    unsigned char buf[size + 1];
    if (byte[0] == 'T' || byte[0] == 'C' || byte[0] == 'W') {
        for (int i = 0; i < size; ++i)
            buf[i] = fgetc(input);
        buf[size] = '\0';
        for (int i = 0; i < size + 1; ++i)
            printf("%c", buf[i]);
    } else
        fread(buf, size, 1, input);
    printf("\n");
    showFrames(input, count + 1);
}

char getFrame(FILE *input, char *frameName) {
    long size = findFrames(input, frameName);
    if (size) {
        printf("%s:\t", frameName);
        unsigned char buf[size+1];
        if (frameName[0] == 'T' || frameName[0] == 'C' || frameName[0] == 'W') {
            for (int i = 0; i < size; ++i)
                buf[i] = fgetc(input);
            buf[size] = '\0';
            for (int i = 0; i < size + 1; ++i)
                printf("%c", buf[i]);
        }
        printf("\n");
        return 1;
    }
    return 0;
}

void skipExternalHeader(FILE *input) {
    char byte[6];
    fread(byte, sizeof(byte), 1, input);
    unsigned size = (byte[0] << 21) + (byte[1] << 14) + (byte[2] << 7) + byte[3];
    char skipBuf[size];
    fread(skipBuf, sizeof(skipBuf), 1, input);
}

// for debug
unsigned printTag(FILE *input) {
    char byte[10];
    fread(byte, sizeof(byte), 1, input);
    if (byte[0] != 'I' && byte[1] != 'D' && byte[2] != '3')
        return 0;
    printf("%c%c%cv2.%u.%u\n", byte[0], byte[1], byte[2], byte[3], byte[4]);
    unsigned size = (byte[6] << 21) + (byte[7] << 14) + (byte[8] << 7) + byte[9];
    char flags[4];
    byte[5] /= 16;
    for (int i = 3; i >= 0; --i) {
        flags[i] = byte[5]%2;
        byte[5] /= 2;
    }
    printf("Flags: a:%d, b:%d, c:%d, d:%d. Size of tag: %u\n", flags[0], flags[1], flags[2], flags[3], size);
    if (flags[1])
        skipExternalHeader(input);
    return size;
}

char writeTag(FILE *output, char *frameName, char *content, unsigned tagSize) {
    unsigned size = findFrames(output, frameName);
    fseek(output, -10, SEEK_CUR);
    if (size) {
        char bytes[size+10];
        fread(bytes, sizeof(bytes), 1, output);
    }
    FILE *tmp = tmpfile();
    while (!feof(output))
        if (fputc(fgetc(output), tmp) == EOF)
            return 0;
    fseek(tmp, 0, SEEK_SET);
    fseek(output, 0, SEEK_SET);
    unsigned long contentSize = strlen(content);
    if (size != contentSize) {
        unsigned futureTagSize = tagSize-size+contentSize;
        unsigned char *bytes = toSyncSize(futureTagSize);
        fseek(output, 6, SEEK_SET);
        for (int i = 0; i < 4; ++i)
            if (fputc(bytes[i], output) == EOF)
                return 0;
        free(bytes);
    }
    findFrames(output, frameName);
    fseek(output, -10, SEEK_CUR);
    if (fputs(frameName, output) == EOF)
        return 0;
    unsigned long strSize = strlen(content);
    unsigned char *bytes = toSyncSize(strSize);
    for (int i = 0; i < 4; ++i)
        if (fputc(bytes[i], output) == EOF)
            return 0;
    free(bytes);
    fseek(output, 2, SEEK_CUR);
    if (fputs(content, output) == EOF)
        return 0;
    while (!feof(tmp))
        if (fputc(fgetc(tmp), output) == EOF)
            return 0;
    fclose(tmp);
    return 1;
}

void checkTextTag(char *tag) {
    if (tag[0] != 'T' && tag[0] != 'W' && strcmp(tag, "COMM"))
        error(INCORRECT_TAG);
}

void solve(FILE *file, char command, char *tag, char *value) {
    unsigned tagSize = printTag(file);
    if (!tagSize)
        error(TAG_NOT_EXIST);
    switch (command) {
        case SHOW_OPTION: {
            if (!showFrames(file, 0))
                error(TAG_HAS_NOT_FRAMES);
            break;
        }
        case GET_OPTION: {
            if (!getFrame(file, tag))
                error(FRAME_NOT_EXIST);
            break;
        }
        case SET_OPTION: {
            if (!writeTag(file, tag, value, tagSize))
                error(WRITE_ERROR);
            printf("Tag updated\n");
        }
    }
    fclose(file);
}

int main(int argc, char **argv) {
    char *filepath, *value, *tag;
    if (argc < 3)
        error(INCORRECT_OPTION_COUNT);
    if (argc == 3) {
        unsigned long filepathSize = strlen(argv[1]);
        if (filepathSize < 12)
            error(INCORRECT_FILEPATH);
        char *tmpStr = malloc(11);
        strncpy(tmpStr, argv[1], 11);
        if (strcmp(tmpStr, "--filepath="))
            error(INCORRECT_OPTION);
        filepathSize -= 11;
        filepath = malloc(filepathSize);
        strncpy(filepath, argv[1]+11, filepathSize);
        unsigned long commandSize = strlen(argv[2]);
        if (commandSize < 6)
            error(INCORRECT_OPTION);
        char *tmpStr2 = malloc(6);
        strncpy(tmpStr2, argv[2], 6);
        if (!strcmp(argv[2], "--show")) {
            solve(getFile(filepath), SHOW_OPTION, NULL, NULL);
        } else if (!strcmp(tmpStr2, "--get=")) {
            if (commandSize != 10)
                error(INCORRECT_GET_OPTION);
            tag = malloc(4);
            strncpy(tag, argv[2]+6, 4);
            checkTextTag(tag);
            solve(getFile(filepath), GET_OPTION, tag, NULL);
        } else
            error(INCORRECT_OPTION);
        free(tmpStr);
        free(tmpStr2);
    } else {
        unsigned long filepathSize = strlen(argv[1]);
        if (filepathSize < 12)
            error(INCORRECT_FILEPATH);
        char *tmpStr = malloc(11);
        strncpy(tmpStr, argv[1], 11);
        if (strcmp(tmpStr, "--filepath="))
            error(INCORRECT_OPTION);
        filepathSize -= 11;
        filepath = malloc(filepathSize);
        strncpy(filepath, argv[1]+11, filepathSize);
        unsigned long tagSize = strlen(argv[2]);
        if (tagSize != 10)
            error(INCORRECT_OPTION);
        char *tmpStr2 = malloc(6);
        strncpy(tmpStr2, argv[2], 6);
        if (strcmp(tmpStr2, "--set="))
            error(INCORRECT_OPTION);
        tag = malloc(4);
        strncpy(tag, argv[2]+6, 4);
        checkTextTag(tag);
        unsigned long valueSize = strlen(argv[3]);
        if (valueSize < 9)
            error(INCORRECT_OPTION);
        char *tmpStr3 = malloc(6);
        strncpy(tmpStr3, argv[3], 8);
        if (strcmp(tmpStr3, "--value="))
            error(INCORRECT_OPTION);
        valueSize -= 8;
        value = malloc(valueSize);
        strncpy(value, argv[3]+8, valueSize);
        for (int i = 4; i < argc; ++i) {
            unsigned long prevValueSize = strlen(value);
            unsigned long newValueSize = strlen(argv[i]);
            value = realloc(value, prevValueSize+newValueSize+2);
            value[prevValueSize] = ' ';
            for (unsigned long j = prevValueSize+1; j <= prevValueSize+newValueSize; ++j)
                value[j] = argv[i][j-prevValueSize-1];
            value[prevValueSize+newValueSize+1] = '\0';
        }
        solve(getFile(filepath), SET_OPTION, tag, value);
        free(tmpStr);
        free(tmpStr2);
        free(tmpStr3);
    }
    return 0;
}