#include "argParse.h"
#include "bmp.h"

void printUsage(char* err){
    printf("Error: %s", err);

}

void openCorrect(FILE* file){
    if (file == NULL){
        printUsage("Can't open a file");
        exit(-1);
    }
}

void argParse(int argv, char** argc){



    /*
    INPUT_FILE_NAME = calloc(MAX_FILENAME_LEN, sizeof(char));
    DIRECTORY_NAME = calloc(MAX_DIRECTORY_LEN, sizeof(char));
    */
}