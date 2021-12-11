#include <string.h>

#include "../structs.h"
#include "bmpgenerate.h"

#define FILE_HEADER_SIZE 14
#define INFO_HEADER_SIZE 40
#define COLOR_TABLE_SIZE 4
#define GAP_SIZE 4

void bmpgenerate(Bmp bmp,Matrix matrix , char* output) {
  FILE* fp;
  fp = fopen(output, "w+");

  int imageSize, width, height;
  memcpy(&width, bmp.infoHeader.width, 4);
  memcpy(&height, bmp.infoHeader.height, 4);
  memcpy(&imageSize, bmp.infoHeader.imageSize, 4);

  int newDataOffset = (FILE_HEADER_SIZE + INFO_HEADER_SIZE + COLOR_TABLE_SIZE + GAP_SIZE);
  memcpy(bmp.fileHeader.dataOffset, &newDataOffset, 4);

  int newFileSize = (FILE_HEADER_SIZE + INFO_HEADER_SIZE + COLOR_TABLE_SIZE + GAP_SIZE + imageSize);
  memcpy(bmp.fileHeader.fileSize, &newFileSize, 4);

  fwrite(&bmp.fileHeader, sizeof(unsigned char), FILE_HEADER_SIZE, fp);
  fwrite(&bmp.infoHeader, sizeof(unsigned char), INFO_HEADER_SIZE, fp);
  fwrite(&bmp.colorTable, sizeof(unsigned char), COLOR_TABLE_SIZE, fp);
  fwrite(bmp.gap.arr, sizeof(unsigned char), bmp.gap.size, fp);

  int rowSize = ((1 * width + 31) / 32) * 4; // in bytes

  unsigned char c = 0;

  for (int i = height - 1; i > -1; --i) {
    int counter = 0,
      bytesCount = 0;

    for (int j = 0; j < width; ++j) {
      int pixel = matrix.arr[i][j];
      pixel = pixel << (7 - counter);
      c = c | pixel;

      if (counter == 7) {
        fwrite(&c, sizeof(unsigned char), 1, fp);
        ++bytesCount;
        c = (unsigned char)(c & 00000000);
        counter = 0;
        continue;
      }

      ++counter;
    }

    if (counter != 0) {
      fwrite(&c, sizeof(unsigned char), 1, fp);
      ++bytesCount;
    }

    c = 0;
    for (int j = bytesCount; j < rowSize; ++j) {
      fwrite(&c, sizeof(unsigned char), 1, fp);
    }
  }

  fclose(fp);
}