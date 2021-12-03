#include "func.h"

int main(int argc, char *argv[]) {
    char *file_arc, *option;
    if (argc < 4) {
        printf("Not enough arguments\n");
        return 1;
    }
    file_arc = argv[2];
    option = argv[3];
    if (!strcmp(option, "--create")) {
        create(file_arc, argc - 4, &argv[4]);
    }
    else if (!strcmp(option, "--extract")) {
        extract(file_arc);
    }
    else if (!strcmp(option, "--list")) {
        list(file_arc);
    }
    else {
        printf("Wrong format\n");
        return 1;
    }
    return 0;
}