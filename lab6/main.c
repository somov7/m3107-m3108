#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define TEMP_FILE "info.txt"

void printUsage(char* str){
    printf("%s", str);
}

void openCorrect(FILE* file){
    if (file == NULL){
        printUsage("can't open a file:");
        exit(-1);
    }
}

char* getFileName(char* filePath){
    if (strrchr(filePath, '\\') == NULL){
        char *res = calloc(strlen(filePath), sizeof(char));
        strcpy(res, filePath);
        return res;
    }
    else{
        char *res = calloc(strlen(filePath), sizeof(char));
        strcpy(res, strrchr(filePath, '\\'));
        return res + 1;
    }
}

uint32_t getHeaderSize(char **str, int numberFiles){
    uint32_t res = 2;
    for (int i = 1; i < numberFiles; i++){
        res += (int)strlen(str[i]) + 8 + 4;
    }
    return res;
}

void getInfo(char** str, int numberFiles){

    remove(TEMP_FILE);
    FILE* info = fopen(TEMP_FILE, "wb");
    FILE* curFile;
    openCorrect(info);

    uint32_t headerSize = getHeaderSize(str, numberFiles);
    fwrite(&headerSize, 4, 1, info);
    fwrite("||", 1, 2, info);

    for (int i = 1; i < numberFiles; i++){

        curFile = fopen(str[i], "rb");
        openCorrect(curFile);

        fseek(curFile, 0, SEEK_END);    // записываем размер архивируемого файла
        uint64_t fileSize = ftell(curFile);
        printf("%lld\n", fileSize);
        fwrite(&fileSize, 8, 1, info);
        fwrite("||", 1, 2, info);

        fclose(curFile);

        char *curFileName = getFileName(str[i]); // записываем путь до архивируемого файла
        fputs(curFileName, info);
        fwrite("||", 1, 2, info);

        free(curFileName);

    }

    fclose(info);
}

void archive(char** str, int numberFiles, char* arcName){

    FILE *arc = fopen(arcName, "wb");
    FILE *curFile;
    openCorrect(arc);

    getInfo(str, numberFiles);
    FILE *info = fopen(TEMP_FILE,"rb");
    openCorrect(info);

    char byte;

    while (!feof(info)){
        if (fread(&byte, 1, 1, info) == 1){
            fwrite(&byte, 1, 1, arc);
        }
    }

    fclose(info);
    remove(TEMP_FILE);

    for (int i = 1; i < numberFiles; i++){
        curFile = fopen(str[i], "rb");
        openCorrect(curFile);
        while (!feof(curFile)){
            if (fread(&byte, 1, 1, curFile) == 1){
                fwrite(&byte, 1, 1, arc);
            }
        }
        fclose(curFile);
    }

    fclose(arc);
}

int main(int argc, char **argv) {
    archive(argv, argc, "test.arc");
    char *buf = malloc(2);
    int a;
    uint64_t b;
    FILE *temp = fopen("test.arc", "rb");
    fread(&a, 4, 1, temp);
    printf("%d\n", a);
    fread(buf, 1, 2, temp);

    fread(&b, 8, 1, temp);
    printf("%lld", b);
    return 0;
}
