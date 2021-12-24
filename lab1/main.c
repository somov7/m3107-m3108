#include <stdio.h>
#include <stdlib.h>
#include "argparse/argparse.h"
#include "fileparse/fileparse.h"

int main (int argc, char** argv) {
  struct Args parsedArgs = argparse(argc, argv);

  int result = fileparse(parsedArgs.filePath, parsedArgs.argType);
  printf("%d\n", result);
  return 0;
}
