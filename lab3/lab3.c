#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
 
int main(){
    struct tm time;
    int* secsArr;
    int window;
    int strcount = 1, months, number = 0, countofErrors = 0, status;
    char month[3];
    char remote_addr[2048];
    char request[2048];
    char bytes_send[7];
    char local_time[32];
    int n = 0;
    char allMonths[12][3] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
 
    printf("Enter desired time interval\n");
    scanf("%lld", &window);
 
    FILE* fin = fopen("access_log_Jul95", "r");
    if (fin == NULL){ 
        printf("%s","no file"); 
        return 1; 
    } 
 
    char c = fgetc(fin);
    while(c != EOF){
        if (c == '\n'){
            strcount++;
        }
        c = fgetc(fin);
    }
 
    secsArr = (int*)malloc(sizeof(int) * strcount);
 
    fseek(fin, 0, SEEK_SET);
 
    printf("\nRequests with server errors :\n\n");
 
    while (!feof(fin)) {
        fscanf(fin,"%s - - [%[^]]] \"%[^\"]\" %d %d", remote_addr, local_time, request, &status, &bytes_send);
        status /= 100;
        if (status == 5){
            number++;
            printf("%d) %s\n",number,request);
            countofErrors = countofErrors + 1;
        }
 
        sscanf(local_time,"%d/%3s/%d:%d:%d:%d",&time.tm_mday,month,&time.tm_year,&time.tm_hour,&time.tm_min,&time.tm_sec);
        time.tm_year -= 1900;
 
        for(int i = 0; i < 12; i++){
            if (allMonths[i] == month){
                months = i + 1;
                break;
            }
        }
 
        time.tm_mon = months;
        secsArr[n] = mktime(&time);
        n++;
    }
 
    int supportArr = 0, counter = 1, maxRequest = 0, j = 1, first, last;
    for (int i = 1; i < strcount; i++) {
        supportArr += secsArr[i] - secsArr[i-1];
        counter++;
        while (supportArr > window && j + 1 < strcount) {
            supportArr -= (secsArr[j] - secsArr[j-1]);
            j++;
            counter--;
 
        }
        if (counter > maxRequest) {
            maxRequest = counter;
            first = i;
            last = j;
        }
    } 
 
    fseek(fin, 0, SEEK_SET);
    char start[32], end[32];
    char* buffer = malloc(4200);
    for(int i = 0; i < strcount; i++){
        fgets(buffer, 4200, fin);
        if (i == first){
            sscanf(buffer, "%*s - - [%s]", start);
        }
        if (i == last){
            sscanf(buffer, "%*s - - [%s]", end);
        }
    }
 
    printf("\nTotal Requests with 5xx(server errors): %d\n", countofErrors);
    printf("\nMax requests in %d seconds window: %d \n", window + 1, maxRequest);
    printf("%s\tto\t%s\n", start, end);
    return 0;
}