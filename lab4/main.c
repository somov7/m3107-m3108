#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(1)


unsigned int convertEndians(unsigned int val) {
    return ((val >> 24) & 0x0000007F) |
           ((val >> 8) & 0x00007F00) |
           ((val << 8) & 0x007F0000) |
           ((val << 24) & 0x7F000000);
}

unsigned int fromMP3Size(unsigned int val) {
    return ((val) & 0x0000007F) |
           ((val >> 8) << 7 & 0x00003FA0) |
           ((val >> 16) << 14 & 0x001FB000) |
           ((val >> 24) << 21 & 0x0FE00000);
}

unsigned int toMP3Size(unsigned int val) {
    return ((val) & 0x0000007F) |
           ((val << 1) & 0x00007F00) |
           ((val << 2) & 0x007F0000) |
           ((val << 3) & 0x7F000000);
}


void copyFileE(FILE *from, FILE *to, unsigned int offsetFrom, unsigned int offsetTo) {
    unsigned char c;
    fseek(from, offsetFrom, SEEK_SET);
    fseek(to, offsetTo, SEEK_SET);

    while (fread(&c, sizeof(unsigned char), 1, from) == 1) {
        fwrite(&c, sizeof(unsigned char), 1, to);
    }
}

void copyFileP(FILE *from, FILE *to, unsigned int offsetFrom, unsigned int offsetTo, int length) {
    unsigned char c;
    fseek(from, offsetFrom, SEEK_SET);
    fseek(to, offsetTo, SEEK_SET);

    while (fread(&c, sizeof(unsigned char), 1, from) == 1 && length > 0) {
        fwrite(&c, sizeof(unsigned char), 1, to);
        length--;
    }
}

typedef union fileHeader {
    unsigned char buffer[10];

    struct {
        unsigned char marker[3];
        unsigned char version;
        unsigned char subVersion;
        unsigned char flags;
        unsigned int size;
    } data;
} fileHeader;

typedef union frameHeader {
    unsigned char buffer[10];

    struct {
        unsigned char id[4];
        unsigned int size;
        unsigned short flags;
    } data;
} frameHeader;

unsigned char *toString(char *notString, int length) {
    unsigned char *string = malloc(length * sizeof(char));
    memcpy(string, notString, length);
    string[length] = '\0';
    return string;
}

// Парсим содержимое тега в строку
char *parseFrameContents(char *frameContents, int length) {
    unsigned char *string;

    if (frameContents[0] == 1) {
        int offset, i;
        length = (length - 3) / 2;
        offset = 3;
        string = malloc((length) * sizeof(char));
        for (i = 0; i < length; i++) {
            string[i] = frameContents[2 * i + offset];
        }
        return string;
    } else if (frameContents[0] == 0) {
        string = malloc((length - 2) * sizeof(char));
        strncpy(string, frameContents + 1, length - 2);
        return toString(string, length - 2);
    } else {
        return toString(frameContents, length);
    }
}

char *stringToTag(char *string, unsigned int *frameSizeNew) {
    unsigned int stringLength = strlen(string) + 1;
    unsigned char *tag;

        int offset;
        char UTF16LEPrefix[3] = {1, 255, 254};
        *frameSizeNew = 2 * stringLength + 3;
        offset = 3;
        tag = calloc(*frameSizeNew, sizeof(char));
        memcpy(tag, UTF16LEPrefix, 3);
        for (int i = 0; i < stringLength; i++) {
            tag[offset + 2 * i] = string[i];
        }
        return tag;


}

void show(char *fileName) {
    FILE *file;
    if ((file = fopen(fileName, "rb")) == NULL) {  // Открыли двоичный файл для чтения
        printf("File is not exist.");
        exit(-1);
    }
    fseek(file, 0, SEEK_SET); // Устанавливаем указатель на начало файла

    fileHeader fileHeader;
    fread(fileHeader.buffer, sizeof(char), 10, file); // Читаем первые 10 байт в буффер

    unsigned int tagSize = fromMP3Size(convertEndians(fileHeader.data.size)); // Считаем tag size

    printf("%sv2.%d(%d) Tag size:%d bytes\n", toString(fileHeader.data.marker, 3), fileHeader.data.version,
           fileHeader.data.subVersion, tagSize);

    if (fileHeader.data.version != 3) { // Проверяем версию
        printf("Unsupported version of ID3. Only ID3v2.3 is supported.");
        exit(-1);
    }

    while (ftell(file) < tagSize + 10) { // Читаем теги
        frameHeader frameHeader;
        fread(frameHeader.buffer, sizeof(char), 10, file); // Читаем frameHeader в буффер

        unsigned int frameSize = convertEndians(frameHeader.data.size); // Считаем длину фрейма

        if (frameSize == 0) {
            break;
        }

        unsigned char *frameContents = malloc(frameSize * sizeof(char));
        fread(frameContents, sizeof(char), frameSize, file);  // Читаем frame data

        printf("%s: %s \n", toString(frameHeader.data.id, 4), parseFrameContents(frameContents, frameSize));

        free(frameContents);
    }
    fclose(file);
}

