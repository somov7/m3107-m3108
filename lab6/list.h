#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef struct node{
    char* value;
    struct node* next;
}node;

void push(node** head, char* data);
node* getSecondLast(node*head);
char* popBack(node** head);