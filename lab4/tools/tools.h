#ifndef PROGA_LAB4_TOOLS_H
#define PROGA_LAB4_TOOLS_H

#include "../id3v2.h"

int bytesToInt(char* arr, int sevenBitsUsed);

void intToBytes(int integer, char* arr, int sevenBitsUsed);

char* getFrameData(FILE* fp, TagHeader tagHeader, TagFrame frame);

void printFrameData(char* frameId, char* value, char isUnicode);

TagHeader readTagHeader(FILE* fp);

TagFrame readFrame(FILE* fp);

#endif
