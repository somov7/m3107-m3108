//
// Created by Антон Чемодуров on 12.12.2021.
//

#include "bitsarray.h"
#include "bitsarray.h"
#include <bitstring.h>

void set(Bool *arr, char a, uint64_t i) {
    arr[i / 8] |=  a << (i % 8);
}

char get(const Bool *arr, uint64_t i) {
    return (arr[i / 8] & (1 << (i % 8))) >> (i % 8);
}