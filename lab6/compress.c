#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Tree {
  unsigned int leafFreq[256];
  unsigned int nodeFreq[256];
  int leafParent[256];
  int nodeParent[256];
  int symbolNumberAndVal[256];
  int bl_count[256];
};

struct Symbol {
  int hcod; // code of symbol
  int bits; // length of symbol
};

struct DstFile {
  char buf[32768]; // buffer
  FILE *file; // file to write
  int sizefile; // file size
  int bit_count; // counter of bits
  unsigned int ch; // buffer of bits

  unsigned int freq[256]; // frequency of symbols 
  struct Tree tree[1]; // tree
  struct Symbol sym[256]; // code table (256 symbols)
  char bufh[16+256]; // header table
};

/**
 * Calculate symbol codes.
 */ 
int calcSymbolCodes(struct Tree *tree, struct Symbol *sym, char *buf, unsigned int *freq) {
  int i, j, k, n, cod, bits, len;
  unsigned int t, aa, ab, bb;
  unsigned int *leafFreq  = tree->leafFreq; // frequiencies of leaves
  unsigned int *nodeFreq  = tree->nodeFreq; // frequiencies of nodes
  int *leafParent = tree->leafParent; // parent node of leaf
  int *nodeParent = tree->nodeParent; // parent node of node
  int *symbolNumberAndVal = tree->symbolNumberAndVal; // number and value of symbol
  int *bl_count = tree->bl_count; // counters of code for different lengths

  // prepare source arrays
  memset(buf, 0, 16+256);
  memset(tree, 0, sizeof(struct Tree));
  memset(sym, 0, 256 * sizeof(struct Symbol));
  for (i=0, n=0; i < 256; i++) {
    if (freq[i] > 0) {
      leafFreq[n] = freq[i];
      symbolNumberAndVal[n++] = i;
    }
  }
  memset(freq, 0, 256 * sizeof(unsigned int));
  if (n == 0) {
    return 0; // n - amount of used alphabet symbols
  } else if (n == 1) {
    bl_count[1] = 1;
  } else {
    // sort by symbol frequency, ascending
    for (i=0; i < n; i++) {
      for (j=n-1; j > i; j--) {
        if (leafFreq[j] < leafFreq[j-1]) {
          t = leafFreq[j];
          leafFreq[j] = leafFreq[j-1];
          leafFreq[j-1] = t;
          t = symbolNumberAndVal[j];
          symbolNumberAndVal[j] = symbolNumberAndVal[j-1];
          symbolNumberAndVal[j-1] = t;
        }
      }
    }

    // create tree
    for(k=0, i=0, j=0; k < (n-1); k++) {
      if (i < (n-1)) {
        aa = leafFreq[i] + leafFreq[i+1];
      } else {
        aa = UINT_MAX;
      }
      if (i < n && j < k) {
        ab = leafFreq[i] + nodeFreq[j];
      } else {
        ab = UINT_MAX;
      }
      if (j < (k-1)) {
        bb = nodeFreq[j] + nodeFreq[j+1];
      } else {
        bb = UINT_MAX;
      }
      if(aa <= ab && aa <= bb) {
        nodeFreq[k] = aa;
        leafParent[i++] = k;
        leafParent[i++] = k; // parent node
      } else if(ab <= aa && ab <= bb) {
        nodeFreq[k] = ab;
        leafParent[i++] = k;
        nodeParent[j++] = k; // parent node
      } else {
        nodeFreq[k] = bb;
        nodeParent[j++] = k;
        nodeParent[j++] = k; // parent node
      }
    }

    // look through tree
    for (--k, i=0; i < n; i++) { // k - index of root node
      for(len=1, j=leafParent[i]; j < k; j=nodeParent[j], len++);
      bl_count[len]++;
    }

    // decrease symbols lengths
    for (i=n-1; i > 16; i--) {
      for(; bl_count[i]>0; ) {
        for (j=i-2; j>0 && bl_count[j]<=0; j--);
        bl_count[i] -= 2;
        bl_count[i-1]++;
        bl_count[j]--;
        bl_count[j+1] += 2;
      }
    }
  }

  // prepare result array
  if (bl_count[8] == 256) {
    bl_count[8] = 255;
    bl_count[9] = 1;
  }
  for (i=1; i<=16; i++) {
    *buf++ = bl_count[i];
  }
  for (cod=0, i=n-1, bits=1; bits <= 16; bits++) {
    for (j=0; j < bl_count[bits]; j++) {
      *buf++ = t = symbolNumberAndVal[i--];
      sym[t].bits = bits;
      sym[t].hcod = cod++;
    }
    cod = cod << 1;
  }
  return n;
}

