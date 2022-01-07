#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arc.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        puts("too few args given");
        exit(EXIT_FAILURE);
    }
    Arc arc;
    char *arc_name = NULL;
    int i = 1;
    while (i < argc) {
        if (argv[i][2] == 'f') {
            arc_name = argv[++i];
        } else if (argv[i][2] == 'c') {
            arc = arc_create(arc_name);
            i++;
            while (i < argc) {
                if (arc_add(&arc, argv[i++]) == -1) {
                    arc_close(&arc);
                    exit(EXIT_FAILURE);
                }
            }
            arc_close(&arc);
        } else if (argv[i][2] == 'e') {
            Arc arc = arc_open(arc_name);
            arc_extract(&arc);
            arc_close(&arc);
        } else if (argv[i][2] == 'l') {
            arc = arc_open(arc_name);
            char **names = get_list(&arc);
            for (int i = 0; i < arc.header.files; i++) {
                printf("%d: %s\n", i + 1, names[i]);
            }
            arc_close(&arc);
        }
        i++;
    }
}
