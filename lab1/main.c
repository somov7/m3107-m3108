

#include "stdio.h"

#include "stdlib.h"

#include "string.h"



int CharCount();  

int WordCount();  

int LineCount(); 



int main(int argc,char *argv[])

{
    if (argv[1] == NULL || argv[2] == NULL)
    {
         printf("error, uncorrect arguments.");
    }
    else if ((strcmp(argv[1], "-c") == 0) && (strcmp(argv[2], "text.in") == 0))

    {

        ;
        printf("char count is :%d\n",CharCount());
    }

    

    else if ((strcmp(argv[1], "-w") == 0) && (strcmp(argv[2], "text.in") == 0))

    {

       
         printf("word count is :%d\n",WordCount());
    }

    else if ((strcmp(argv[1], "-l") == 0) && (strcmp(argv[2], "text.in") == 0))

    {

        
        printf("line count is:%d\n",LineCount());

    }

    return 0;





}



int CharCount() 

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
    return c;
    

    fclose(fp);

};

int WordCount() 

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

        if ((ch >= 'a'&&ch <= 'z')||(ch >= 'A'&&ch <= 'Z')||(ch >= '0'&&ch <= '9')||ch == '_')

        {

            while ((ch >= 'a'&&ch <= 'z')||(ch >= 'A'&&ch <= 'Z')||(ch >= '0'&&ch <= '9')||ch == '_')

            {

                ch = fgetc(fp);

            }
            w ++;
        }

             ch = fgetc(fp);

    }
    return w;
   

    fclose(fp);

}

int LineCount() 

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

        }

       
        ch = fgetc(fp);
    }
    return l;
    

    fclose(fp);

}