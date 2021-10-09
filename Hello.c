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
  if (File==NULL) {
      printf("ERROR:NO FILE\n");
  }
  
  if ((strcmp(argv[1], "-l")==0) || (strcmp(argv[1], "--lines")==0)) 
{ 
    while ((pr=fgetc(File)) != EOF) 
  { 
    if (pr == symbol) 
            linecnt++; 
  } 
    printf("%d\n",linecnt); 
    fclose(File);
  } 
  else if (strcmp(argv[1], "-w")==0 || (strcmp(argv[1], "--words")==0)) 
{ 
  int kol = 1; 
   int Wordcount= 0; 
  char wrds[2] = {0}; 
  while ((cntcol=fgetc(File)) != EOF) 
  { 
        fgets(wrds, 1, File); 
        if (wrds[0] > 32) 
            kol= 0; 
        else if (kol == 0) 
        { 
            Wordcount++; 
            kol= 1; 
        } 
        wrds[0] = 0; 
    } 
  fclose(File); 
    printf("%d\n", Wordcount); 
  } 
  else if (strcmp(argv[1], "-c")==0 || (strcmp(argv[1], "--bytes")==0)) 
{ 
    if ((argc!=3)||(argv[2]==NULL)||(argv[1]==NULL)){
    fprintf(stderr,"Check input values\n");
    return 3;
  }
  while ((cntcol=fgetc(File)) != EOF) 
  { 
    sizeb++;
    } 
    printf("%d\n", sizeb);
    fclose(File);
  }
  else 
  {
      printf("ERROR: NO INPUT VALUES\n");
  } 
  return 0; 
}