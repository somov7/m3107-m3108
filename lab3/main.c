#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <conio.h>

int main()
{

    struct tm time;

    char *filename = "access_log_Jul95";

    long *line_time;

    char entire_line[4150];
    int line_count = 0;

    char ref[2049];
    char time_of_request[27];
    char data[2049];
    int result;
    char data_weight[4];
    int ErrorCount = 0;
    char month[3];
    char months[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Dec"};

    long window;

    FILE *foe = fopen(filename, "r");

    if (!foe) {

        printf("%s", "File did not open.");

        return 1;
    }

    while (fgets(entire_line, 4150, foe) != NULL) line_count++;

    //printf("line_count = %d\n", line_count);

    printf("Enter time window\n");
    scanf("%lld", &window);

    printf("Counting requests with 5xx error (server errors)...\n\n");

    int line = 0;

    line_time = malloc(sizeof(long) * line_count);

    //for (int i = 0; i < line_count; i++) line_time[line_count] = 0;

    fseek(foe, 0, SEEK_SET);

    while (!feof(foe)) {

        fscanf(foe, "%s - - [%[^]]] \"%[^\"]\" %d %d", ref, time_of_request, data, &result, &data_weight);

        if ((result / 100) == 5) {

            ErrorCount++;

            printf("%d)  %s\n", ErrorCount, data);
        }

        sscanf(time_of_request,"%d/%3s/%d:%d:%d:%d", &time.tm_mday, month, &time.tm_year, &time.tm_hour, &time.tm_min, &time.tm_sec);

        //printf("%d/%3s/%d:%d:%d:%d\n", time.tm_mday, month, time.tm_year, time.tm_hour, time.tm_min, time.tm_sec);
        //getch();

        time.tm_year -= 1900;

        int cur_month = 0;

        for (cur_month = 0; cur_month < 12; cur_month++) {
            if (!strcmp(months[cur_month], month)) break;
        }

        time.tm_mon = cur_month;

        //printf("%d/%3s/%d:%d:%d:%d\n", time.tm_mday, month, time.tm_year, time.tm_hour, time.tm_min, time.tm_sec);
        //getch();

        line_time[line] = (long)mktime(&time);

        //printf("%lld\n%lld\n", mktime(&time), line_time[line]);
        //getch();

        line++;
    }

    long long left_bound = 0, right_bound = 0, maximum = 0, current_length = 0, left_bound_final = 0, right_bound_final = 0;

    while (1) {
        if (line_time[right_bound] - line_time[left_bound] > window) {
            left_bound++;
            current_length = 0;
        } else {
            current_length++;

            if (current_length > maximum) {
                left_bound_final = left_bound;
                right_bound_final = right_bound;
                maximum = current_length;
            }

            right_bound++;
        }
        if (right_bound > line_count) break;
    }

    struct tm *time_start;
    struct tm *time_end;

    time_t left_bound_final_ll = line_time[left_bound_final];
    time_t right_bound_final_ll = line_time[right_bound_final];

    time_start = localtime (&left_bound_final_ll);
    time_end = localtime (&right_bound_final_ll);

    printf("\nQuantity of requests with 5xx error (server errors): %d\n", ErrorCount);
    printf("\nMaximum quantity of requests in %ld second(s) window: %lld\nFrom %s  to %s", window, maximum, asctime(time_start), asctime(time_end));

    fclose(foe);
    return 0;
}