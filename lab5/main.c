#include <stdio.h>
#include <string.h>
#include "argparse/argparse.h"
#include "game/game.h"

int main(int argc, char *argv[]) {
    Params params = args(argc, argv);
    FILE *file = fopen(params.input, "rb"); // open file for read
    if (file == NULL) {
        fprintf(stderr, "Файл не найден\n");
        return 1;
    }
    start(file, params);
    return 0;
}