/**
 * Write symbol to dst file.
 */
int writeSymbol(int ch, struct DstFile *dstFileS) {
  int k = fputc(ch, dstFileS->file);
  if (k < 0) {
    return -1;
  }
  dstFileS->sizefile++;
  return k;
}

/**
 * Write string to file.
 */
int writeString(const char *buf, int n, struct DstFile *dstFileS) {
  int k = fwrite(buf, 1, n, dstFileS->file);
  if(k != n) {
    return -1;
  }
  dstFileS->sizefile += n;
  return(n);
}

/**
 * Write bits to file.
 */
int writeBits(int cod, int bits, struct DstFile *dstFileS) {
  int value;
  dstFileS->ch = (dstFileS->ch << bits) + (((1 << bits) - 1) & cod);
  dstFileS->bit_count += bits;
  while(dstFileS->bit_count >= 8) {
    dstFileS->bit_count -= 8;
    value = (dstFileS->ch >> dstFileS->bit_count) & 255;
    if (writeSymbol(value, dstFileS) < 0) {
      return -1;
    }
  }
  return 0;
}

/**
 * Flush bits to file.
 */ 
int flushBits(struct DstFile *dstFileS) {
  int value, bits;
  bits = dstFileS->bit_count;
  if (bits > 0) {
    // write to file
    value = dstFileS->ch << (8 - bits);
    if(writeSymbol(value, dstFileS) < 0) {
      return -1; 
    }
  }
  dstFileS->bit_count = 0; // reset bits counter
  dstFileS->ch = 0; // reset bits buffer
  return 0;
}

/**
 * Put header to destination file.
 */ 
int putHeader(struct DstFile *dstFileS, int dl1, int dl2) {
    char buf[4];
    buf[0] = ((dl1) >> 8) & 255;
    buf[1] = (dl1) & 255;
    buf[2] = ((dl2) >> 8) & 255;
    buf[3] = (dl2) & 255;
    if (writeString(buf, 4, dstFileS) < 0) {
      return -1;
    }
    if (writeString(dstFileS->bufh, dl1, dstFileS) < 0) {
      return -1;
    }
    return 0;
}

/**
 * Encode and write block.
 */ 
int encodeAndWriteBlock(struct DstFile *dstFileS, int len) {
  unsigned int *freq = dstFileS->freq;
  struct Symbol *sym = dstFileS->sym;
  char *p1, *p2;
  for(p1 = dstFileS->buf, p2 = p1 + len; p1 < p2; p1++){
    freq[*p1 & 255]++;
  }
  int k = calcSymbolCodes(dstFileS->tree, sym, dstFileS->bufh, freq);
  if (putHeader(dstFileS, k+16, len) < 0) {
    return -1;
  }
  for(p1 = dstFileS->buf, p2 = p1 + len; p1 < p2; p1++) {
    k = *p1 & 255;
    if (writeBits(sym[k].hcod, sym[k].bits, dstFileS) < 0) {
      return -1;
    }
  }
  if (flushBits(dstFileS) < 0) {
    return -1;
  }
  return 0;
}

/**
 * Perform encoding.
 */
int encode(char *srcName, char *dstName) {
  FILE *srcFile = NULL, *dstFile = NULL;
  srcFile = fopen(srcName, "rb");
  if (srcFile == NULL) {
    return -1;
  }
  dstFile = fopen(dstName, "wb");
  if (dstFile == NULL) {
    fclose(srcFile);
    return -1;
  }
  // prepare structure for dst file
  struct DstFile *dstFileS = NULL;
  dstFileS = (struct DstFile*)malloc(sizeof(struct DstFile));
  memset(dstFileS, 0, sizeof(struct DstFile));
  dstFileS->file = dstFile;

  // read source file
  int len, error = 0;
  for (;;) {
    len = fread(dstFileS->buf, 1, sizeof(dstFileS->buf), srcFile);
    if(len != sizeof(dstFileS->buf) && feof(srcFile) == 0) {
	  error = 1;
	  break;
    }
    if(len == 0) break;
    if(encodeAndWriteBlock(dstFileS, len) < 0) {
      error = 1;
      break;
    }
  }
  if(writeString("\0\0\0\0", 4, dstFileS) < 0) {
    error = 1;
  }
  fclose(srcFile);
  fclose(dstFile);
  free(dstFileS);
  return error == 1 ? -1 : 0;
}

/**
 * Compress src file and save result to dst file.
 */
int compress(char *src, char *dst) {
    int result = encode(src, dst);
    return result;
}


