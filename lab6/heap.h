#ifndef HEAP
#define HEAP

typedef struct Node
{
    unsigned char byte;
    int freq;
    struct Node* left;
    struct Node* right;
} Node;

typedef struct Vector
{
    Node** arr;
    int sz;
    int cap;
} Vector;

Node* Node_new(unsigned char byte, int freq);
Vector* Vector_new();
void Vector_free(Vector* v);
void Vector_push(Vector* v, Node* x);
void Vector_pop(Vector* v);
void Vector_print(Vector* v);
void Node_swap(Node** l, Node** r);
void Node_free(Node* x);
void shiftdown(Vector* v, int i);
Node* extractmin(Vector* v);
void buildheap(Vector* v);
void Node_insert(Vector* v, Node* x);



#endif