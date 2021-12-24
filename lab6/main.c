#include <stdlib.h>
#include <stdio.h>

#include "argparse/argparse.h"
#include "archiver/archiver.h"

int main(int argc, char** argv) {
  Args args = argparse(argc, argv);

  if (args.create == 1) {
    create(args.sourceFilesCount, args.sourceFiles, args.filename);
  }

  if (args.extract == 1) {
    extract(args.filename);
  }

  if (args.list == 1) {
    list(args.filename);
  }

  return 0;
}
