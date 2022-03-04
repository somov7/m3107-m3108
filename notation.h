#include <stdio.h>
#include "str.h"
char *get_10_2 (int data, int size, int iterator, char *arr)
{
    iterator++;
    for (int i=size*iterator-1; i>=size*iterator-size; i--)
    {
        arr[i]=data%2+'0';
        data/=2;
    }
    return arr;
}
int get_2_10 (char* arr)
{
    int data=0;
    int j=1;
    for (int i = strlen(arr)-1; i>=0; i--)
    {
        if(arr[i]=='1')
            data+=j;
        j*=2;
    }
    return data;
}

