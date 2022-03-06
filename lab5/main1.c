#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct params
{
  char *input; // bmp file
  char *output; // result dir
  int max_iter; // iterations
  int dump_freq; // frequency of dumps
} params;

// params appparams = {"2.bmp", ".", 100, 1}; // test
params appparams = {"", "", -1, 1};

short WHITE = 1;
short BLACK = 0;
const short POINT = 1;
const short NO_POINT = 0;

short **prepareData();
void freeData(short **data);

/**
 * https://jenyay.net/Programming/Bmp
 * http://www.ue.eti.pg.gda.pl/fpgalab/zadania.spartan3/zad_vga_struktura_pliku_bmp_en.html
 */
typedef struct header
{
  unsigned int offset; // where image pixels starts, position in bmp: 10 
  unsigned int width; // position in bmp: 18
  unsigned int height; // position in bmp: 22
  unsigned short bitsByPixel; // amount of bits per pixel
  char *fileHeader; // header till data begin
} header;

header bmpheader;

/**
 * Get params structure from command line.
 */
void readParams(int argc, char *argv[]) {
  int argIndex = 1;
  char *name = "";
  for (; argIndex < argc; argIndex++) {
    if (strlen(name) == 0) {
      name = argv[argIndex];
    } else {
      char *value = argv[argIndex];
      if (strcmp(name, "--input") == 0) {
        appparams.input = value;
      } else if (strcmp(name, "--output") == 0) {
        appparams.output = value;
      } else if (strcmp(name, "--max_iter") == 0) {
        appparams.max_iter = atoi(value);
      } else if (strcmp(name, "--dump_freq") == 0) {
        appparams.dump_freq = atoi(value);
      } 
      name = "";
    }
  }
}

/**
 * Check that params are ok.
 */
int checkParams() {
  if (strlen(appparams.input) == 0 || strlen(appparams.output) == 0) {
    printf("ERROR: wrong params\nUsage:\n--input - path to bmp file with begin state (mandatory)\n--output - path to output dir (mandatory)\n--max_iter - amount of iterations\n--dump_freq - frequency of dumps");
    return 0;
  }
  return 1;
}

/**
 * Read header of file.
 */ 
int readHeader(FILE *file) {
  fseek(file, 0, SEEK_SET);
  unsigned short type;
  if (fread(&type, sizeof(short), 1, file) == 0) return 0;
  if (type != 0x4d42) {
    printf("ERROR: input file is not bmp");
    return 0;
  }
  if (fseek(file, 8, SEEK_CUR) != 0) return 0;
  if (fread(&bmpheader.offset, sizeof(int), 1, file) == 0) return 0;
  if (fseek(file, 4, SEEK_CUR) != 0) return 0;
  if (fread(&bmpheader.width, sizeof(int), 1, file) == 0) return 0;
  if (fread(&bmpheader.height, sizeof(int), 1, file) == 0) return 0;
  if (fseek(file, 2, SEEK_CUR) != 0) return 0;
  if (fread(&bmpheader.bitsByPixel, sizeof(short), 1, file) == 0) return 0;
  printf("HEADER: Offset: %d, Width: %d, Height: %d, BitsByPixel: %d\n", bmpheader.offset, bmpheader.width, bmpheader.height, bmpheader.bitsByPixel);
  if (bmpheader.bitsByPixel != 1) {
    printf("ERROR: input file is not mono bmp, please save with 1 bit per pixel");
    return 0;
  }
  // check if black = 0 or 1
  if (fseek(file, 54, SEEK_SET) != 0) return 0;
  unsigned char colorTable[4];
  if (fread(colorTable, 4, 1, file) == 0) return 0;
  if (colorTable[0] == 0xFF) {
    BLACK = 1;
    WHITE = 0;
  } else {
    BLACK = 0;
    WHITE = 1;
  }
  // get file header to save in iterations
  bmpheader.fileHeader = calloc(sizeof(char), bmpheader.offset);
  if (fseek(file, 0, SEEK_SET) != 0) return 0;
  if (fread(bmpheader.fileHeader, sizeof(char), bmpheader.offset, file) == 0) return 0;
  return 1;
}

/**
 * Read initial state of the game.
 */
void readData(FILE *file, short **gameState) {
  fseek(file, bmpheader.offset, SEEK_SET);
  int x = 0, y = 0, readSymbols = 0;
  while (x < bmpheader.height) {
    unsigned char symbol = fgetc(file);
    if (++readSymbols == 4) {
      readSymbols = 0;
    }
    // process symbol
    short bit;
    for (bit = 7; bit >= 0; bit--) {
      short point = ((1 << bit) & symbol) >> bit; // 1 or 0
      if (point != WHITE) {
        gameState[x][y] = POINT;
      } else {
        gameState[x][y] = NO_POINT;
      }
      y++;
      if (y == bmpheader.width) break; // if line is filled then break
    }
    if (y == bmpheader.width) {
      // read till 4 bytes in one line
      while (readSymbols < 4) {
        fgetc(file);
        readSymbols++;
      }
      readSymbols = 0;
      // go to read next line of points
      x++;
      y = 0;
    }
  }
}

/**
 * Save state iteration to file.
 */
