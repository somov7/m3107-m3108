//
// Created by Rustam Tishkov on 09.01.2022.
//

#ifndef LAB5_ARGPARSE_H
#define LAB5_ARGPARSE_H

struct {
    char *output; // result dir
    char *input; // bmp file
    int dumpFreq; // frequency of dumps
    int maxIter; // iterations
} typedef Params;

Params args(int argc, char *argv[]);

#endif //LAB5_ARGPARSE_H
