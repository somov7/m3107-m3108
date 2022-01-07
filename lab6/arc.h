#ifndef ARV_ARC_H
#define ARV_ARC_H
#include <stdio.h>

#pragma pack(push, 1)
typedef struct {
    char sig[3];
    size_t file_size;
    size_t files;
}Header;
#pragma pack(pop)

typedef struct {
    Header header;
    FILE *file;
}Arc;

Arc arc_create(char *arc_name);
Arc arc_open(const char *arc_name);
void arc_close(Arc *arc);
int arc_extract(Arc *arc);
char **get_list(Arc *arc);
long arc_add(Arc *arc, char* file_name);
#endif //ARV_ARC_H
