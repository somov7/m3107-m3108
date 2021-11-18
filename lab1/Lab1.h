#include <stdio.h>

FILE* open_file(char* filename, int bin);
int count_lines(FILE* file);
int count_bytes(FILE* file);
int count_words(FILE* file);