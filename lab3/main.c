#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

double *arr;
long long arr_size = 1;

int main(int argc, char *argv[])
{
    struct tm zeroTime;
    time_t zero;
    zeroTime.tm_year = 71;
    zeroTime.tm_mday = 1;
    zeroTime.tm_mon = 1;

    zero = mktime(&zeroTime);

    FILE *fp;
    if (argc == 3)
    {
        if (!(fp = fopen(argv[1], "r")))
        {
            printf("Error file doesn`t exist");
            return 0;
        }
    }
    else
    {
        printf("Error");
        return 0;
    }

    char log[500];
    long long counter = 0;
    long long count = 0;
    int time_param = atoi(argv[2]);
    char *months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    arr = calloc(arr_size, sizeof(double));

    while (fscanf(fp, "%[^\n]s", log) != EOF)
    {
        int i = strchr(log, '[') - log + 1;
        char log_day[4] = "";
        memcpy(log_day, &log[i], 2);

        i = strchr(log, '/') - log + 1;
        char log_month[6] = "";
        memcpy(log_month, &log[i], 3);

        char log_year[8] = "";
        memcpy(log_year, &log[i + 4], 4);

        char log_hours[4] = "";
        memcpy(log_hours, &log[i + 9], 2);

        char log_minutes[4] = "";
        memcpy(log_minutes, &log[i + 12], 2);

        char log_seconds[4] = "";
        memcpy(log_seconds, &log[i + 15], 2);

        struct tm logTime = {0};
        time_t something;
        logTime.tm_mday = atoi(log_day);
        logTime.tm_sec = atoi(log_seconds);
        logTime.tm_min = atoi(log_minutes);
        logTime.tm_hour = atoi(log_hours);
        logTime.tm_year = atoi(log_year) - 1900;
        logTime.tm_isdst = -1;
        for (int j = 0; j < 12; j++)
        {
            if (strcmp(months[j], log_month) == 0)
            {
                logTime.tm_mon = j;
            }
        }

        something = mktime(&logTime);

        double smth = difftime(something, zero);

        arr[count] = smth;
        count++;
        arr_size++;
        arr = realloc(arr, arr_size * sizeof(double));

        int c = 0;

        for (int i = 0; i < strlen(log); i++)
        {
            if (log[i] == '"')
            {
                c += 1;
                if (c == 2 && log[i + 2] == '5')
                {
                    counter++;
                    printf("%d. %s\n", counter, log);
                    break;
                }
            }
        }

        fgetc(fp);
        fscanf(fp, "%[^\n]s", log);
    }
    printf("There are %d 5xx logs\n", counter);

    int c = 0;
    int max_c = 0;
    for (int i = 0; i < arr_size - 1; i++)
    {
        int j = i;
        while ((arr[j] - arr[i] <= time_param) && (j < arr_size - 1))
        {
            j++;
            c++;
            if (c > max_c)
            {
                max_c = c;
            }
        }
        c = 0;
    }
    printf("max logs in time %d sec: %d",time_param, max_c);

    return 0;
}