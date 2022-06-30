#include "arc.h"
#include "list.h"
#include "argParse.h"

int main(int argc, char **argv) {
    char* arcFileName = calloc(MAX_FILENAME_LEN, sizeof(char));
    char** files = calloc(argc, sizeof(char*));
    int filesCnt = 0;

    if (argc < 4){
        printUsage("Invalid number of arguments");
        return -2;
    }

    for (int i = 1; i < argc; i++){
        if (!strcmp("--file", argv[i])){
            arcFileName = argv[i+1];
        }
    }

    if (strlen(arcFileName) == 0){
        printUsage("Archive name not found");
        return -3;
    }

    for (int i = 1; i < argc; i++){
        if (!strcmp("--create", argv[i])){
            for (int j = i + 1; j < argc; j++){
                files[filesCnt] = argv[j];
                filesCnt++;
            }
            inCompress(files, filesCnt, arcFileName);
            return 0;
        }
        else if (!strcmp("--extract", argv[i])){
            outCompress(arcFileName);
            return 0;
        }
        else if (!strcmp("--list", argv[i])){
            showFiles(arcFileName);
            return 0;
        }
    }

    printUsage("Unknown command");
}
