#include "arc.h"
#include "list.h"
#include "argParse.h"

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

uint64_t getFileSize(char *fileName){
    uint64_t res;
    FILE *f = fopen(fileName, "rb");
    fseek(f, 0, SEEK_END);
    res = ftell(f);
    fclose(f);
    return res;
}


int digits(uint64_t x){
    int res = 1;
    while (x > 10){
        res++;
        x /= 10;
    }
    return res;
}

uint32_t getHeaderSize(char **str, int numberFiles) {
    uint32_t res = 2;
    for (int i = 1; i < numberFiles; i++) {
        res += (int) strlen(str[i]) + digits(getFileSize(str[i])) + 4; // четыре "|" + длина числа отвечающего за размер
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
        int d = digits(fileSize);
        char *fileSizeStr = calloc(d, sizeof(char));
        sprintf(fileSizeStr, "%lld", fileSize);
        fwrite(fileSizeStr, 1, d, info);
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
        remove(str[i]);
    }

    fclose(arc);
}

node *parseInfo(int *tokCnt, char* info){
    char* tok = strtok(info, "||");
    node *tokens = NULL;
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
    openCorrect(arc);
    int infoSize;
    fread(&infoSize, 4, 1, arc);

    char *infoBlock = calloc(infoSize + 1, sizeof(char));
    fread(infoBlock, 1, infoSize, arc);

    infoBlock[infoSize] = '\0';

    int tokenCnt = 0;
    node *tokens = parseInfo(&tokenCnt, infoBlock);
    free(infoBlock);
    int filesCnt = tokenCnt / 2;

    char byte;

    for (int i = 0; i < filesCnt; i++){

        char* size = popBack(&tokens);
        char* name = popBack(&tokens);
        FILE *curFile = fopen(name, "wb");
        openCorrect(curFile);

        uint64_t intSize = atoll(size);

        for (uint64_t j = 0; j < intSize; j++){
            if (fread(&byte, 1, 1, arc) == 1){
                fwrite(&byte, 1, 1, curFile);
            }
        }
        fclose(curFile);
        free(size);
        free(name);
    }

    fclose(arc);
    remove(arcName);
}


void showFiles(char* arcName){
    FILE *arc = fopen(arcName, "rb");
    openCorrect(arc);
    int infoSize;
    fread(&infoSize, 4, 1, arc);

    char *infoBlock = calloc(infoSize + 1, sizeof(char));
    fread(infoBlock, 1, infoSize, arc);

    infoBlock[infoSize] = '\0';

    int tokenCnt = 0;
    node *tokens = parseInfo(&tokenCnt, infoBlock);
    free(infoBlock);

    int filesCnt = tokenCnt / 2;

    printf("There are %d files archived: \n", filesCnt);

    for (int i = 0; i < filesCnt; i++){

        char* size = popBack(&tokens);
        char* name = popBack(&tokens);

        printf("-- %s\n", name);

        free(size);
        free(name);
    }

}