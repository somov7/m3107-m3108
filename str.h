#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
char *merge (char *arr1, char *arr2)
{
    int size_str=strlen(arr1)+strlen(arr2);
    char *str=(char*)malloc(size_str+1);
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
    if (data==0)
        size_str++;
    bool znak=0;
    if(data<0)
        znak=1;
    size_str+=znak;

    char *str=(char*)malloc(size_str+1);
    int j=1;
    for (int i=size_str-1; i>=znak; i--)
    {
        int ch=data%10;
        if (ch<0)
            ch*=-1;
        str[i]=ch+'0';
        data/=10;
    }
    if (znak==1)
        str[0]='-';
    str[size_str]=0;
    return str;
}
int get_int (char *str)
{
    int data=0;
    bool k;
    if (str[0]=='-')
        k=1;
    else
        k=0;
    for(int i=k; i<strlen(str); i++)
        data=data*10+str[i]-'0';
    if (k==1)
        data*=-1;
    return data;
}
