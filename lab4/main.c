#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

const int headerBytes = 10;
const int frameBytes = 11;

#pragma pack(push,1)
typedef struct{
    char headerId[3];
    char version[2];
    char flag;
    char size[4];
} id3v2Header;

typedef struct{
    char frameId[4];
    char size[4];
    char flag[2];
    char unicode;
} id3v2Frame;
#pragma pack(pop)

id3v2Header header;
id3v2Frame frame;

void fileCheck(FILE *file){
    if (file == NULL){
        printf("file == null >:C");
        exit(2);
    }
}

void itob(int x, char *ch, bool is_header) {
    int bits = 127;
    if (header.version[0] == 4 || is_header) {
        ch[3] = (char) (x & bits);
        ch[2] = (char) ((x >> 7) & bits);
        ch[1] = (char) ((x >> 14) & bits);
        ch[0] = (char) ((x >> 21) & bits);
    }
    else {
        ch[3] = (char) (x & bits);
        ch[2] = (char) ((x >> 8) & bits);
        ch[1] = (char) ((x >> 16) & bits);
        ch[0] = (char) ((x >> 24) & bits);
    }
}

int btoi(char byte[4], bool is_header) { //байты в инт
    if (header.version[0] == 4 || is_header)
        return byte[0] << 21 | byte[1] << 14 | byte[2] << 7 | byte[3];
    return byte[0] << 24 | byte[1] << 16 | byte[2] << 8 | byte[3];
}

void show(char *filepath){
    FILE *fin = fopen(filepath, "rb");
    fileCheck(fin);

    fread(&header, sizeof(char), headerBytes, fin);
    int headerSizeInt = btoi(header.size, true);
    fread(&frame, 1, frameBytes, fin);

    while (frame.frameId[0] != 0 && ftell(fin) < headerSizeInt) {
        int frameTextSize = btoi(frame.size, false);
        char *frameText = calloc(frameTextSize, 1);
        fgets(frameText, frameTextSize, fin);

        printf("id: %s, data: ", frame.frameId);
        if (frame.unicode) {
            wprintf(L"%ls\n", frameText + 2);
        }
        else {
            printf("%s\n", frameText);
        }

        fread(&frame, 1, frameBytes, fin);
        free(frameText);
    }

    fclose(fin);
}

int get(char *filepath, char *id, bool flag) {
    FILE *fin = fopen(filepath, "rb");
    fileCheck(fin);

    fread(&header, 1, headerBytes, fin);
    int headerSizeInt = btoi(header.size, true);
    fread(&frame, 1, frameBytes, fin);
    int position = -1;

    while (frame.frameId[0] != 0 && ftell(fin) < headerSizeInt) {
        int frameTextSize = btoi(frame.size, false);
        char *frameText = calloc(frameTextSize, 1);
        fgets(frameText, frameTextSize, fin);

        if (!strcmp(frame.frameId, id)) {
            if (flag) {
                printf("id: %s, data: ", frame.frameId);
                if (frame.unicode) {
                    wprintf(L"%ls\n", frameText);
                }
                else {
                    printf("%s\n", frameText);
                }
            }

            position = ftell(fin) - frameTextSize - 10;
            fclose(fin);
            return position;
        }

        fread(&frame, 1, frameBytes, fin);
    }
    fclose(fin);
    return position;
}

