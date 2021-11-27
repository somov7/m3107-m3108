#include <stdlib.h>
#include <wchar.h>

#include "tools.h"

#define TAG_HEADER_SIZE 10
#define FRAME_SIZE 11

int bytesToInt(char* arr, int sevenBitsUsed) {
  int result;
  if (sevenBitsUsed == 1) {
    result = (arr[0] << 21) + (arr[1] << 14) + (arr[2] << 7) + arr[3];
  } else {
    result = (arr[0] << 24) + (arr[1] << 16) + (arr[2] << 8) + arr[3];
  }
  return result;
}

void intToBytes(int integer, char* arr, int sevenBitsUsed) {
  int bits;
  if (sevenBitsUsed == 1) {
    bits = 127;
    arr[0] = (char)((integer >> 21) & bits);
    arr[1] = (char)((integer >> 14) & bits);
    arr[2] = (char)((integer >> 7) & bits);
    arr[3] = (char)(integer & bits);
  } else {
    bits = 255;
    arr[0] = (char)((integer >> 24) & bits);
    arr[1] = (char)((integer >> 16) & bits);
    arr[2] = (char)((integer >> 8) & bits);
    arr[3] = (char)(integer & bits);

  }
}


char* getFrameData(FILE* fp, TagHeader tagHeader, TagFrame frame) {
  int sevenBitsUsed = ((tagHeader.majorVersion) == 4 ? 1 : 0); // in id3v2.4.0 uses seven bits Size 4 * %0xxxxxxx
  int frameSize = bytesToInt(frame.size, sevenBitsUsed);

  char* returnValue = calloc(frameSize, sizeof(char));
  fread(returnValue, sizeof(char), frameSize - 1, fp);

  return returnValue;
}

void printFrameData(char* frameId, char* frameData, char isUnicode) {
  if (isUnicode == 1) {
    wprintf(L"%ls: %ls\n", frameId, frameData);
  } else {
    printf("%s: %s\n", frameId, frameData);
  }
}

TagHeader readTagHeader(FILE* fp) {
  TagHeader returnValue;
  int status = fread(&returnValue, sizeof(char), TAG_HEADER_SIZE, fp);
  if (status != 0) {
    return returnValue;
  } else {
    exit(1);
  }
}

TagFrame readFrame(FILE* fp) {
  TagFrame returnValue;
  int status = fread(&returnValue, sizeof(char), FRAME_SIZE, fp);
  if (status != 0) {
    return returnValue;
  } else {
    returnValue.frameId[0] = '\0';
    return returnValue;
  }
}