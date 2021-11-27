#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../id3v2.h"
#include "../tools/tools.h"

#define TAG_HEADER_SIZE 10
#define FRAME_SIZE 11

int get (FILE* fp, char* propName) {
  TagHeader tagHeader = readTagHeader(fp);
  TagFrame frame = readFrame(fp);

  int foundFramePosition = TAG_HEADER_SIZE + FRAME_SIZE;

  int tagSize = bytesToInt(tagHeader.size, 1); // uses seven bits ID3v2 size = 4 * %0xxxxxxx

  while (frame.frameId[0] != '\0') {
    char* frameData = getFrameData(fp, tagHeader, frame);

    if (strcmp(propName, frame.frameId) == 0) {
      printFrameData(frame.frameId, frameData, frame.isUnicode);
      free(frameData);

      return foundFramePosition;
    }

    foundFramePosition += bytesToInt(frame.size, 0);

    frame = readFrame(fp);
  }

  return 0;
}