void set(char *filepath, char *propName, char *propValue) {


    FILE *fin = fopen(filepath, "rb");
    FILE *fout = fopen("newFile", "wb");

    fileCheck(fin);
    fileCheck(fout);

    int position = get(filepath, propName, false);
    fread(&header, 1, headerBytes, fin);
    char *buf = calloc(position - 10, 1);
    fread(buf, 1, position - 10, fin);

    id3v2Frame curFrame;
    fread(&curFrame, 1, frameBytes, fin);
    int frameSize = btoi(curFrame.size, false);

    fseek(fin, 0, SEEK_END);
    int endPos = ftell(fin);
    fseek(fin, 0, SEEK_SET);
    fread(&header, 1, headerBytes, fin);

    int newHeaderSize = btoi(header.size, true) + ((int) strlen(propValue) - frameSize) + 1;
    itob(newHeaderSize, header.size, true);

    fwrite(&header, 1, headerBytes, fout);

    fread(buf, 1, position - 10, fin);
    fwrite(buf, 1, position - 10, fout);

    fread(&curFrame, 1, frameBytes, fin);
    char *frameText = calloc(frameSize, 1);
    fgets(frameText, frameSize, fin);

    itob((int) strlen(propValue) + 1, curFrame.size, false);
    fwrite(&curFrame, 1, frameBytes, fout);
    fwrite(propValue, 1, (int) strlen(propValue), fout);

    int curPos = ftell(fin);
    int diff = endPos - curPos;

    char *lastBytes = malloc(diff);
    fread(lastBytes, 1, diff, fin);
    fwrite(lastBytes, 1, diff, fout);

    fclose(fin);
    remove(filepath);
    fclose(fout);
    rename("newFile", filepath);

}

void update(char *filepath, char *propName, char *propValue) {

    FILE *fin = fopen(filepath, "rb");
    FILE *fout = fopen("newfile", "wb");

    fileCheck(fin);
    fileCheck(fout);

    fseek(fin, 0, SEEK_END);
    int endPos = ftell(fin);
    fseek(fin, 0, SEEK_SET);

    id3v2Frame newFrame;
    strcpy(newFrame.frameId, propName);
    itob((int) strlen(propValue) + 1, newFrame.size, 0);
    newFrame.flag[0] = 0;
    newFrame.flag[1] = 1;
    newFrame.unicode = 0;

    fread(&header, 1, headerBytes, fin);
    int headerSizeInt = btoi(header.size, true);
    fread(&frame, 1, frameBytes, fin);

    int newHeaderSize = headerSizeInt + 12 + (int) strlen(propValue);
    itob(newHeaderSize, header.size, true);
    fwrite(&header, 1, headerBytes, fout);


    while (frame.frameId[0] != 0 && ftell(fin) < headerSizeInt) {

        int frameTextSize = btoi(frame.size, false);
        char *frameText = calloc(frameTextSize - 1, 1);
        fgets(frameText, frameTextSize, fin);

        fwrite(&frame, 1, frameBytes, fout);
        fwrite(frameText, 1, frameTextSize - 1, fout);

        fread(&frame, 1, frameBytes, fin);
    }

    fwrite(&newFrame, 1, frameBytes, fout);
    fwrite(propValue, 1, strlen(propValue), fout);

    int curPos = ftell(fin) - 10;
    int diff = endPos - curPos;

    char *lastBytes = malloc(diff);
    fread(lastBytes, 1, diff, fin);
    fwrite(lastBytes, 1, diff, fout);

    fclose(fin);
    remove(filepath);
    fclose(fout);
    rename("newfile", filepath);

}



int main(int argc, char *argv[]){
    if (argc < 3 || argc > 4){
        printf("Invalid amount of arguments >:C");
        return 1;
    }

    strtok(argv[1], "=");
    char *filepath = strtok(NULL, "=");

    if (strstr(argv[2], "--show") != NULL) {
        show(filepath);
    }
    else if (strstr(argv[2], "--get") != NULL) {
        strtok(argv[2], "=");
        char *propName = strtok(NULL, "=");
        get(filepath, propName, true);

    }
    else if (strstr(argv[2], "--set") != NULL) {
        strtok(argv[2], "=");
        char *propName = strtok(NULL, "=");
        strtok(argv[3], "=");
        char *propValue = strtok(NULL, "=");
        if (get(filepath, propName, false) == -1) {
            update(filepath, propName, propValue);
        }
        else {
            set(filepath, propName, propValue);
        }
    }
    else{
        printf("Invalid command >:C\nCorrect input format:\napp.exe --filepath=Song.mp3 --command\n");
    }

    return 0;
}
