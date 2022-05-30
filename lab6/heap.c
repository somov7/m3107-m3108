#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "heap.h"

Node* Node_new(unsigned char byte, int freq)
{
    Node* t = malloc(sizeof(Node));
    t->byte = byte;
    t->freq = freq;
    t->left = NULL;
    t->right = NULL;
    return t;
}


Vector* Vector_new()
{
    Vector* v = malloc(sizeof(Vector));
    v->sz = 0;
    v->cap = 1000;
    v->arr = malloc(sizeof(Node*)*(v->cap));
    return v;
}

void Vector_free(Vector* v)
{
    for (int i = 0; i < v->sz; i++)
        free(v->arr[i]);
    free(v->arr);
    free(v);
}

void Vector_push(Vector* v, Node* x)
{
    if (v->sz == v->cap)
    {
        v->cap *= 2;
        v->arr = realloc(v->arr, v->cap * sizeof(Node*));
    }
    v->arr[(v->sz)] = x;
    v->sz++;
}

void Vector_pop(Vector* v)
{
    v->sz--;
    v->arr[v->sz] = NULL;
}

void Vector_print(Vector* v)
{
    for (int i = 0; i < v->sz; i++)
    {
        printf("Byte %d freq %d\n", v->arr[i]->byte, v->arr[i]->freq);
    }
}

void Node_swap(Node** l, Node** r)
{
    Node* t = *l;
    *l = *r;
    *r = t;
}

void Node_free(Node* x)
{
    if (x->left == NULL && x->right == NULL)
    {
        free(x);
        return;
    }
    if (x->left) Node_free(x->left);
    if (x->right) Node_free(x->right);
}


void shiftdown(Vector* v, int i)
{
    int l = i*2 + 1, r = i*2+2;
    int min = i;
    if (l < v->sz && v->arr[l]->freq < v->arr[i]->freq)
        min = l;
    if (r < v->sz && v->arr[r]->freq < v->arr[min]->freq)
        min = r;
    if (min != i)
    {
        Node_swap(&(v->arr[i]), &(v->arr[min]));
        shiftdown(v, min);
    }
}

Node* extractmin(Vector* v)
{
    Node* min = v->arr[0];
    Node_swap(&v->arr[0], &v->arr[v->sz-1]);
    Vector_pop(v);
    shiftdown(v, 0);
    return min;
}

void buildheap(Vector* v)
{
    for (int i = (v->sz-1)/2; i >= 0; i--)
        shiftdown(v, i);
}

void Node_insert(Vector* v, Node* x)
{
    Vector_push(v, x);
    int i = v->sz - 1;
    int p = (i-1)/2;
    while (i && v->arr[i]->freq < v->arr[p]->freq)
    {
        v->arr[i] = v->arr[p];
        i = (i-1)/2;
        p = (i-1)/2;
    }
    v->arr[i] = x;
}