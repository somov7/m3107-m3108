#include "func.h"

int main(int argc, char *argv[]) {
    char *option;
    FILE *file_arc;

    if (argc < 4) {
        printf("Not enough arguments\n");
        return 1;
    }

    option = argv[3];
    if (!strcmp(option, "--create")) {
        if ((file_arc = (fopen(argv[2], "wb"))) != NULL)
            create(file_arc, argc - 4, &argv[4]);
        else {
            printf("Cannot create archive\n");
            return 1;
        }
    }
    else if (!strcmp(option, "--extract")) {
        if ((file_arc = (fopen(argv[2], "rb"))) != NULL)
            extract(file_arc);
        else {
            printf("Cannot extract files\n");
            return 1;
        }
    }
    else if (!strcmp(option, "--list")) {
        if ((file_arc = (fopen(argv[2], "rb"))) != NULL)
            list(file_arc);
        else {
            printf("Cannot extract files\n");
            return 1;
        }
    }
    else {
        printf("Wrong format\n");
        return 1;
    }
    return 0;
}