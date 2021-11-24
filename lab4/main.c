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
int bytesToInt(char byte[4], int is_header) { //байты в инт
    if (header.version[0] == 4 || is_header)
        return byte[0] << 21 | byte[1] << 14 | byte[2] << 7 | byte[3];
    return byte[0] << 24 | byte[1] << 16 | byte[2] << 8 | byte[3];
}

void itob(int x, char *ch, int is_header) {
    int bits = 127;
    if (header.version[0] == 4 || is_header) {
        ch[3] = (char) (x & bits);
        ch[2] = (char) ((x >> 7) & bits);
        ch[1] = (char) ((x >> 14) & bits);
        ch[0] = (char) ((x >> 21) & bits);
    } else {
        ch[3] = (char) (x & bits);
        ch[2] = (char) ((x >> 8) & bits);
        ch[1] = (char) ((x >> 16) & bits);
        ch[0] = (char) ((x >> 24) & bits);
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
        } else {
            printf("%s\n", frameText);
        }

        fread(&frame, 1, frameBytes, fin);
        free(frameText);
    }
}

int get(char *filepath, char *id, int print) {
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

        if (!strcmp(frame.frameId, id)) {

            if (print) {
                printf("id: %5s || size: %5d || value: ", frame.frameId, frameTextSize);
                if (frame.unicode) {
                    wprintf(L"%ls\n", frameText); // для юникод
                } else {
                    printf("%s\n", frameText);
                }
            }

            pos = ftell(fin) - frameTextSize - 10;
            return pos;     // возращает позицию начала нужного фрейма

        }

        fread(&frame, 1, frameBytes, fin);
        free(frameText);
    }

    return pos;
}


void set(char *filepath, char *propName, char *propValue) {
    FILE *fin = fopen(filepath, "rb");
    FILE *fout = fopen("temp", "wb");

    if (fin == NULL or fout == NULL) {
        printUsage("Can't open a file");
        exit(2);
    }

    int pos = get(filepath, propName, 0);
    fread(&header, 1, headerBytes, fin);        //считываем хеддер
    char *buf = calloc(pos - 10, 1);            //считываем всё от хеддера до нужного фрейма.
    fread(buf, 1, pos - 10, fin);

    id3v2Frame curFrame;
    fread(&curFrame, 1, frameBytes, fin);       // считываем нужный фрейм
    int frameSize = bytesToInt(curFrame.size, 0);

    fseek(fin, 0, SEEK_END);    // запоминаем конечную позицию для считывания
    int endPos = ftell(fin);
    fseek(fin, 0, SEEK_SET);
    fread(&header, 1, headerBytes, fin);

    int newHeaderSize =
            bytesToInt(header.size, 1) + ((int) strlen(propValue) - frameSize) + 1;  // нашли новый размер хэддера в инт
    itob(newHeaderSize, header.size, 1); // перевод в байты

    fwrite(&header, 1, headerBytes, fout);      // запись хеддера

    fread(buf, 1, pos - 10, fin); // перемещаем указатель до нужного фрейма
    fwrite(buf, 1, pos - 10, fout); // записываем всё до нужного фрейма

    fread(&curFrame, 1, frameBytes, fin);   // переместили указатель до начала значения фрейма
    char *frameText = calloc(frameSize, 1);
    fgets(frameText, frameSize, fin); //переместили указатель до конца значения фрейма

    itob((int) strlen(propValue) + 1, curFrame.size, 0); // меняем размер фрейма
    fwrite(&curFrame, 1, frameBytes, fout); // записали нужный фрейм
    fwrite(propValue, 1, (int) strlen(propValue), fout); // записали значение нужно фрейма

    int curPos = ftell(fin);
    int diff = endPos - curPos;

    uint8_t *lastBytes = malloc(diff);
    fread(lastBytes, 1, diff, fin);
    fwrite(lastBytes, 1, diff, fout);

}

void update(char *filepath, char *propName, char *propValue) {
    FILE *fin = fopen(filepath, "rb");
    FILE *fout = fopen("temp", "wb");

    if (fin == NULL or fout == NULL) {
        printUsage("Can't open a file");
        exit(2);
    }

    fseek(fin, 0, SEEK_END);    // запоминаем конечную позицию для считывания
    int endPos = ftell(fin);
    fseek(fin, 0, SEEK_SET);

    id3v2Frame newFrame;
    strcpy(newFrame.frameId, propName);
    itob((int) strlen(propValue) + 1, newFrame.size, 0);
    newFrame.flag[0] = 0;
    newFrame.flag[1] = 1;
    newFrame.unicode = 0;

    fread(&header, 1, headerBytes, fin);
    int headerSizeInt = bytesToInt(header.size, 1);
    fread(&frame, 1, frameBytes, fin);

    int newHeaderSize = headerSizeInt + 12 + (int) strlen(propValue);
    itob(newHeaderSize, header.size, 1);                    //  изменяем размер хеддера
    fwrite(&header, 1, headerBytes, fout);                          // записываем новый хеддер


    while (frame.frameId[0] != 0 and ftell(fin) < headerSizeInt) {  // находим позицию последнего фрейма

        int frameTextSize = bytesToInt(frame.size, 0);
        char *frameText = calloc(frameTextSize - 1, 1);
        fgets(frameText, frameTextSize, fin);

        fwrite(&frame, 1, frameBytes, fout);
        fwrite(frameText, 1, frameTextSize - 1, fout);

        fread(&frame, 1, frameBytes, fin);
        free(frameText);
    }

    fwrite(&newFrame, 1, frameBytes, fout);
    fwrite(propValue, 1, strlen(propValue), fout);

    int curPos = ftell(fin) - 10;
    int diff = endPos - curPos;

    uint8_t *lastBytes = malloc(diff);
    fread(lastBytes, 1, diff, fin);
    fwrite(lastBytes, 1, diff, fout);

    remove(filepath);
    rename("temp", filepath);

}

int main(int argc, char **argv) {

    setlocale(LC_ALL, "Russian");

    if (!(argc < 3 or argc > 4)) {
        printUsage("Invalid value of arguments");
        return 1;
    }

    strtok(argv[1], "=");
    char *filepath = strtok(NULL, "=");


    if (strstr(argv[2], "--show") != NULL) {
        show(filepath);

    } else if (strstr(argv[2], "--get") != NULL) {
        strtok(argv[2], "=");
        char *propName = strtok(NULL, "=");
        get(filepath, propName, 1);

    } else if (strstr(argv[2], "--set") != NULL) {
        strtok(argv[2], "=");
        char *propName = strtok(NULL, "=");
        strtok(argv[3], "=");
        char *propValue = strtok(NULL, "=");
        if (get(filepath, propName, 0) == -1) {
            update(filepath, propName, propValue);
        }
        else {
            set(filepath, propName, propValue);
        }

    } else {
        printUsage("unknown command");
    }


}
