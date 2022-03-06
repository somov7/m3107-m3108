#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compress.h"
#include "decompress.h"

// app commands
const short CREATE = 1;
const short EXTRACT = 2;
const short LIST = 3;

const short FILES_BUFFER_SIZE = 2; // buffering input file names reading
const int BUFFER_SIZE = 10000; // buffer size while copy data from file to archive file
const long HEADER_START = 0x98765432; // archive start header (to check)

char *TEMP_FILE = "huffmanTemp_924234.arc";

typedef struct params
{
  short command;
  char *arcFile; // arc file
  char **files; // files
} params;

params appparams = {0, ""};

typedef struct info
{
  int filesSize;
} info;

info appinfo = {0};

void checkParams();

/**
 * Get params structure from command line.
 */
void readParams(int argc, char *argv[]) {
  int argIndex, filesCounter = 0;
  for (argIndex = 1; argIndex < argc; argIndex++) {
    if (strcmp(argv[argIndex], "--file") == 0) {
      if (argIndex + 1 < argc) {
        appparams.arcFile = argv[argIndex + 1];
        argIndex++;
      }
    } else if (strcmp(argv[argIndex], "--create") == 0) {
      appparams.command = CREATE;
    } else if (strcmp(argv[argIndex], "--list") == 0) {
      appparams.command = LIST;
    } else if (strcmp(argv[argIndex], "--extract") == 0) {
      appparams.command = EXTRACT;
    } else if (appparams.command == CREATE && strcmp(appparams.arcFile, "") != 0) {
      // read files only after all params are read and command = --create
      if (filesCounter == appinfo.filesSize) {
        appinfo.filesSize += FILES_BUFFER_SIZE;
        if (filesCounter == 0) {
          appparams.files = calloc(sizeof(char*), appinfo.filesSize);
        } else {
          // create file names array of bigger size
          char **newFiles = calloc(sizeof(char*), appinfo.filesSize);
          int index;
          for (index = 0; index < appinfo.filesSize - FILES_BUFFER_SIZE; index++) {
            newFiles[index] = appparams.files[index];
          }
          free(appparams.files);
          appparams.files = newFiles;
        }
      }
      appparams.files[filesCounter] = argv[argIndex];
      filesCounter++;
    } else {
      appExit(1);
    }
  }
  if (appparams.command == CREATE && filesCounter > 0) {
    appinfo.filesSize = filesCounter;
  }
  checkParams();
}

/**
 * Check that params are ok.
 */
void checkParams() {
  if (strcmp(appparams.arcFile, "") == 0 || appparams.command == 0 || 
      appparams.command == CREATE && appinfo.filesSize == 0) {
    printf("ERROR: wrong params\nUsage:\n--file - archive file name (mandatory)\n--create - create file\n--extract - extract files from archive\n--list - list files in archive\nFILE1 FILE2 .... FILEN - files (if --create)");
    appExit(1);
  }
}

/**
 * Handle error if archive is bad.
 */
void archiveError(FILE *file) {
  printf("Archive file %s is not correct\n", appparams.arcFile);
  fclose(file);
  appExit(1);
}

/**
 * Read start of archive and check if archive is not bad.
 */
FILE *readArchiveFile() {
  FILE *file = fopen(appparams.arcFile, "rb"); // open file for read
  if (file == NULL) {
    printf("Cannot open archive file: %s\n", appparams.arcFile);
    appExit(1);
  }
  long headerStart;
  fread(&headerStart, 1, sizeof(long), file);
  if (headerStart != HEADER_START) {
    archiveError(file);
  }
  int read;
  read = fread(&appinfo.filesSize, 1, sizeof(int), file);
  int fileSizeRpt;
  read = fread(&fileSizeRpt, 1, sizeof(int), file);
  if (read == 0 || appinfo.filesSize == 0 || appinfo.filesSize != fileSizeRpt) {
    archiveError(file);
  }
  // here we on the start of file headers
  return file;
}

/**
 * File header:
 * - name size (int)
 * - name (char*)
 */ 
void readFileHeader(int fileIndex, FILE *file) {
  int fileNameLen;
  fread(&fileNameLen, 1, sizeof(int), file);
  appparams.files[fileIndex] = calloc(sizeof(char), fileNameLen);
  fread(appparams.files[fileIndex], fileNameLen, sizeof(char), file);
}

/**
 * Get file from archive and save to disk.
 * 
 * File:
 * - file length (long)
 * - file (char*)
 */ 
