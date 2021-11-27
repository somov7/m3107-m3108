#ifndef PROGA_LAB4_ID3V2_H
#define PROGA_LAB4_ID3V2_H

#pragma pack(push, 1) // not to allow paddings in memory just for fwrite

typedef struct TagHeader {
  unsigned char headerId[3];
  unsigned char majorVersion;
  unsigned char subVersion;
  unsigned char flags;
  unsigned char size[4];
} TagHeader;

#pragma pack(pop)

typedef struct TagFrame {
  unsigned char frameId[4];
  unsigned char size[4];
  unsigned char flags[2];
  unsigned char isUnicode;
} TagFrame;

#endif
