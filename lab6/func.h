#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void create(FILE *arc_file, int num_of_files, char*files[]);
void extract(FILE *arc_file);
void list(FILE *arc_file);
/* .arc here:
 * first 4 bytes - size of file name (fn_size)
 * fn_size bytes - file name
 * 4 bytes - size of file (size)
 * f_size bytes - file content
 * */