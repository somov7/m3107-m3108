//
// Created by Антон Чемодуров on 30.11.2021.
//

#include "gamelive.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

char next(uint16_t w, uint16_t h, char *bitmap) {
    char *tmp = malloc((w + 2)*(h + 2));
    int w1 = w+2;
    for (int i = 1; i < h; i++) {
        for (int j = 1; j < w; ++j) {
            int p = i*w1 + j;
            tmp[p] = bitmap[p - 1] + bitmap[p + 1] +
                    bitmap[p - w1] + bitmap[p + w1] +
                    bitmap[p - w1 - 1] + bitmap[p - w1 + 1] +
                    bitmap[p + w1 + 1] + bitmap[p + w1 -1];
        }
    }
    for (int i = 1; i < h; ++i) {
        for (int j = 1; j < w; ++j) {
            int p = j*w1 + i;
            if (!bitmap[p] && tmp[p] == 3) {
                bitmap[p] = 1;
            } else {
                if (tmp[p] > 3 || tmp[p] < 2) {
                    bitmap[p] = 0;
                }
            }
        }
    }
    free(tmp);
    return is_over(w, h, bitmap);
}

char is_over(uint16_t w, uint16_t h, char *bitmap) {
    return 1;
}