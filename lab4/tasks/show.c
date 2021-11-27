#include <stdio.h>
#include <stdlib.h>

#include "../id3v2.h"
#include "../tools/tools.h"

#define TAG_HEADER_SIZE 10
#define FRAME_SIZE 11

void show (FILE* fp) {
  TagHeader tagHeader = readTagHeader(fp);
  TagFrame frame = readFrame(fp);

  int tagSize = bytesToInt(tagHeader.size, 1); // uses seven bits ID3v2 size = 4 * %0xxxxxxx
  printf("Tag size: %d\n", tagSize);

  while (frame.frameId[0] != '\0') {
    char* frameData = getFrameData(fp, tagHeader, frame);
    printFrameData(frame.frameId, frameData, frame.isUnicode);

    free(frameData);

    frame = readFrame(fp);
  }
}