#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../tools/tools.h"
#include "archiver.h"

#define DEFAULT_SOURCE_FILE_NAME_LENGTH 64

//  file format:
// 1. 4 bytes - size of file(archive)
// 2. 4 bytes - count of files in archive
// 3. frame with:
// 3.1. 4 bytes - size of file
// 3.2 filename - till \0 char
// 3.3 file content

void create(int sourceFilesCount, char** sourceFiles, char* filename) {
  FILE* archiveFile = fopen(filename, "wb+");

  //  begins with filesize field and amount of files field
  unsigned int archiveFileSize = 8;

  //  allocate space for filesize
  unsigned char space[4] = {0, 0, 0, 0};
  fwrite(&space, sizeof(unsigned char), 4, archiveFile);

  //  write amount of source files
  fwrite(&sourceFilesCount, sizeof(unsigned char), 4, archiveFile);

  for (int i = 0; i < sourceFilesCount; ++i) {
    // open file
    char* sourceFileName = sourceFiles[i];
    FILE* sourceFile = fopen(sourceFileName, "rb+");
    if (sourceFile == NULL) {
      fprintf(stderr, "File %s was not found", sourceFileName);
      exit(EXIT_FAILURE);
    }

    //  get source file size
    fseek(sourceFile, 0, SEEK_END);
    int sourceFileSize = (int)ftell(sourceFile);
    fseek(sourceFile, 0, SEEK_SET);
    char sourceFileSizeBin[4];
    memcpy(sourceFileSizeBin, &sourceFileSize, 4);

    // write source file size
    fwrite(&sourceFileSizeBin, sizeof(char), 4, archiveFile);

    //  write source file name
    char* clearSourceFileName = getClearFileName(sourceFileName);
    unsigned int clearSourceFileNameSize = snprintf(NULL, 0, clearSourceFileName);
    for (int j = 0; j < clearSourceFileNameSize; ++j) {
      fwrite(&clearSourceFileName[j], sizeof(char), 1, archiveFile);
    }
    fwrite("\0", sizeof(char), 1, archiveFile);

    // write source file data
    char c;
    for (int j = 0; j < sourceFileSize; ++j) {
      c = (char)getc(sourceFile);
      fwrite(&c, sizeof(char), 1, archiveFile);
    }

    archiveFileSize += sourceFileSize;
    archiveFileSize += 4; // for source file size
    archiveFileSize += clearSourceFileNameSize + 1;

    free(sourceFileName);
  }


  //  write archive file size
  fseek(archiveFile, 0, SEEK_SET);
  char archiveFileSizeBin[4];
  memcpy(archiveFileSizeBin, &archiveFileSize, 4);
  fwrite(&archiveFileSizeBin, sizeof(char), 4, archiveFile);

  fclose(archiveFile);
}

void extract(char* filename) {
  FILE* archiveFile = fopen(filename, "rb+");
  if (archiveFile == NULL) {
    fprintf(stderr, "File %s does not exist", filename);
    exit(EXIT_FAILURE);
  }

  int archiveFileSize;
  fread(&archiveFileSize, sizeof(char), 4, archiveFile);
  
  int archiveFilesCount;
  fread(&archiveFilesCount, sizeof(char), 4, archiveFile);

  for (int i = 0; i < archiveFilesCount; ++i) {
    //  get source file size
    int sourceFileSize;
    fread(&sourceFileSize, sizeof(char), 4, archiveFile);

    //  read source file name
    char* sourceFileName = readFileName(archiveFile);

    // open the file
    FILE* sourceFile = fopen(sourceFileName, "wb+");

    //  write the source file
    char c;
    for (int j = 0; j < sourceFileSize; ++j) {
      c = (char)getc(archiveFile);
      fwrite(&c, sizeof(char), 1, sourceFile);
    }

    free(sourceFileName);
    fclose(sourceFile);
  }

  fclose(archiveFile);
}

void list(char* filename) {
  FILE* archiveFile = fopen(filename, "rb+");
  if (archiveFile == NULL) {
    fprintf(stderr, "File %s does not exist", filename);
    exit(EXIT_FAILURE);
  }

  int archiveFilesCount;

  //  read archive file size first
  char space[4];
  fread(&space, sizeof(char), 4, archiveFile);

  //  read source files amount now
  fread(&archiveFilesCount, sizeof(char), 4, archiveFile);

  printf("Total amount of files: %d\n", archiveFilesCount);

  for (int i = 0; i < archiveFilesCount; ++i) {
    //  read first source file size
    int sourceFileSize;
    fread(&sourceFileSize, sizeof(char), 4, archiveFile);

    char* sourceFileName = readFileName(archiveFile);

    //  skip file data
    int pos = (int)ftell(archiveFile);
    fseek(archiveFile, sourceFileSize + pos, SEEK_SET);

    printf("%d. %s\n", i + 1, sourceFileName);

    free(sourceFileName);
  }

  fclose(archiveFile);
}
