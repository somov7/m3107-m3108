#include <stdio.h>
#include <stdlib.h>
#include <iso646.h>
#include <string.h>
#include <locale.h>

#define headerBytes 10 // кол-во байт занимаемых хэддером
#define frameBytes 11 // кол-во байт занимаемых фрэймом

#pragma pack(push, 1)
typedef struct id3v2Header {
    char identifier[3];
    char version[2];
    char flag;
    char size[4];
} id3v2Header;

typedef struct id3v2Frame {
    char frameId[4];
    char size[4];
    char flag[2];
    char unicode;
} id3v2Frame;
#pragma pack(pop)

id3v2Header header;
id3v2Frame frame;

void printUsage(char *err) {
    printf("Error: %s\n", err);
    printf("Correct usage: progName.exe --filepath=<path> --command\n==============\n");
    printf("Command list: \n--show\n--set=<propname> --value=<propvalue>\n--get=<propname>\n==============");
}

void openCorrect(FILE *file) {
    if (file == NULL) {
        printUsage("Can't open a file");
        exit(2);
    }
}

//сдвигаем на 7i так как 7-ой байт не используется
int bytesToInt(char byte[4], int is_header){ //байты в инт
    if (header.version[0] == 4 || is_header)
        return byte[0] << 21 | byte[1] << 14 | byte[2] << 7 | byte[3];
    return byte[0] << 24 | byte[1] << 16 | byte[2] << 8 | byte[3];
}

void itob(int x, char* ch, int is_header) {
    int bits = 127;
    if (header.version[0] == 4 || is_header) {
        ch[3] = (char)(x & bits);
        ch[2] = (char)((x >> 7) & bits);
        ch[1] = (char)((x >> 14) & bits);
        ch[0] = (char)((x >> 21) & bits);
    }
    else {
        ch[3] = (char)(x & bits);
        ch[2] = (char)((x >> 8) & bits);
        ch[1] = (char)((x >> 16) & bits);
        ch[0] = (char)((x >> 24) & bits);
    }
}

void show(char *filepath) {
    FILE *fin = fopen(filepath, "rb");
    openCorrect(fin);

    fread(&header, sizeof(char), headerBytes, fin);
    int headerSizeInt = bytesToInt(header.size, 1);
    printf("struct size: %d\n", headerSizeInt);
    fread(&frame, 1, frameBytes, fin);

    while (frame.frameId[0] != 0 and ftell(fin) < headerSizeInt) {
        int frameTextSize = bytesToInt(frame.size, 0);
        char *frameText = calloc(frameTextSize, 1);
        fgets(frameText, frameTextSize, fin);

        printf("id: %5s || size: %5d || value: ", frame.frameId, frameTextSize);
        for (int i = 0; i < frameTextSize; i++)
            printf("%c", frameText[i]);
        printf("\n");

        fread(&frame, 1, frameBytes, fin);
        free(frameText);
    }
}

void get(char *filepath, char* id){
    FILE *fin = fopen(filepath, "rb");
    openCorrect(fin);

    fread(&header, sizeof(char), headerBytes, fin);
    int headerSizeInt = bytesToInt(header.size, 1);
    fread(&frame, 1, frameBytes, fin);

    while (frame.frameId[0] != 0 and ftell(fin) < headerSizeInt) {
        int frameTextSize = bytesToInt(frame.size, 0);
        char *frameText = calloc(frameTextSize, 1);
        fgets(frameText, frameTextSize, fin);

        if (!strcmp(frame.frameId, id)){
            printf("id: %5s || size: %5d || value: ", frame.frameId, frameTextSize);
            for (int i = 0; i < frameTextSize; i++)
                wprintf(L"%c", frameText[i]);
            printf("\n");
            break;
        }

        fread(&frame, 1, frameBytes, fin);
        free(frameText);
    }
}


void set(char *filepath, char* propName, char* propValue) {

}

int main(int argc, char **argv) {

    if (argc != 3) {
        printUsage("Invalid value of arguments");
        return 1;
    }
    show("01.Barrel Of A Gun.mp3");
    //get("C:\\mp3Tool\\music\\black-wonderful-life.mp3", "TIT2");
}
