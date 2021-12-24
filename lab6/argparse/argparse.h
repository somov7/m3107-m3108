#ifndef PROGA_LABA6_ARGPARSE_H
#define PROGA_LABA6_ARGPARSE_H

typedef struct Args {
  char* filename;
  int create;
  int extract;
  int list;

  int sourceFilesCount;
  char** sourceFiles;
} Args;

Args argparse(int argc, char** argv);

#endif //PROGA_LABA6_ARGPARSE_H
