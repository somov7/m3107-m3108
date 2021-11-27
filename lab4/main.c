#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>
#include "tag.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("wrong number of parameters: %d\n", argc);
        return 1;
    }

    char *filename, *parameter_name, *parameter_value;
    int is_set = false;

    filename = argv[1] + 11;
    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "--get=", 6) == 0) {
            parameter_name = argv[i] + 6;
        } else if (strncmp(argv[i], "--set=", 6) == 0) {
            is_set = true;
            parameter_name = argv[i] + 6;
        } else if (strncmp(argv[i], "--value=", 8) == 0) {
            parameter_value = argv[i] + 8;
        }
    }

    if (is_set)
        set_frame(filename, parameter_name, parameter_value);
    else
        get_frames(filename, parameter_name);

    return 0;
}
