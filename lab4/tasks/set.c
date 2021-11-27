#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../id3v2.h"
#include "../tools/tools.h"
#include "tasks.h"

#define TAG_HEADER_SIZE 10
#define FRAME_SIZE 11

void set(FILE* fp, char* propName, char* prop_Value) {
  FILE* output = fopen("output.mp3", "wb");
  int isFound = get(fp, propName);

  fseek(fp, 0, SEEK_SET);

  TagHeader tagHeader = readTagHeader(fp);
  TagFrame frame = readFrame(fp);

  int tagHeaderSize = bytesToInt(tagHeader.size, 1); // uses seven bits ID3v2 size = 4 * %0xxxxxxx

  if (isFound != 0) {
    fwrite(&tagHeader, sizeof(char), TAG_HEADER_SIZE, output);
  } else {
    //int a = bytesToInt(tagHeader.size, 1);

    // write new tag header size
    int tagSizeNewInteger = tagHeaderSize + strlen(prop_Value) + FRAME_SIZE;

    char tagSizeNewString[4];
    intToBytes(tagSizeNewInteger, tagSizeNewString, 1);

    memcpy(tagHeader.size, tagSizeNewString, 4);

    //a = bytesToInt(tagHeader.size, 1);

    fwrite(&tagHeader, sizeof(char), TAG_HEADER_SIZE, output);

    int pos = 0;
    // write existing frames
    while (frame.frameId[0] != '\0') {
      char* frameData = getFrameData(fp, tagHeader, frame);

      int sevenBitsUsed = ((tagHeader.majorVersion) == 4 ? 1 : 0);
      int frameSize = bytesToInt(frame.size, sevenBitsUsed);

      fwrite(&frame, sizeof(char), FRAME_SIZE, output);
      fwrite(frameData, sizeof(char), frameSize, output);

      frame = readFrame(fp);
      pos += FRAME_SIZE + frameSize;
    }

     adding new frame
    TagFrame newFrame;
    int sevenBitsUsed = ((tagHeader.majorVersion) == 4 ? 1 : 0); // in id3v2.4.0 uses seven bits Size 4 * %0xxxxxxx
    int frameSizeInteger = strlen(prop_Value) / sizeof(char);
    char frameSizeString[4];
    intToBytes(frameSizeInteger, frameSizeString, sevenBitsUsed);

    memcpy(newFrame.frameId, propName, sizeof(newFrame.frameId));
    memcpy(newFrame.size, frameSizeString, sizeof(frameSizeString));
    memcpy(newFrame.flags, "  ", sizeof("  "));
    newFrame.isUnicode = '\0';

    fwrite(&newFrame, sizeof(char), FRAME_SIZE, output);
    fwrite(prop_Value, sizeof(char), sizeof(prop_Value), output);


//     write all other data
    int c;
    while ((c = getc(fp)) != '\n') {}

    while ((c = getc(fp)) != EOF) {
      fputc(c, output);
    }

    fclose(output);
  }
}
