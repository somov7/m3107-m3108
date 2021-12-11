#ifndef PROGA_LABA5_GAMEOFLIFE_H
#define PROGA_LABA5_GAMEOFLIFE_H

#include "../bmpparse/bmpparse.h"

void gameoflife(Matrix matrix);
int countAliveCellNeighbours(Matrix matrix, int y, int x);

#endif //PROGA_LABA5_GAMEOFLIFE_H
