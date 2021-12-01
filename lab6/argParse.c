#include "argParse.h"
#include "arc.h"

const char USAGE_MESSAGE[] = "\t\t==============CONSOLE ARCHIVER==============\n"
                             "Supported commands:\n"
                             "--file FILE            \t||\tThe name of the file archive that the archiver will work with\n"
                             "--create               \t||\tCommand to create a file archive\n"
                             "--extract              \t||\tCommand to extract files from a file archive\n"
                             "--list                 \t||\tCommand to provide a list of files stored in the archive\n"
                             "FILE1 FILE2 .... FILEN \t||\tFree arguments for passing a list of files to be packed\n\n"
                             "Usage examples:\n"
                             "arc.exe --file data.arc --create a.txt b.bin c.bmp\n"
                             "arc.exe --file data.arc --extract\n"
                             "arc.exe --file data.arc --list\n";

void printUsage(char *str) {
    printf("An error has occurred: %s\n\n", str);
    printf(USAGE_MESSAGE);
}


void openCorrect(FILE *file) {
    if (file == NULL) {
        printUsage("can't open a file");
        exit(-1);
    }
}

void argParse(int argc, char** argv){
    char* arcFileName = calloc(MAX_FILENAME_LEN, sizeof(char));
    char** files = calloc(argc, sizeof(char*));
    int filesCnt = 0;

    if (argc < 4){
        printUsage("Invalid number of arguments");
        exit(-2);
    }

    for (int i = 1; i < argc; i++){
        if (!strcmp("--file", argv[i])){
            arcFileName = argv[i+1];
        }
    }

    if (strlen(arcFileName) == 0){
        printUsage("Archive name not found");
        exit(-3);
    }

    for (int i = 1; i < argc; i++){
        if (!strcmp("--create", argv[i])){
            for (int j = i + 1; j < argc; j++, filesCnt++){
                files[filesCnt] = argv[j];
            }
            inCompress(files, filesCnt, arcFileName);
        }
        else if (!strcmp("--extract", argv[i])){
            outCompress(arcFileName);
        }
        else if (!strcmp("--list", argv[i])){
            showFiles(arcFileName);
        }
    }

}

