
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

void CharCount();  
void WordCount();  
void LineCount();  
void Muiltiple();  

int main(int argc,char *argv[])
{
    if ((strcmp(argv[1], "-c") == 0) && (strcmp(argv[2], "text.in") == 0))
    {
        CharCount();
    }
    
    if ((strcmp(argv[1], "-w") == 0) && (strcmp(argv[2], "text.in") == 0))
    {
        WordCount();
    }
    if ((strcmp(argv[1], "-l") == 0) && (strcmp(argv[2], "text.in") == 0))
    {
        LineCount();
    }
    return 0;


}

void CharCount() 
{
    FILE *fp;
    int c = 0;
    char ch;
    if((fp = fopen("text.in","r")) == NULL)
    {
        printf("file read failure.");
    }
    ch = fgetc(fp);
    while(ch != EOF)
    {
            c ++;
            ch = fgetc(fp);
    }
    printf("char count is :%d\n",c);
    fclose(fp);
};
void WordCount() 
{
    FILE *fp;
    int w = 0;
    char ch;
    if((fp = fopen("text.in","r")) == NULL)
    {
        printf("file read failure.");
    }
    ch = fgetc(fp);
    while(ch != EOF)
    {
        if ((ch >= 'a'&&ch <= 'z')||(ch >= 'A'&&ch <='Z')||(ch >= '0'&&ch <= '9'))
        {
            while ((ch >= 'a'&&ch <= 'z')||(ch >= 'A'&&ch <= 'Z')||(ch >= '0'&&ch <= '9')||ch == '_')
            {
                ch = fgetc(fp);
            }
            w ++;
            ch = fgetc(fp);    
        }
        else 
        {
            ch = fgetc(fp);
        }
    }
    printf("word count is :%d\n",w);
    fclose(fp);
}
void LineCount() 
{
    FILE *fp;
    int l = 1;
    char ch;
    if((fp = fopen("text.in","r")) == NULL)
    {
        printf("file read failure.");
    }
    ch = fgetc(fp);
    while(ch != EOF)
    {
        if (ch == '\n')
        {
            l ++;
            ch = fgetc(fp);
        }
        else
        {
            ch = fgetc(fp);
        }
    }
    printf("line count is:%d\n",l);
    fclose(fp);
}