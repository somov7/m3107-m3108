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

