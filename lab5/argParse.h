#ifndef LAB5_ARGPARSE_H
#define LAB5_ARGPARSE_H
#endif //LAB5_ARGPARSE_H
#include <stdio.h>
#include <stdlib.h>

static char USAGE_MESSAGE[] = "List of commands: \n"
                       "--input input_file.bmp \t name of monochrome input .bmp file\n"
                       "--output dir_name \t Name of directory for saving files\n"
                       "--max_iter N \t [OPTIONAL] The maximum number of generations that can emulate program. Optional parameter, by default infinity\n"
                       "--dump_freq N \t [OPTIONAL] The frequency with which the program should save generations as an image. by default, it is equal to 1\n";

int MAX_ITER; // (inf)
int DUMP_FREQ; // save frequency
char* INPUT_FILE_NAME;
char* DIRECTORY_NAME;

void printUsage(char* err);
void openCorrect(FILE* file);
void argParse(int argc, char** argv);