void get(char *fileName, char *tagName) {
    FILE *file;
    if ((file = fopen(fileName, "rb")) == NULL) {  // Открыли двоичный файл для чтения
        printf("File is not exist.");
        exit(-1);
    }

    fseek(file, 0, SEEK_SET);

    fileHeader fileHeader;
    fread(fileHeader.buffer, sizeof(char), 10, file);

    unsigned int tagSize = fromMP3Size(convertEndians(fileHeader.data.size));

    if (fileHeader.data.version != 3) {
        printf("Unsupported version of ID3. Only ID3v2.3 is supported.");
        exit(-1);
    }

    int searchFlag = 1;  // Флаг нашли ли мы искомый тег
    while (ftell(file) < tagSize + 10 && searchFlag) {
        union frameHeader frameHeader;

        fread(frameHeader.buffer, sizeof(char), 10, file);

        unsigned int frameSize = convertEndians(frameHeader.data.size);

        if (frameSize == 0) {
            break;
        }

        if (!strcmp(tagName, toString(frameHeader.data.id, 4))) {
            searchFlag = 0;

            unsigned char *frameContents = malloc(frameSize * sizeof(char));

            fread(frameContents, sizeof(char), frameSize, file);

            printf("%s: %s\n", toString(frameHeader.data.id, 4), parseFrameContents(frameContents, frameSize));
            free(frameContents);
            break;
        }
        fseek(file, frameSize, SEEK_CUR); // Переносим указатель на следующий фрейм
    }
    if (searchFlag) {
        printf("Tag was not found.\n");
    }
    fclose(file);

}

void set(char *fileName, char *propName, char *propValue) {
    show(fileName);

    FILE *file;
    if ((file = fopen(fileName, "rb")) == NULL) {
        printf("File is not exist.");
        exit(-1);
    }

    fseek(file, 0, SEEK_SET);

    union fileHeader fileHeader;
    fread(fileHeader.buffer, sizeof(char), 10, file);

    unsigned int tagSize = fromMP3Size(convertEndians(fileHeader.data.size));

    if (fileHeader.data.version != 3) {
        printf("Unsupported version of ID3. Only ID3v2.3 is supported.");
        exit(-1);
    }

    int searchFlag = 1;
    unsigned int frameSize;
    unsigned int frameSizeNew;
    unsigned int frameChangePos;
    unsigned char *newTag;


    while (ftell(file) < tagSize + 10 && searchFlag) {
        union frameHeader frameHeader;
        fread(frameHeader.buffer, sizeof(char), 10, file);
        frameSize = convertEndians(frameHeader.data.size);
        if (frameSize == 0) {
            break;
        }
        if (!strcmp(propName, toString(frameHeader.data.id, 4))) {
            searchFlag = 0;
            newTag = stringToTag(propValue, &frameSizeNew);
            frameChangePos = ftell(file) - 10;
            break;
        }
        fseek(file, frameSize, SEEK_CUR);
    }

    if (searchFlag) {
        printf("Tag was not found.\n");
    } else {
        char *newFileName = calloc(sizeof(fileName) + 3, sizeof(char));
        strncpy(newFileName + 4, fileName, sizeof(fileName));
        newFileName[0] = 'n';
        newFileName[1] = 'e';
        newFileName[2] = 'w';
        newFileName[3] = '_';
        FILE *out = fopen(newFileName, "wb");
        fseek(file, 0, SEEK_SET);
        fseek(out, 0, SEEK_SET);

        fwrite(fileHeader.buffer, sizeof(char), 6, out);

        unsigned int value = convertEndians(toMP3Size(tagSize - frameSize + frameSizeNew)); // Считаем новый tag size
        fwrite(&value, sizeof(int), 1, out);
        copyFileP(file, out, 10, 10, frameChangePos);

        // Меняем длину фрейма
        fseek(out, ftell(out) - 6, SEEK_SET);
        value = convertEndians(frameSizeNew);
        fwrite(&value, sizeof(int), 1, out);
        fseek(out, ftell(out) + 2, SEEK_SET);

        for (int i = 0; i < frameSizeNew; i++) {
            fwrite(&newTag[i], sizeof(char), 1, out); // Записываем новый тег
        }
        copyFileE(file, out, 10 + frameChangePos + frameSize,
                  10 + frameChangePos + frameSizeNew); // Копируем остальные данные


        fclose(out);
        printf("\nafter modification\n\n");
        show(newFileName);

    }
    fclose(file);

}

void main(int argc, char *argv[]) {
    char *filepath = strpbrk(argv[1], "=") + 1;
    if (argc < 3) {
        printf("Please use commands like this:\n--filepath=song.mp3 --show\n--filepath=song.mp3 --get=TYER\n--filepath=song.mp3 --set=TYER value=2000\n");
        exit(-1);
    }
    if (!strcmp(argv[2], "--show")) {
        show(filepath);
    } else if (argv[2][2] == 'g' && argv[2][3] == 'e' && argv[2][4] == 't') {
        get(filepath, strpbrk(argv[2], "=") + 1);
    } else if (argv[2][2] == 's' && argv[2][3] == 'e' && argv[2][4] == 't') {
        set(filepath, strpbrk(argv[2], "=") + 1, strpbrk(argv[3], "=") + 1);
    } else {
        printf("Please use commands like this:\n--filepath=song.mp3 --show\n--filepath=song.mp3 --get=TYER\n--filepath=song.mp3 --set=TYER value=2000\n");
        exit(-1);
    }

}
//--filepath=song.mp3 --set=TIT2 value=NewMinecraft