void saveData(short **data, int iteration) {
  // prepare file name
  char *fileName = calloc(1, strlen(appparams.output) + 20);
  sprintf(fileName, "%s/step_%d.bmp", appparams.output, iteration);
  FILE *file = fopen(fileName, "wb"); // open file for write
  free(fileName);
  if (file == NULL) {
    printf("Cannot create file\n");
    exit(0);
  }
  fseek(file, 0, SEEK_SET);
  int written = fwrite(bmpheader.fileHeader, sizeof(char), bmpheader.offset, file);
  if (written != bmpheader.offset) { // check if disc is full
    printf("Cannot write file, disk is full");
    exit(0);
  }
  int x = 0, y = 0, writtenSymbols = 0;
  while (x < bmpheader.height) {
    unsigned char symbol = (char)0;
    short bit;
    for (bit = 7; bit >= 0; bit--) {
      if (data[x][y] == POINT) {
        symbol = symbol | (BLACK << bit);
      } else { // NO POINT
        symbol = symbol | (WHITE << bit);
      }
      y++;
      if (y == bmpheader.width) {
        break; // line is full
      }
    }
    if (fputc(symbol, file) == EOF) { // check if disc is full
      printf("Cannot write file, disk is full");
      exit(0);
    }
    if (++writtenSymbols == 4) {
      writtenSymbols = 0;
    }
    symbol = (char)0;
    if (y == bmpheader.width) {
      // write till 4 bytes in one line
      while (writtenSymbols < 4) {
        if (fputc((char)0, file) == EOF) { // check if disc is full
          printf("Cannot write file, disk is full");
          exit(0);
        }
        writtenSymbols++;
      }
      writtenSymbols = 0;
      x++; // got to next line
      y = 0;
    }
  }
  fclose(file);
}

/**
 * Calculate amount of neighbours for point.
 */ 
int calcNeighbours(short **gameState, int x, int y) {
  int neighbours = 0, subX, subY;
  for (subX = x - 1; subX <= x + 1; subX++) {
    for (subY = y - 1; subY <= y + 1; subY++) {
      if (subX == x && subY == y) continue; // current point
      int nX = subX, nY = subY;
      if (nX == -1) nX = bmpheader.height - 1; // take from other side
      else if (nX == bmpheader.height) nX = 0; // take from other side
      if (nY == -1) nY = bmpheader.width - 1; // take from other side
      else if (nY == bmpheader.width) nY = 0; // take from other side
      if (gameState[nX][nY] == POINT) {
        neighbours++;
      }
    }
  }
  return neighbours;
}

/**
 * One game iteration (wiki).
 *
 * Any live cell with fewer than two live neighbours dies, as if by underpopulation.
 * Any live cell with two or three live neighbours lives on to the next generation.
 * Any live cell with more than three live neighbours dies, as if by overpopulation.
 * Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
 *
 * return 1 if game over (no changes or all pixels die).
 */
int gameIteration(short **gameState) {
  short **newGameState = prepareData();
  int x = 0, y, changed = 0, liveExists = 0;
  for (; x < bmpheader.height; x++) {
    for (y = 0; y < bmpheader.width; y++) {
      int neighbours = calcNeighbours(gameState, x, y);
      if (gameState[x][y] == POINT) {  
        if (neighbours < 2 || neighbours > 3) {
          newGameState[x][y] = NO_POINT;
          changed = 1;
        } else {
          newGameState[x][y] = POINT;
          liveExists = 1;
        }
      } else { // NO_POINT
        if (neighbours == 3) {
          newGameState[x][y] = POINT;
          liveExists = 1;
          changed = 1;
        } else {
          newGameState[x][y] = NO_POINT;
        }
      }
    }
  }
  // move new game state to existing structure of game state
  for (x = 0; x < bmpheader.height; x++) {
    for (y = 0; y < bmpheader.width; y++) {
      gameState[x][y] = newGameState[x][y];
    }
  }
  freeData(newGameState);
  int stop;
  if (changed == 0 || liveExists == 0) {
    stop = 1;
  } else {
    stop = 0;
  }
  return stop;
}

/**
 * Put data in memory (helper).
 */ 
short **prepareData() {
  short **data = calloc(sizeof(short*), bmpheader.height);
  int x = 0;
  for (; x < bmpheader.height; x++) {
    data[x] = calloc(sizeof(short), bmpheader.width);
  }
  return data;
}

/**
 * Clean data memory (helper).
 */ 
void freeData(short **data) {
  int x = 0;
  for (; x < bmpheader.height; x++) {
    free(data[x]);
  }
  free(data);
}

void debugData(short **data) {
  int x = bmpheader.height - 1, y; // first row in array shows last row in image
  for (; x >= 0 ; x--) {
    printf("%d:  ", x);
    for (y = 0; y < bmpheader.width; y++) {
      printf("%d  ", data[x][y]);
    }
    printf("\n--------------\n");
  }
  printf("\n");
}

/**
 * Read data and execute iterations.
 */ 
void game() {
  FILE *file = fopen(appparams.input, "rb"); // open file for read
  if (file == NULL) {
    printf("Cannot open file\n");
    exit(0);
  }
  if (readHeader(file) != 1) {
    fclose(file);
    return;
  }
  short **gameState = prepareData(); // memory for state
  readData(file, gameState); // read init state from file
  fclose(file);
  int iteration = 0, saves = 0;
  // debugData(data);
  for (;;) {
    int stop = gameIteration(gameState);
    // debugData(data);
    iteration++;
    if (stop == 0) {
      if (appparams.max_iter > 0 && iteration == appparams.max_iter) {
        stop = 1; // check if max iteration
      }
    }
    if (iteration % appparams.dump_freq == 0 || stop == 1) {
      saveData(gameState, iteration); // save state to file
      saves++;
      if (saves % 10 == 0 && stop == 0) { // ask every 10 saves if stop
        printf("%d saves, continue? [1/0]: ", saves);
        int next10 = 0;
        scanf("%d", &next10);
        if (next10 == 0) {
          stop = 1;
        }
      }
    }
    if (stop == 1) break;
  }
  freeData(gameState);
  free(bmpheader.fileHeader);
}

int main(int argc, char *argv[]) {
  readParams(argc, argv);
  if (checkParams() == 0) exit(0);
  game();
  return 0;
}

