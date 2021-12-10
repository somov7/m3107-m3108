#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../structs.h"
#include "bmpparse.h"

#define FILE_HEADER_SIZE 14
#define INFO_HEADER_SIZE 40
#define COLOR_TABLE_SIZE 4

Bmp bmpparse(FILE* fp) {
  Bmp returnValue;
  BmpFileHeader fileHeader;
  BmpInfoHeader infoHeader;
  BmpColorTable colorTable;
  BmpGap gap;

  fread(&fileHeader, sizeof(unsigned char), FILE_HEADER_SIZE, fp);
  fread(&infoHeader, sizeof(unsigned char), INFO_HEADER_SIZE, fp);
  fread(&colorTable, sizeof(unsigned char), COLOR_TABLE_SIZE, fp);

  int dataOffset;
  memcpy(&dataOffset, fileHeader.dataOffset, 4);
  gap.size = dataOffset - (FILE_HEADER_SIZE + INFO_HEADER_SIZE + COLOR_TABLE_SIZE);
  gap.arr = malloc(gap.size * sizeof(unsigned char));
  fread(gap.arr, sizeof(unsigned char), gap.size, fp);

  returnValue.fileHeader = fileHeader;
  returnValue.infoHeader = infoHeader;
  returnValue.colorTable = colorTable;
  returnValue.gap = gap;

  return returnValue;
}