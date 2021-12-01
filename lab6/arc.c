#include "arc.h"
#include "list.h"

void printUsage(char *str) {
    printf("%s", str);
}


void openCorrect(FILE *file) {
    if (file == NULL) {
        printUsage("can't open a file:");
        exit(-1);
    }
}


uint64_t getLen(const char str[8]){
    uint64_t res = 0;
    uint64_t x = 1;
    for (int i = 0; i < 8; i++){
        res += str[i]*x;
        x *= 256;
    }
    return res;
}


char *getFileName(char *filePath) {
    if (strrchr(filePath, '\\') == NULL) {
        char *res = calloc(strlen(filePath), sizeof(char));
        strcpy(res, filePath);
        return res;
    } else {
        char *res = calloc(strlen(filePath), sizeof(char));
        strcpy(res, strrchr(filePath, '\\'));
        return res + 1;
    }
}


uint32_t getHeaderSize(char **str, int numberFiles) {
    uint32_t res = 2;
    for (int i = 1; i < numberFiles; i++) {
        res += (int) strlen(str[i]) + 4 + 8;
    }
    return res;
}


void getInfo(char **str, int numberFiles) {

    remove(TEMP_FILE);
    FILE *info = fopen(TEMP_FILE, "wb");
    FILE *curFile;
    openCorrect(info);

    uint32_t headerSize = getHeaderSize(str, numberFiles);
    fwrite(&headerSize, 4, 1, info);
    fwrite("||", 1, 2, info);


    for (int i = 1; i < numberFiles; i++) {

        curFile = fopen(str[i], "rb");
        openCorrect(curFile);

        fseek(curFile, 0, SEEK_END);    // записываем размер архивируемого файла
        uint64_t fileSize = ftell(curFile);
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


void inCompress(char **str, int numberFiles, char *arcName) {

    FILE *arc = fopen(arcName, "wb");
    FILE *curFile;
    openCorrect(arc);

    getInfo(str, numberFiles);
    FILE *info = fopen(TEMP_FILE, "rb");
    openCorrect(info);

    char byte;

    while (!feof(info)) {
        if (fread(&byte, 1, 1, info) == 1) {
            fwrite(&byte, 1, 1, arc);
        }
    }

    fclose(info);
    remove(TEMP_FILE);

    for (int i = 1; i < numberFiles; i++) {
        curFile = fopen(str[i], "rb");
        openCorrect(curFile);
        while (!feof(curFile)) {
            if (fread(&byte, 1, 1, curFile) == 1) {
                fwrite(&byte, 1, 1, arc);
            }
        }
        fclose(curFile);
    }

    fclose(arc);
}

node *parseInfo(int *tokCnt, char* info){

    char* tok = strtok(info, "||");
    node *tokens;

    while (tok){
        if (strlen(tok) == 0){
            break;
        }
        push(&tokens, tok);
        tok = strtok(NULL, "||");
        (*tokCnt)++;
    }

    return tokens;
}

void outCompress(char* arcName){
    FILE *arc = fopen(arcName, "rb");
    int infoSize;
    fread(&infoSize, 4, 1, arc);

    char *infoBlock = calloc(infoSize + 1, sizeof(char));
    fread(infoBlock, 1, infoSize, arc);
    infoBlock[infoSize] = '\0';

    int tokenCnt = 0;
    node *tokens = parseInfo(&tokenCnt, infoBlock);
    free(infoBlock);
    int filesCnt = tokenCnt / 2;

    for (int i = 0; i < filesCnt; i++){

    }

    fclose(arc);
    remove(arcName);
}