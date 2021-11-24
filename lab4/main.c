#include <stdio.h>
#include <stdlib.h>
#include <iso646.h>
#include <string.h>
#include <locale.h>
#include <inttypes.h>

const int headerBytes = 10; // кол-во байт занимаемых хэддером
const int frameBytes = 11; // кол-во байт занимаемых фрэймом

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
        if (frame.unicode) {
            wprintf(L"%ls\n", frameText + 2); // для юникод
        }
        else {
            printf("%s\n", frameText);
        }

        fread(&frame, 1, frameBytes, fin);
        free(frameText);
    }
}

int get(char *filepath, char* id){
    FILE *fin = fopen(filepath, "rb");
    openCorrect(fin);

    fread(&header, 1, headerBytes, fin);
    int headerSizeInt = bytesToInt(header.size, 1);
    fread(&frame, 1, frameBytes, fin);
    int pos = -1;

    while (frame.frameId[0] != 0 and ftell(fin) < headerSizeInt) {
        int frameTextSize = bytesToInt(frame.size, 0);
        char *frameText = calloc(frameTextSize, 1);
        fgets(frameText, frameTextSize, fin);

        if (!strcmp(frame.frameId, id)){
            printf("id: %5s || size: %5d || value: ", frame.frameId, frameTextSize);
            if (frame.unicode) {
                wprintf(L"%ls\n", frameText); // для юникод
            }
            else {
                printf("%s\n", frameText);
            }
            pos = ftell(fin) - frameTextSize - 10;
            return pos;     // возращает позицию начала нужного фрейма
        }

        fread(&frame, 1, frameBytes, fin);
        free(frameText);
    }

    return pos;
}


void set(char *filepath, char* propName, char* propValue) {
    FILE* fin = fopen(filepath, "rb");
    FILE* fout = fopen("temp", "wb");

    if (fin == NULL or fout == NULL){
        printUsage("Can't open a file");
        exit(2);
    }

    int pos = get(filepath, propName);
    fread(&header, 1, headerBytes, fin);        //считываем хеддер
    char* buf = calloc(pos - 10, 1);            //считываем всё от хеддера до нужного фрейма.
    fread(buf, 1, pos - 10, fin);

    id3v2Frame curFrame;
    fread(&curFrame, 1, frameBytes, fin);       // считываем нужный фрейм
    int frameSize = bytesToInt(curFrame.size, 0);

    fseek(fin, 0, SEEK_END);    // запоминаем конечную позицию для считывания
    int endPos = ftell(fin);
    fseek(fin, 0, SEEK_SET);
    fread(&header, 1, headerBytes, fin);

    int newHeaderSize = bytesToInt(header.size, 1) + ((int)strlen(propValue) - frameSize);  // нашли новый размер хэддера в инт
    itob(newHeaderSize, header.size, 1); // перевод в байты

    fwrite(&header, 1, headerBytes, fout);      // запись хеддера

    fread(buf, 1, pos - 10, fin); // перемещаем указатель до нужного фрейма
    fwrite(buf, 1, pos - 10, fout); // записываем всё до нужного фрейма

    fread(&curFrame, 1, frameBytes, fin);   // переместили указатель до начала значения фрейма
    char *frameText = calloc(frameSize, 1);
    fgets(frameText, frameSize, fin); //переместили указатель до конца значения фрейма

    itob((int)strlen(propValue), curFrame.size, 0); // меняем размер фрейма
    fwrite(&curFrame, 1, frameBytes, fout); // записали нужный фрейм
    fwrite(propValue, 1, (int)strlen(propValue), fout); // записали значение нужно фрейма

    int curPos = ftell(fin);
    int diff = endPos - curPos;

    uint8_t *lastBytes = malloc(diff);
    fread(lastBytes, 1, diff, fin);
    fwrite(lastBytes, 1, diff, fout);

}

void update();

int main(int argc, char **argv) {

    setlocale(LC_ALL, "Russian");

    if (argc != 3) {
        printUsage("Invalid value of arguments");
        return 1;
    }
    show("C:\\mp3Tool1\\music\\01.Barrel Of A Gun.mp3");
    printf("\n");
    set("C:\\mp3Tool1\\music\\01.Barrel Of A Gun.mp3", "TCON", "123");

}
