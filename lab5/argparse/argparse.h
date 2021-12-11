#ifndef PROGA_LABA5_ARGPARSE_H
#define PROGA_LABA5_ARGPARSE_H


typedef struct Args {
  char* input;
  char* output;
  int maxIter;
  int dumpFreq;
} Args;

Args argparse(int argc, char** argv);

#endif
