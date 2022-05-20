/* Orlov Aleksandr, 12-5, M3107, 15.12.2021 */

#include <stdio.h>
#include <string.h>
#include "archive.h"

int main(int argc, char *argv[]) {
    FILE *f;
    unsigned char byte;
    char filename[MAX_STRING_SIZE];
    unsigned int sz;
    int files = 0, operation = 0;

    if (argc == 2 && !strcmp(argv[1], "--help")) {
        printf("This app is for creating and unpacking archives of files\n");
        printf("App supports 4 commands:\n--file - for the name of the archive\n");
        printf("--create - for creating archive and for names of source files\n");
        printf("--extract - for unpacking the archive\n");
        printf("--list - for displaying names of files in the archive\n");
        printf("Example of the command: Lab6.exe --file data.arc --create D:\\\\a.txt C:/b.bin c.bmp\n");
        printf("PLEASE, BUY OUR LICENSE\n");
        return 0;
    }

    if (argc < 4) {
        printf("Error: incorrect number of arguments\n");
        return 1;
    }

    for (int i = 1; i < argc; i += 2) {
        if (!strcmp("--file", argv[i]))
            strcpy(filename, argv[i + 1]);
        else if (!strcmp("--create", argv[i])) {
            if (argc == 4) {
                printf("Error: incorrect flenames\n");
                return 1;
            }
            files = argc - 4, operation = 1;
            break;
        }
        else if (!strcmp("--extract", argv[i])) {
            operation = 2;
            break;
        }
        else if (!strcmp("--list", argv[i])) {
            operation = 3;
            break;
        }
    }

    if (operation == 0) {
        printf("Error: incorrect command\n");
        return 1;
    }

    switch (operation) {
        case 1:
            create_archive(argv, files, filename);
            break;
        case 2:
            unpack_archive(filename);
            break;
        case 3:
            file_list(filename);
            break;
    }

    return 0;
}
