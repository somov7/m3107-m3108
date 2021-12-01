#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef struct node{
    char* value;
    struct node* next;
}node;

void push(node** head, char* data);
char* pop(node** head);
node* getSecondLast(node*head);
char* popBack(node** head);