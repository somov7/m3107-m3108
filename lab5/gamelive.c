//
// Created by Антон Чемодуров on 30.11.2021.
//

#include "gamelive.h"
#include <stdint.h>
#include <stdlib.h>

char next(uint32_t w, uint32_t h, char *bitmap) {
    char *tmp = malloc(w * h);
    for (int i = 1; i < h -1; ++i) {
        for (int j = 1; j < w -1; ++j) {
            int p = i * w + j;
            tmp[p] = bitmap[p-1] + bitmap[p + 1] +
                     bitmap[p + w] + bitmap[p - w] +
                     bitmap[p + w -1] + bitmap[p + w + 1] +
                     bitmap[p - w -1] + bitmap[p - w + 1];
        }
    }
    for (int i = 1; i < h -1; ++i) {
        for (int j = 1; j < w - 1; ++j) {
            int p = i * w + j;
            if (tmp[p] == 3 && !bitmap[p]) {
                bitmap[p] = 1;
            }
            if ((tmp[p] < 2 || tmp[p] > 3) && bitmap[p]) {
                bitmap[p] = 0;
            }
        }
    }
    free(tmp);
    return is_over(w, h, bitmap);
}

char is_over(uint16_t w, uint16_t h, char *bitmap) {
    return 1;
}