void extractFile(int fileIndex, FILE *arcFile) {
  // read file length
  long fileLen;
  fread(&fileLen, 1, sizeof(long), arcFile);
  FILE *fileItem = fopen(TEMP_FILE, "wb");
  char *buffer = calloc(BUFFER_SIZE, sizeof(char));
  for (;;) {
    int readSize;
    if (fileLen > BUFFER_SIZE) {
      readSize = BUFFER_SIZE;
    } else {
      readSize = fileLen;
    }
    int read = fread(buffer, sizeof(char), readSize, arcFile);
    if (read <= 0) break;
    fwrite(buffer, sizeof(char), read, fileItem);
    if (fileLen <= BUFFER_SIZE) {
      break; // all was read
    }
    fileLen -= BUFFER_SIZE; // left file size to read
  }
  free(buffer);
  fclose(fileItem);
  decompress(TEMP_FILE, appparams.files[fileIndex]);
  remove(TEMP_FILE);
}

/**
 * Write file header to archive file.
 *
 * File header:
 * - name size (int)
 * - name (char*)
 */ 
void writeFileHeader(int fileIndex, FILE *file) {
  char *fileName = appparams.files[fileIndex];
  // check if file exists
  short fileExists = 1;
  FILE *fileItem = fopen(fileName, "rb");
  if (fileItem == NULL) {
    fileExists = 0;
    printf("Cannot find file: %s\n", fileName);
  }
  fclose(fileItem);
  if (fileExists == 0) appExit(1);
  int fileNameLen = strlen(fileName);
  fwrite(&fileNameLen, 1, sizeof(int), file);
  fwrite(fileName, fileNameLen, sizeof(char), file);
}

/**
 * Save file to archive.
 *
 * File:
 * - file size (long)
 * - file (char*)
 */ 
void archiveFile(int fileIndex, FILE *arcFile) {
  char *fileName = appparams.files[fileIndex];
  compress(fileName, TEMP_FILE);
  FILE *fileItem = fopen(TEMP_FILE, "rb");
  // get file size
  fseek(fileItem, 0L, SEEK_END);
  long fileSize = ftell(fileItem);
  fseek(fileItem, 0L, SEEK_SET);
  
  fwrite(&fileSize, 1, sizeof(long), arcFile);
  char *buffer = calloc(BUFFER_SIZE, sizeof(char));
  for (;;) {
    int read = fread(buffer, sizeof(char), BUFFER_SIZE, fileItem);
    if (read <= 0) break;
    fwrite(buffer, sizeof(char), read, arcFile);
    if (read <= BUFFER_SIZE) break;
  }
  free(buffer);
  fclose(fileItem);
  remove(TEMP_FILE);
}

/**
 * Create archive file.
 *
 * FORMAT: HEADER (long), files size (int), files size repeat (int), file headers, file contents.
 */
void createArchive() {
  FILE *file = fopen(appparams.arcFile, "wb"); // open file for write
  if (file == NULL) {
    printf("Cannot create archive file: %s\n", appparams.arcFile);
    appExit(1);
  }
  fseek(file, 0, SEEK_SET); 
  fwrite(&HEADER_START, 1, sizeof(long), file);
  fwrite(&appinfo.filesSize, 1, sizeof(int), file);
  fwrite(&appinfo.filesSize, 1, sizeof(int), file);
  int fileIndex;
  for (fileIndex = 0; fileIndex < appinfo.filesSize; fileIndex++) {
    writeFileHeader(fileIndex, file);
  }
  for (fileIndex = 0; fileIndex < appinfo.filesSize; fileIndex++) {
    archiveFile(fileIndex, file);
  }
  free(appparams.files); // clean file names memory
  fclose(file);
}

/**
 * Print archived file names.
 */ 
void listFiles() {
  FILE *arcFile = readArchiveFile();
  appparams.files = calloc(sizeof(char*), appinfo.filesSize);
  int fileIndex;
  printf("Archived files:\n");
  for (fileIndex = 0; fileIndex < appinfo.filesSize; fileIndex++) {
    readFileHeader(fileIndex, arcFile);
    printf("  %s\n", appparams.files[fileIndex]);
    free(appparams.files[fileIndex]); // clean file name memory
  }
  free(appparams.files); // clean file names memory
  fclose(arcFile);
}

/**
 * Extract files from archive.
 */
void extractFiles() {
  FILE *arcFile = readArchiveFile();
  appparams.files = calloc(sizeof(char*), appinfo.filesSize);
  int fileIndex;
  // read headers
  for (fileIndex = 0; fileIndex < appinfo.filesSize; fileIndex++) {
    readFileHeader(fileIndex, arcFile);
  }
  // read files (after reading all headers)
  for (fileIndex = 0; fileIndex < appinfo.filesSize; fileIndex++) {
    extractFile(fileIndex, arcFile);
    free(appparams.files[fileIndex]); // clean file name memory
  }
  free(appparams.files); // clean file names memory
  fclose(arcFile);
}

void perform() {
  if (appparams.command == LIST) {
    listFiles();
  } else if (appparams.command == EXTRACT) {
    extractFiles();
  } else {
    createArchive();
  }
}

int appExit(int status) {
  exit(status);
}

int main(int argc, char **argv) {
  // compress("test.docx", "testdoc.arc");
  // decompress("testdoc.arc", "test.docx");
  readParams(argc, argv);
  perform();
  appExit(0);
}

