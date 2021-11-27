#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>
#include "tag.h"

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "Russian");

    if (argc < 3) {
        printf("wrong number of parameters: %d\n", argc);
        return 1;
    }

    char *filename, *parameter_name, *parameter_value;
    int is_show = false, is_get = false, is_set = false;

    filename = argv[1] + 11;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--show") == 0) {
            is_show = true;
        } else if (strncmp(argv[i], "--get=", 6) == 0) {
            is_get = true;
            parameter_name = argv[i] + 6;
        } else if (strncmp(argv[i], "--set=", 6) == 0) {
            is_set = true;
            parameter_name = argv[i] + 6;
        } else if (strncmp(argv[i], "--value=", 8) == 0) {
            parameter_value = argv[i] + 8;
        }
    }

    if (is_show) {
        get_frames(filename);
    } else if (is_get) {
        get_frame(filename, parameter_name);
    } else if (is_set) {
        set_frame(filename, parameter_name, parameter_value);
    }

    return 0;
}
