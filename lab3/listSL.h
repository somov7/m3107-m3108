typedef struct node{
    char* value;
    struct node* next;
}node;

void push(node** head, char* data);
char* pop(node** head);
node* getSecondLast(node*head);
char* popBack(node** head);

#ifndef LOGPARSE_LISTSL_H
#define LOGPARSE_LISTSL_H

#endif //LOGPARSE_LISTSL_H
