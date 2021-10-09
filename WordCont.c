#include <stdio.h>
#include <string.h>
int main(int argc, char *argv[])
{
 FILE *fp;

 char ch;
 int lines, words, bytes, a;
 lines = 0;
 words = 0;
 bytes = 0;
 a = argc;
 
   fp = fopen(argv[1],"r");
   if ( fp )
   {
	   while ((ch=getc(fp)) != EOF) 
       {
            ++bytes;
		   
		   if (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r')
            {
               ++words;
            }
		   if (ch == '\n')
            { 
               ++lines;
               ++bytes;
            }
	   }

	   if (bytes > 0) 
       {
		++lines;
		++words;
	   }
    }
   else
    {
         printf("Failed to open file\n");
    }

    if (a < 3)
 {
    printf("Lines : %d \n", lines);
    printf("Words : %d \n", words);
    printf("Bytes : %d \n", bytes);
    return(0);
 }
    if (strcmp(argv[2], "-l") == 0 || 
        strcmp(argv[2], "--lines") == 0)
    {
        printf("Lines : %d \n", lines);
        return(0);
    }
    if (strcmp(argv[2], "-c") == 0 || 
        strcmp(argv[2], "--bytes") == 0)
    {
        printf("Bytes : %d \n", bytes);
        return(0);
    }
    if (strcmp(argv[2], "-w") == 0 || 
        strcmp(argv[2], "--words") == 0)
    {
        printf("Words : %d \n", words);
        return(0);
    }
  

    
getchar();
return(0);
}