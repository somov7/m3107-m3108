//
// Created by Антон Чемодуров on 03.12.2021.
//

#ifndef LAB5_BINARYSEARCHTREE_H
#define LAB5_BINARYSEARCHTREE_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    uint16_t *key;
    size_t size;
}Key;

typedef struct Node {
    struct Node *left;
    struct Node *right;
    uint16_t value;
    Key key;
}Node;

typedef Node* BinarySearchTree;

BinarySearchTree init(char size);
void BTS_add(BinarySearchTree bts, Key key, uint16_t value);
char exist(BinarySearchTree bts, uint16_t *key);


#endif //LAB5_BINARYSEARCHTREE_H

