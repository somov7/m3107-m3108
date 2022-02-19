//
// Created by Rustam Tishkov on 09.01.2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "argparse.h"


Params args(int argc, char *argv[]) {
    Params params = {"", "", -1, 1};

    if (argc < 5 || argc > 9 || argc % 2 == 0) {
        fprintf(stderr, "Неверное количество аргументов");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; i += 2) {
        if (!strcmp(argv[i], "--input")) {
            params.input = argv[i + 1];
        } else if (!strcmp(argv[i], "--output")) {
            params.output = argv[i + 1];
        } else if (!strcmp(argv[i], "--dump_freq")) {
            params.dumpFreq = atoi(argv[i + 1]);
        } else if (!strcmp(argv[i], "--max_iter")) {
            params.maxIter = atoi(argv[i + 1]);
        } else {
            fprintf(stderr, "Неверный аргумент");
            exit(EXIT_FAILURE);
        }
    }

    if (strlen(params.input) == 0 || strlen(params.output) == 0) {
        fprintf(stderr, "Неверное параметры");
        exit(EXIT_FAILURE);
    }

    return params;
}
