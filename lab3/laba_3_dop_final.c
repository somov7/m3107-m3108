#include <stdio.h>
#include <time.h>
#include <string.h>
#include <malloc.h>

int month_(char month[4])
{
    int m;
    if (strcmp(month, "Jan") == 0)
        m = 0;
    if (strcmp(month, "Feb") == 0)
        m = 1;
    if (strcmp(month, "Mar") == 0)
        m = 2;
    if (strcmp(month, "Apr") == 0)
        m = 3;
    if (strcmp(month, "May") == 0)
        m = 4;
    if (strcmp(month, "Jun") == 0)
        m = 5;
    if (strcmp(month, "Jul") == 0)
        m = 6;
    if (strcmp(month, "Aug") == 0)
        m = 7;
    if (strcmp(month, "Sep") == 0)
        m = 8;
    if (strcmp(month, "Oct") == 0)
        m = 9;
    if (strcmp(month, "Nov") == 0)
        m = 10;
    if (strcmp(month, "Dec") == 0)
        m = 11;
    return m;
}

long long time_(FILE *file)
{
    struct tm *time;
    char sim = fgetc(file);
    while ((sim != '[') && (sim != EOF))
        sim = fgetc(file);
    time->tm_mday = (fgetc(file) - 48) * 10 + fgetc(file) - 48;
    sim = fgetc(file);
    char month[4];
    month[0] = fgetc(file);
    month[1] = fgetc(file);
    month[2] = fgetc(file);
    month[3] = '\0';
    time->tm_mon = month_(month);
    sim = fgetc(file);
    time->tm_year = (fgetc(file) - 48) * 1000 + (fgetc(file) - 48) * 100 + (fgetc(file) - 48) * 10 + (fgetc(file) - 48) - 1900;
    sim = fgetc(file);
    time->tm_hour = (fgetc(file) - 48) * 10 + (fgetc(file) - 48);
    sim = fgetc(file);
    time->tm_min = (fgetc(file) - 48) * 10 + (fgetc(file) - 48);
    sim = fgetc(file);
    time->tm_sec = (fgetc(file) - 48) * 10 + (fgetc(file) - 48);
    sim = fgetc(file);
    // long long mk_time = mktime(time);
    return mktime(time);
}

int main(int argc, char **argv)
{
    struct tm *time = (struct tm *)malloc(sizeof(struct tm));
    char request[1000];
    long long counter = 0;
    // FILE *file1 = fopen("NASA_access_log_Jul95", "r");
    // FILE *file2 = fopen("NASA_access_log_Jul95", "r");
    // int second = 10;

    FILE *file1 = fopen(argv[1], "r");
    FILE *file2 = fopen(argv[1], "r");
    int second = atoi(argv[2]);

    char sim;
    sim = fgetc(file1);
    long long count = 0, time_l = 0, time_r = 0, max_time_l = 0, max_time_r = 0, max_count = 0;
    while (sim != EOF)
    {
        if (count == 0)
        {
            time_l = time_(file2);
            max_time_l = time_l;
        }
        time_r = time_(file1);
        if (time_r - time_l > second)
        {
            if (count > max_count)
            {
                max_count = count;
                max_time_l = time_l;
            }
            time_l = time_(file2);
        }
        else
            count++;

        while ((sim != '"') && (sim != EOF))
            sim = fgetc(file1);

        if (sim == '\"')
        {
            sim = fgetc(file1);
            long long i = 0;
            while (sim != '\"')
            {
                request[i] = sim;
                i++;
                sim = fgetc(file1);
            }
            sim = fgetc(file1);
            sim = fgetc(file1);
            //если нужно считать, что ошибки это все кроме 1 и 2
            /*if ((sim != '1') && (sim != '2'))
            {
            counter++;
            */
            if (sim == '5')
            {
                counter++;
                int j;
                for (j = 0; j < i; j++)
                    printf("%c", request[j]);
                printf("\n");
            }
        }

        if ((time_r < max_time_l + second) && (time_l == max_time_l))
        {
            max_count = count;
            max_time_l = time_l;
        }
        while ((sim != '\n') && (sim != EOF))
            sim = fgetc(file1);
    }
    /*
129.130.115.19 - - [27/Jul/1995:11:44:31 -0400] "POST /shuttle/missions/sts-69/mission-sts-69.html HTTP/1.0" 501 -
surg1.mayo.edu - - [27/Jul/1995:11:44:33 -0400] "GET /shuttle/missions/51-l/docs/ HTTP/1.0" 200 368
surg1.mayo.edu - - [27/Jul/1995:11:44:36 -0400] "GET /shuttle/missions/51-l/ HTTP/1.0" 200 1712
malin.space.se - - [27/Jul/1995:11:44:37 -0400] "GET /persons/nasa-cm/jmd.html HTTP/1.0" 200 3933
news.ti.com - - [27/Jul/1995:11:44:39 -0400] "GET /history/apollo/images/apollo-logo1.gif HTTP/1.0" 200 1173
    */
    max_time_r = max_time_l + second;
    printf("Ошибок с номером \"5\" %lli\n", counter);
    printf("Максимально запросов в промежутке %d секунд:", second);
    printf("%lli \n", max_count);
    time = localtime(&max_time_l);
    printf("с:\n %s", asctime(time));
    time = localtime(&max_time_r);
    printf("по:\n %s", asctime(time));
    return 0;
}
