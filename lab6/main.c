#include <stdio.h>
#include <string.h>
#include "archiver.h"

int main(int argc, char **argv) {
   if (argc < 2) {
       puts("too few args given");
       exit(EXIT_FAILURE);
   }
   Archive *arc = NULL;
   char *arc_name = NULL;
   int i = 1;
    while (i < argc) {
       if (argv[i][2] == 'f') {
            arc_name = argv[++i];
       } else if (argv[i][2] == 'c') {
           arc = arc_create(arc_name);
           i++;
           while (i < argc) {
               if (arc_add(arc, argv[i++]) == -1) {
                   puts(get_errorm());
                   arc_close(arc);
                   exit(EXIT_FAILURE);
               }
           }
           arc_close(arc);
       } else if (argv[i][2] == 'e') {
           arc_extract(arc_name);
       } else if (argv[i][2] == 'l') {
            arc = arc_open(arc_name);
            char **names = set_getall(arc->file_names);
            for (int i = 0; i < set_get_size(arc->file_names); i++) {
                printf("%d: %s\n", i + 1, names[i]);
            }
           arc_close(arc);
       } else if (argv[i][2] == 'a') {
           arc = arc_open(arc_name);
           i++;
           while (i < argc) {
               if (arc_add(arc, argv[i++]) == -1) {
                   puts(get_errorm());
                   arc_close(arc);
                   exit(EXIT_FAILURE);
               }
           }
           arc_close(arc);
       }
       i++;
   }
}
