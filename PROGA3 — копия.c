 #include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
int main(int argc,char **argv)
{
    FILE *fin,*ffin;
    const struct tm *time;
    const struct tm *time2;
    int month;
    int Status;
    char remote[2048];
    char loctime[32];
    char request[2048];
    char bytes[18];
    char mouth [3];
    fin = fopen(argv[1],"r");
    ffin = fopen (argv[1],"r");
    long long req = atoi(argv[2]);
    if ((argc!=3)||(argv[2]==NULL)||(argv[1]==NULL)){
     fprintf(stderr,"Check input values\n");
     return 3;
   }
    if (fin==NULL || ffin==NULL){
        printf ("ERORR OPENNING");
        return 1;
    }
    struct tm datetm;
    char str[4200];
    char mouths[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    int *Secs;
    int Secs2;
    long long quamstr = 0;
    char symbol;
    long long j =0 ;
    while(fgets(str,4096,ffin)!=NULL) {
    quamstr++;
  }
  fclose(ffin);
  //printf("%lld",quamstr);
  int str_5xx=0;
Secs = (int*)malloc(sizeof(int) * quamstr);
while(!feof(fin)){
     fscanf(fin,"%s - - [%[^]]] \"%[^\"]\" %d %d", remote, loctime, request, &Status, &bytes);
if(Status/100==5){
    str_5xx++;
}
sscanf(loctime,"%d/%3s/%d:%d:%d:%d",&datetm.tm_mday,mouth,&datetm.tm_year,&datetm.tm_hour,&datetm.tm_min,&datetm.tm_sec);
datetm.tm_year-=1900;
for(int i = 0;i<12;i++){
    if(strcmp(mouth,&mouths[i])){
            month=i+1;
            break;
    }
}
datetm.tm_mon=month;
    Secs[j] = mktime(&datetm);
    j+=1;}
   long long pr,coun,Maximum,l,k,left,right;
   coun =1;
   l=1;
   Maximum=0;
   pr = 0;
   for (k = 1; k<quamstr; k++) {
     pr += Secs[k] - Secs[k-1];
     coun++;
     while (pr > req && l+1 < quamstr) {
       pr -= Secs[l] - Secs[l-1];
       l++;
       coun--;
     }
     if(Maximum<coun){
         Maximum = coun;
         right= Secs[k];
         left = Secs[l];
         
        
     }
     
   }
   printf("reqs with error 5xx : %d\n",str_5xx);
   printf("Maximum requests in %lld time window : %lld , Time window: from %d to %d\n",req,Maximum,left,right);
    return 0;
}