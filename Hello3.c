#include <stdio.h> 
#include <ctype.h> 
#include <string.h> 
int main (int argc, char **argv) 
{ 
  const char symbol = '\n'; 
  FILE *File; 
  int sizeb=0; 
  int line; 
  int CountWord; 
  int linecnt = 1; 
  int pr;
  int cntcol; 
  File = fopen(argv[2], "r");
  if ((argc!=3)||(argv[2]==NULL)||(argv[1]==NULL)||(argv[0]==NULL)){
    fprintf(stderr,"Check input values\n");
    return 3;
  }
  while ((cntcol=fgetc(File)) != EOF) 
  { 
        sizeb++;
    } 
   
  fclose(File); 
  File = fopen(argv[2], "r");
  int kol = 1; 
   int Wordcount= 0; 
  char wrds[2] = {0}; 
  while (! feof(File)) 
  { 
        fgets(wrds, 2, File); 
        if (wrds[0] > 41) 
            kol= 0; 
        else if (kol == 0) 
        { 
            Wordcount++; 
            kol= 1; 
        } 
        wrds[0] = 0; 
    } 
  fclose(File); 
  File = fopen(argv[2], "r"); 
  while ((pr=fgetc(File)) != EOF) 
  { 
    if (pr == symbol) 
            linecnt++; 
  } 
  if ((strcmp(argv[1], "-l")==0) || (strcmp(argv[1], "--lines")==0)) 
{ 
    printf("%d",linecnt); 
  } 
  else if (strcmp(argv[1], "-w")==0 || (strcmp(argv[1], "--words")==0)) 
{ 
    printf("%d", Wordcount); 
  } 
  else if (strcmp(argv[1], "-c")==0 || (strcmp(argv[1], "--bytes")==0)) 
{ 
    printf("%d", sizeb); 
  } 
  return 0; 
}