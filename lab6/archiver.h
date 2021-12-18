//
// Created by Антон Чемодуров on 14.11.2021.
//

#ifndef ARCHIVER_ARCHIVER_H
#define ARCHIVER_ARCHIVER_H

#include <stdio.h>
#include <stdlib.h>
#include "Map/Set.h"

#define MAX_NAME_SIZE 256


typedef struct Archive {
    FILE* arc;
    Set *file_names;
}Archive;

long arc_add(Archive *arc, char* file_name);
Archive *arc_create(char *arc_name);
void arc_close(Archive *arc);
Archive *arc_open(const char *arc_name);
int arc_extract(char *arc_name);
const char *get_errorm();
#endif //ARCHIVER_ARCHIVER_H
