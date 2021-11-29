#include "argParse.h"
#include "bmp.h"

void printUsage(char* err){
    printf("Error: %s\n", err);
    printf("%s", USAGE_MESSAGE);
}

void openCorrect(FILE* file){
    if (file == NULL){
        printUsage("Can't open a file");
        exit(-1);
    }
}

void argParse(int argc, char** argv){
    if (argc < 3){                              // число аргументов точно не может быть < 3
        printUsage("Invalid number of arguments");
        exit(-2);
    }
    for (int i = 1; i < argc; i += 2){
        if (!strcmp("--input", argv[i])){
            INPUT_FILE_NAME = argv[i + 1];
        }
        else if (!strcmp("--output", argv[i])){
            DIRECTORY_NAME = argv[i + 1];
        }
        else if (!strcmp("--max_iter", argv[i])){
            MAX_ITER = atoi(argv[i + 1]);
        }
        else if (!strcmp("--dump_freq", argv[i])){
            DUMP_FREQ = atoi(argv[i + 1]);
        }

    }

    if (INPUT_FILE_NAME == NULL){
        printUsage("The input file name is not specified");
        exit(-1);
    }
    if (DIRECTORY_NAME == NULL){
        printUsage("The output directory is not specified");
        exit(-1);
    }

}