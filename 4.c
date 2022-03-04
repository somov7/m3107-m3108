#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

char COMM [5]="COMM";
char TXXX [5]="TXXX";
struct Frame {
    char frame_ID[4];
    char size[4];
    char flags[2];
};

struct MainHead {
    char ID3[3];
    char versionBytes[2];
    char flagByte;
    char size[4];
};

void setFrameSize(char* frameSize, int valSize) {
    for (int i = 0; i < 4; i++) {
        frameSize[i] = valSize / pow(2, 7 * (3 - i));
        valSize -= frameSize[i] * pow(2, 7 * (3 - i));
    }
}
bool equality(char* str1, char* str2) {
    for (int i = 0; i < sizeof(str1); i++) {
        if (str1[i] != str2[i]) {
            return false;
        }
    }
    return true;
}
int getFrameSize(char* byteArr) {
    int size = 0;
    unsigned char ch;
    for (int i = 0; i < 4; i++) {
        ch = byteArr[i];
        size += ch * pow(2, 7 * (3 - i));
    }
    return size;
}

void showFrames(FILE* f) {
    fseek(f, 0, SEEK_SET);
    struct MainHead header;
    fread(&header, sizeof(header), 1, f);
    long long tagSize = getFrameSize(header.size);
    struct Frame frame;
    long long counter = 0;
    while (counter <= tagSize) {
        fread(&frame, sizeof(frame), 1, f);
        long long frameSize = getFrameSize(frame.size);
        counter += 10 + frameSize;
        if (frame.frame_ID[0] == 'T') {
            printf("%s ", frame.frame_ID);
            while(frameSize--) {
                char ch = fgetc(f);
                if (ch >= 32 && ch <= 126)
                    printf("%c", ch);
            }
            printf("\n");
        }
        else if (equality(frame.frame_ID, COMM) == true) {
            printf("%s ", frame.frame_ID);
            fseek(f, 4, SEEK_CUR);
            frameSize -= 4;
            while(frameSize--) {
                char ch = fgetc(f);
                if (ch >= 32 && ch <= 126)
                    printf("%c", ch);
            }
            printf("\n");
        }
    }
}

void showFrame(FILE* f, char* frame_ID) {
    fseek(f, 0, SEEK_SET);
    struct MainHead header;
    fread(&header, sizeof(header), 1, f);
    int fileSize = getFrameSize(header.size);
    struct Frame frame;
    int counter = 0;
    while (counter <= fileSize) {
        fread(&frame, sizeof(frame), 1, f);
        int frameSize = getFrameSize(frame.size);
        counter += 10 + frameSize;
        if (equality(frame.frame_ID, frame_ID) == true) {
            printf("%s ", frame.frame_ID);
            while(frameSize--) {
                char ch = fgetc(f);
                if (ch >= 32 && ch <= 126)
                    printf("%c", ch);
            }
        }
        else {
            fseek(f, getFrameSize(frame.size), SEEK_CUR);
        }
    }
}

void setVal(FILE* fin, FILE* fout, char* someFrame, char* val) {
    fseek(fin, 0, SEEK_SET);
    struct MainHead header;
    fread(&header, sizeof(header), 1, fin);
    fwrite(&header, sizeof(header), 1, fout);
    int valSize = strlen(val);
    int fileSize = getFrameSize(header.size);
    struct Frame frame;
    int counter = 0;
    int frameSize = 0;
    while (counter <= fileSize) {
        fread(&frame, sizeof(frame), 1, fin);
        frameSize = getFrameSize(frame.size);
        counter += 10 + frameSize;
        if (equality(frame.frame_ID, someFrame) == false && frameSize >= 0 && frameSize <= 100000) {
            fwrite(&frame, 1, sizeof(frame), fout);
            char info[frameSize];
            fread(&info, frameSize, 1, fin);
            fwrite(info, frameSize, 1, fout);
        }
        else if (equality(frame.frame_ID, someFrame) == true && frameSize >= 0){
            int oldFrameSize = frameSize;
            setFrameSize(header.size, getFrameSize(header.size) - frameSize + valSize);
            int curPos = ftell(fout);
            fseek(fout, 6, SEEK_SET);
            fwrite(header.size, sizeof(header.size), 1, fout);
            fseek(fout, curPos, SEEK_SET);
            if (equality(frame.frame_ID, COMM) == true)
                setFrameSize(frame.size, valSize + 4);
            else if (equality(frame.frame_ID, TXXX) == true)
                setFrameSize(frame.size, valSize + 2);
            else
                setFrameSize(frame.size, valSize);
            int newFrameSize = getFrameSize(frame.size);
            fwrite(&frame, sizeof(frame), 1, fout);
            if (equality(frame.frame_ID, COMM) == true) {
                for (int i = 0; i < 4; i++) {
                    fputc(fgetc(fin), fout);
                }
                fseek(fin, oldFrameSize - 4, SEEK_CUR);
            }
            else if (equality(frame.frame_ID, TXXX) == true) {
                for (int i = 0; i < 2; i++) {
                    fputc(fgetc(fin), fout);
                }
                fseek(fin, oldFrameSize - 2, SEEK_CUR);
            }
            else
                fseek(fin, oldFrameSize, SEEK_CUR);
            fwrite(val, valSize, 1, fout);
        }
    }
    int ch;
    while ((ch = fgetc(fin)) != EOF) {
        fputc(ch, fout);
    }
    fseek(fout, 0, SEEK_SET);
    fseek(fin , 0, SEEK_SET);
    while ((ch = getc(fout)) != EOF) {
        fputc(ch, fin);
    }
}



int main(int argc, char* argv[]) {
    char com[3][20];
    char arg[3][50];
    int k = 0, m = 0, j = 0;

    for (int i = 1; i < argc; i++) {
        k = 0;
        for (j = 0; argv[i][j] != '=' && argv[i][j] != '\0'; j++) {
            com[m][k++] = argv[i][j];
        }
        com[m][k] = '\0';

        j++;
        k = 0;
        while (argv[i][j] != '\0') {
            arg[m][k++] = argv[i][j++];
        }
        arg[m][k] = '\0';
        m++;
    }

    FILE* mp3;
    mp3 = fopen(arg[0], "rb");
    FILE* output;
    output = fopen("out.mp3", "rb");
    for (int i = 1; i < argc - 1; i++) {
        if (strcmp(com[i], "--show") == 0) {
            showFrames(mp3);
    }
        else if (strcmp(com[i], "--get") == 0) {
            showFrame(mp3, arg[i]);
        }
        else if (strcmp(com[i], "--set") == 0) {
            setVal(mp3, output, arg[i], arg[i + 1]);
        }
    }

    return 0;
}