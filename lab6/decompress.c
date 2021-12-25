#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Context {
  const char *currentPosition;  // current position of reading buffer
  const char *rightPosition;    // end of reading buffer data
  char buf_rd[512];             // reading buffer
  char buf[4096];               // buffer
  int bit_count;                // counter of bits
  unsigned int ch;              // buffer of bits
  FILE *srcFile;                // src file
  FILE *dstFile;                // dst file
  int sizefile;                 // size of dst file
  int err;

  // Huffman table
  struct {
    char sym[256]; // symbols sorted by code increase
    int n;         // amount of symbols
    int max[17];   // all codes of length less than max[length]
    int idx[17];   
    int arr[256];  // array of fast decode
  } shfm;
};

/**
 * Read symbol from src file.
 */
int readSymbol(struct Context *context) {
  int k;
  if (context->currentPosition >= context->rightPosition) {
    // if all from read buffer is process then read new portion
    k = fread(context->buf_rd, 1, sizeof(context->buf_rd), context->srcFile);
    if (k <= 0) return -1;
    context->currentPosition = context->buf_rd;
    context->rightPosition = context->buf_rd + k;
  }
  return (*context->currentPosition++) & 255;
}

/**
 * Read block from src file.
 */
int readBlock(char *buf, int blockSize, struct Context *context) {
  // first take the block from already read buffer
  int k = context->rightPosition - context->currentPosition;
  int lenOfRead = 0;
  if (k > 0) {
    if (k > blockSize) {
      k = blockSize; 
    }
    memmove(buf, context->currentPosition, k);
    lenOfRead += k;
    context->currentPosition += k;
  }
  // if buffer is not enough then read the rest from file
  if (lenOfRead < blockSize) {
    k = fread(buf + lenOfRead, 1, blockSize - lenOfRead, context->srcFile);
    if (k != (blockSize - lenOfRead)) return -1;
  }
  return 0;
}

/**
 * Prepare Huffman table.
 */
int prepareHuffmanTable(const char *buf, int Lbuf, struct Context *context) {
  if (Lbuf < 16) return -1;
  int i, j, k, k1, k2, n, n1, cod, len, ch;

  for (i=0, n=0; i<16; i++) n = n + (buf[i] & 255); // calculate symbols
  if (n == 0 || n > 256 || (n+16) > Lbuf) return -1;

  memset(&context->shfm, 0, sizeof(context->shfm));
  memmove(context->shfm.sym, buf + 16, n);
  context->shfm.n = n;

  // table for slow decode
  for (len = 1, cod = 0, i=0; len <= 16; len++) {
    k = buf[len-1] & 255; // count of codes of currnt length
    if (k > 0) {
      context->shfm.idx[len] = i - cod;
      i += k;
      cod += k;
      if((i < n && cod > ((1 << len) - 1)) || (i == n && (cod - 1) > ((1 << len) - 1))) {
        return -1;
      }
      context->shfm.max[len] = cod; // max code of current length + 1
    }
    cod <<= 1;
  }
  
  // table for fast decode
  for(len = 1, cod = 0, i=16; len <= 8; len++){
    k = buf[len-1] & 255;
    if (k > 0) {
      for(; k > 0; k--) {
        ch = buf[i++] & 255;
        n1 = (1 << (8 - len)) - 1;
        k1 = (ch << 4) + len;
        k2 = (cod << (8-len));
        for(j=0; j<=n1; j++){
          context->shfm.arr[k2+j] = k1;
        }
        cod++;
      }
    }
    cod <<= 1;
  }
  return n;
}

/**
 * Get Huffman code.
 */ 
int getHuffmanCode(struct Context *context) {
  int *max, cod, len, k, bits, a;
  if (context->bit_count < 8) {
    k = readSymbol(context);
    if(k < 0) return(-1);
    context->ch = (context->ch << 8) | k;
    context->bit_count += 8;
  }
  cod = (context->ch >> (context->bit_count - 8)) & 255;
  a = context->shfm.arr[cod];
  bits = a & 15;
  if (bits > 0) {
    context->bit_count -= bits;
    return (a >> 4);
  }
  max = context->shfm.max; // arrays of max codes for different lengths
  context->bit_count -= 8; // 8-bit prefix (cod)
  for (len = 9; len <= 16; len++) {
    if(context->bit_count <= 0) {
      k = readSymbol(context);
      if(k < 0) return(-1);
      context->ch = k;
      context->bit_count = 8;
    }
    cod = (cod << 1) | ((context->ch >> (--(context->bit_count))) & 1);
    if (cod < max[len]) {
      k = context->shfm.idx[len] + cod;
      return (context->shfm.sym[k] & 255);
    }
  }
  return -1;
}

/**
 * Read byte from src file.
 */
int readByte(struct Context *context, int *k1) {
  if (context->bit_count < 8) {
    context->bit_count = 0;
    return 0;
  }
  *k1 = context->ch & 255;
  context->bit_count = 0;
  return 1;
}

/**
 * Read header of block.
 */
int readHeader(struct Context *context) {
  int k, k1, len1, len2;
  char buf[16+256];
  k = readByte(context, &k1);
  if (k == 0) {
    k1 = readSymbol(context);
    if (k1 < 0) return(-1);
  }
  buf[0] = k1;
  if (readBlock(buf+1, 3, context) < 0) return -1;
  len1 = ((buf[0] & 255) << 8) + (buf[1] & 255);
  if (len1 == 0) return 0;
  len2 = ((buf[2] & 255) << 8) + (buf[3] & 255);
  if (len1 > (16+256)) return -1;
  if (readBlock(buf, len1, context) < 0) return -1;
  if (prepareHuffmanTable(buf, len1, context) < 0) return -1;
  return len2;
}

/**
 * Decode file.
 */
int decodeFile(struct Context *context) {
  int k, n, len=0, Sbuf = sizeof(context->buf);
  char *p1, *p2, *buf = context->buf;
  while (1) {
    if (len == 0) {
      len = readHeader(context);
      if (len < 0) return -1;
      if (len == 0) break;
    }
    if(len > Sbuf) n = Sbuf;
    else n = len;
    for (p1 = buf, p2 = p1+n; p1 < p2; p1++){
      k = getHuffmanCode(context);
      if (k < 0) return -1;
      *p1 = k;
    }
    len -= n;
    context->sizefile += n;
    k = fwrite(buf, 1, n, context->dstFile);
    if (k != n) return -1;
  }
  return(context->sizefile);
}

/**
 * Decode file and save to another file.
 */
int decode(char *srcName, char *dstName) {
  FILE *srcFile = NULL, *dstFile = NULL;
  srcFile = fopen(srcName, "rb");
  if (srcFile == NULL) return -1;
  dstFile = fopen(dstName, "wb");
  if (dstFile == NULL) {
    fclose(srcFile);
    return -1;
  }
  struct Context *context = (struct Context*)malloc(sizeof(struct Context));
  memset(context, 0, sizeof(struct Context));
  context->srcFile = srcFile;
  context->dstFile = dstFile;
  context->currentPosition = context->rightPosition = context->buf_rd;

  int result = decodeFile(context);

  fclose(srcFile);
  fclose(dstFile);
  free(context);
  return result < 0 ? -1 : 0;
}

/**
 * Decompress src file and save result to dst file.
 */
int decompress(char *src, char *dst) {
  int result = decode(src, dst);
  return result;  
}
