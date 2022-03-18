#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

int cntr;

typedef struct
{
    char remote_addr[32];
    char local_time[32];
    char request[350];
    char status[32];
    char bytes_send[32];
    char trash[32];
} nasa_log;

void parse_string(char string[], int parameter)
{
    time_t rawtime;
    char mnth[5];
    struct tm timeinfo;
    int timer, month = 0, i, counter, status;
    nasa_log result;
    char *token;
    char * months[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
                        "Aug", "Sep", "Oct", "Nov", "Dec" };

    token = strtok(string, "[");
    strcpy(result.remote_addr, token);
    token = strtok(NULL, " ]");
    strcpy(result.local_time, token);
    token = strtok(NULL, "\"");
    strcpy(result.trash, token);
    token = strtok(NULL, "\"");
    strcpy(result.request, token);
    token = strtok(NULL, " ");
    strcpy(result.status, token);
    status = atoi(result.status);
    token = strtok(NULL, "");
    strcpy(result.bytes_send, token);



    if (status >= 500)
    {
        cntr += 1;
        printf("%s %s UTC -4hrs; %s; Error: %s; %s", result.remote_addr, result.local_time,
               result.request,result.status, result.bytes_send);
        printf("Errors: %d\n", cntr);
    }

    sscanf(result.local_time, "%d/%3s/%d:%d:%d:%d", &timeinfo.tm_mday, mnth, &timeinfo.tm_year,
           &timeinfo.tm_hour, &timeinfo.tm_min, &timeinfo.tm_sec);

    for (i = 0; i < 12; i++) // 12 = number of months
    {
        if (strcmp(mnth, months[i]) == 0)
        {
            month = i + 1;
            break;
        }
    }
    timeinfo.tm_mon = month;



    timeinfo.tm_year -= 1900;
    rawtime = mktime(&timeinfo);
    printf("%lld %d/%d/%d:%d:%d:%d\n", rawtime, timeinfo.tm_mday, timeinfo.tm_mon, timeinfo.tm_year,
           timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

}

int main(int argc, char *argv[])
{
    int seconds;
    scanf("%d", &seconds);
    char string[500];
    FILE *Lab3;
    if (argv[1] != NULL) {
        Lab3 = fopen(argv[1], "r");
        if (Lab3 == NULL) {
            fprintf(stderr, "%s\n", "File doesn't exist.");
            return 1;
        }
    }
    if (argv[1] == NULL) {
        fprintf(stderr, "%s\n", "Few arguments.");
        return 1;
    }

    while (!feof(Lab3))
    {
        parse_string(fgets(string, 500, Lab3), seconds);
    }

    return 0;
}