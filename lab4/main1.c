#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * https://habr.com/ru/post/103635/
 */

typedef struct params
{
  char *filepath;
  char *get; // get meta by this name
  char *set; // update meta by this name
  char *value; // value to update
  int show; // 1 - show all meta
} params;

typedef struct meta
{
  char name[4];
  char size[4];
  char flags[3];
  int valueSize;
} meta;

const char A = 'A';
const char Z = 'Z';
const int MAIN_HEADER_SIZE = 10;
const int META_HEADER_SIZE = 11;
const char TEMP_FILE_NAME[] = "temp.mp3";
const int BUFFER_SIZE = 10000;

/**
 * Get params structure from command line.
 */
params readParams(int argc, char *argv[]) {
  params params = {"", "", "", "", 0};
  // reading params
  int argIndex = 1;
  for (; argIndex < argc; argIndex++) {
    if (strcmp(argv[argIndex], "--show") == 0) {
      params.show = 1;
    } else {
      char *name = strtok(argv[argIndex], "="); // get value before '='
      char *value = strtok(NULL, "="); // get value after '=' 
      if (strcmp(name, "--filepath") == 0) {
        params.filepath = value;
      } else if (strcmp(name, "--get") == 0) {
        params.get = value;
      } else if (strcmp(name, "--set") == 0) {
        params.set = value;
      } else if (strcmp(name, "--value") == 0) {
        params.value = value;
      }
    }
  }
  // reset other params if show all meta table
  if (params.show == 1) {
    params.get = "";
    params.set = "";
    params.value = "";
  }
  return params;
}

/**
 * Check that params are ok.
 */
int checkParams(params params) {
  int result = 1;
  if (strlen(params.filepath) == 0) {
    result = 0;
  } else if (params.show == 0) {
    if (strlen(params.get) == 0 && strlen(params.set) == 0) {
      result = 0;
    } else if (strlen(params.set) > 0 && strlen(params.value) == 0) {
      result = 0;
    }
  }
  if (result == 0) {
    printf("ERROR: wrong params\nUsage:\n--filepath - path to mp3 file (mandatory)\n--show - show all meta\n--get=<metaname> - show meta by name\n--set=<metaname> --value=<metavalue> - set meta value by name");
  }
  return result;
}

/**
 * Print meta (helper).
 */
void printMeta(meta meta, FILE *file) {
  char *metaValue = calloc(meta.valueSize, sizeof(char));
  fread(metaValue, meta.valueSize, sizeof(char), file);

/*
  char unicode = meta.flags[2];
  // printf("%d\n", unicode);
  if (unicode) {
    wprintf(L"%s: %ls\n", meta.name, metaValue);
  } else {*/
    printf("%s: %s\n", meta.name, metaValue);
  /*}*/
  free(metaValue);
}

/**
 * Put int value to char array (helper).
 */
void intToCharArr(int value, char *target) {
  target[0] = (value >> 21) & 127;
  target[1] = (value >> 14) & 127;
  target[2] = (value >> 7) & 127;
  target[3] = value & 127;
}

/**
 * Put char array to int value (helper).
 */
int charArrToInt(char *src) {
  int value = ((src[0] & 127) << 21) | ((src[1] & 127) << 14) | ((src[2] & 127) << 7) | (src[3] & 127);
  return value;
}

/**
 * Create another file with updated meta.
 * 
 * src file pointer is on the end of meta.
 */
void updateFile(params params, meta meta, FILE *file) {
  FILE *targetFile = fopen(TEMP_FILE_NAME, "wb"); // open file for write

  // get position of meta header start (note: current position is after meta header finish)
  long metaPos;
  fpos_t *metaPos_t = &metaPos;
  fgetpos(file, metaPos_t);
  metaPos -= META_HEADER_SIZE; // meta header size

  // get value difference
  int newValueSize = strlen(params.value);
  int difference = newValueSize - meta.valueSize;

  // rewrite size in start header
  fseek(file, 0, SEEK_SET); 
  char *buffer = calloc(MAIN_HEADER_SIZE, sizeof(char));
  fread(buffer, MAIN_HEADER_SIZE, sizeof(char), file);
  int sizeInHeader = charArrToInt(buffer + 6);
  int newSizeInHeader = sizeInHeader + difference;
  intToCharArr(newSizeInHeader, buffer + 6);
  fwrite(buffer, MAIN_HEADER_SIZE, sizeof(char), targetFile);
  free(buffer);

  // read after header and before meta name 
  buffer = calloc(metaPos - MAIN_HEADER_SIZE, sizeof(char));
  fread(buffer, metaPos - MAIN_HEADER_SIZE, sizeof(char), file);
  fwrite(buffer, metaPos - MAIN_HEADER_SIZE, sizeof(char), targetFile);
  free(buffer);

  // change value size in meta and put to file
  newValueSize++; // add to size one byte at the end of header
  intToCharArr(newValueSize, meta.size);
  fwrite(&meta, 1, META_HEADER_SIZE, targetFile);

  // put new value
  fwrite(params.value, strlen(params.value), sizeof(char), targetFile);
  
  // move to next meta after changed
  fseek(file, META_HEADER_SIZE + meta.valueSize, SEEK_CUR);
  
  // write the rest of file
  buffer = calloc(BUFFER_SIZE, sizeof(char));
  for (;;) {
    int read = fread(buffer, sizeof(char), BUFFER_SIZE, file);
    if (read <= 0) break;
    fwrite(buffer, sizeof(char), read, targetFile);
    if (read < BUFFER_SIZE) break;
  }
  free(buffer);
  fclose(targetFile);
}

/**
 * Read file.
 */
void processFile(params params) {
  FILE *file = fopen(params.filepath, "rb"); // open file for read as binary
  if (file == NULL) {
    printf("Cannot open file\n");
    exit(0);
  }
  fseek(file, MAIN_HEADER_SIZE, SEEK_SET); // skip first 10 bytes from begin (main header size)
  meta meta;
  int updated = 0;
  while (fread(&meta, META_HEADER_SIZE, 1, file) > 0) {
    if (meta.name[0] < A || meta.name[0] > Z) {
      break; // meta are finished
    }
    meta.valueSize = charArrToInt(meta.size);
    meta.valueSize--; // last byte of meta header adds 1 to value size but not a part of value
    if (params.show != 1) {
     if (strcmp(meta.name, params.get) != 0 && strcmp(meta.name, params.set) != 0) {
        // if not to show all table of meta and current meta is not found
        fseek(file, meta.valueSize, SEEK_CUR); // go to next meta
        continue;
      }
    }
    if (strlen(params.set) > 0) { // if set meta value
      updateFile(params, meta, file); // note: current file is on position before changed value
      updated = 1;
      break; // can be only one set -> exit while
    }
    printMeta(meta, file);
    if (strlen(params.get) > 0) {
      break; // can be only one get -> exit while
    }
  }
  fclose(file);
  if (updated) { // if set then rename file
    remove(params.filepath);
    rename(TEMP_FILE_NAME, params.filepath);
  }
}

int main(int argc, char *argv[]) {
  params params = readParams(argc, argv);
  if (checkParams(params) == 0) exit(0);
  processFile(params);
  return 0;
}

