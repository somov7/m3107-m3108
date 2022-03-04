#include <stdlib.h>
#include <string.h>
#include <stdio.h>
char *merge (char *arr1, char *arr2)
{
    int size_str=strlen(arr1)+strlen(arr2);
    char *str=malloc(size_str+1);
    for (int i=0; i<size_str; i++)
        if (i<strlen(arr1))
            str[i]=arr1[i];
        else
            str[i]=arr2[i-strlen(arr1)];
    str[size_str]=0;
    return str;
}
char *get_str (int data)
{
    int size_str=0;
    int data_clone=data;
    while (data_clone!=0)
    {
        size_str++;
        data_clone/=10;
    }
    char *str=malloc(size_str+1);
    int j=1;
    for (int i=size_str-1; i>=0; i--)
    {
        str[i]=data%10+'0';
        data/=10;
    }
    str[size_str]=0;
    return str;
}
