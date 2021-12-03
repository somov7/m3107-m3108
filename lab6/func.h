#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void create(char *arc_name, int num_of_files, char*files[]);
void extract(char *arc_name);
void list(char *arc_name);
/* .arc here:
 * first 4 bytes - size of file name (fn_size)
 * fn_size bytes - file name
 * 4 bytes - size of file (size)
 * f_size bytes - file content
 * */