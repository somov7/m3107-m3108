#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main( void ) 
{
    struct tm time;

    int* secsArr;
    int win;
    int strcnt = 1, months = 0, number = 0, errorcnt = 0, status;

    char month[4];
    char remote_addr[2048];
    char request[2048];
    char bytes_send[7];
    char local_time[32];

    int n = 0;

    char Mon[12][4] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

    printf("Time interval: \n");
    scanf("%d", &win);

    FILE* File = fopen("C:\\Users\\Егор\\source\\repos\\lab2main\\lab2main\\access_log_Jul95", "r");

    if (File == NULL) 
    {
        printf("%s", "Error file not opened");
        return 1;
    }

    char c = fgetc(File);
    while (c != EOF) 
    {
        if (c == '\n') 
        {
            strcnt++;
        }
        c = fgetc(File);
    }

    secsArr = (int*)malloc(sizeof(int) * strcnt);

    fseek(File, 0, SEEK_SET);

    printf("\nRequests-errors :\n\n");

    while (!feof(File)) 
    {
        fscanf(File, "%s - - [%[^]]] \"%[^\"]\" %d %d", remote_addr, local_time, request, &status, &bytes_send);
        status /= 100;
        if (status == 5) 
        {
            number++;
            printf("%d) %s\n", number, request);
            errorcnt = errorcnt + 1;
        }

        sscanf(local_time, "%d/%3s/%d:%d:%d:%d", &time.tm_mday, month, &time.tm_year, &time.tm_hour, &time.tm_min, &time.tm_sec);
        time.tm_year -= 1900;

        for (int i = 0; i < 12; i++) 
        {
            if (Mon[i] == month) 
            {
                months = i;
                break;
            }
        }

        time.tm_mon = months;
        secsArr[n] = mktime(&time);
        n++;
    }

    int supportArr = 0, cnt = 1, maxr = 0, j = 1, frst, last;
    for (int i = 1; i < strcnt; i++) 
    {
        supportArr += secsArr[i] - secsArr[i - 1];
        cnt++;

        while (supportArr > win && j + 1 < strcnt) 
        {
            supportArr -= (secsArr[j] - secsArr[j - 1]);
            j++;
            cnt--;

        }

        if (cnt > maxr) 
        {
            maxr = cnt;
            frst = i;
            last = j;
        }
    }

    fseek(File, 0, SEEK_SET);

    char start[32], end[32];
    char* buffer;
    buffer = (char*)calloc(4300, sizeof(char));

    for (int i = 0; i < strcnt; i++) 
    {
        fgets(buffer, 4200, File);

        if (i == frst) 
        {
            sscanf(buffer, "%*s - - [%s]", start);
        }
        if (i == last) 
        {
            sscanf(buffer, "%*s - - [%s]", end);
        }
    }

    printf("\nTotal requests-errors: %d\n", errorcnt);
    printf("\nMax requests in %d seconds window: %d \n", win + 1, maxr);
    printf("%s\t to \t%s\n", start, end);

    return 0;
}