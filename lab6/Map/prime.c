//
// Created by Антон Чемодуров on 12.12.2021.
//

#include <stdint.h>
#include <stdlib.h>

#include "prime.h"
#include "bitsarray.h"

Bool *sieveEratos(uint64_t n) {
    Bool *sieve = calloc(n/8 + 1, 1);
    for (uint64_t i = 2; i < n; i++) {
        if (get(sieve, i) == 0) {
            for (uint64_t j = i*i; j < n; j += i) {
                set(sieve, 1, j);
            }
        }
    }
    return sieve;
}
uint64_t get_prime(uint64_t a, uint64_t b) {
    static Bool *primes = NULL;
    if (primes == NULL) {
        primes = sieveEratos(1000000);
    }
    for (uint64_t i = rand() % a + b; i >= 0; i--) {
        if (get(primes, i) == 0) {
            return i;
        }
    }
    return 0;
}