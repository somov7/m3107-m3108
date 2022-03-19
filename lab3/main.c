#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

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
    int data_weight;
    int ErrorCount = 0;
    char month[3];
    char months[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Dec"};

    long long window;

    FILE *foe = fopen(filename, "r");

    if (!foe) {

        printf("%s", "File did not open.");

        return -1;
    }

    while (fgets(entire_line, 4150, foe) != NULL) line_count++;

    printf("Enter time window\n");
    scanf("%lld", &window);

    printf("Counting requests with 5xx error (server errors)...\n\n");

    int line = 0;

    line_time = malloc(sizeof(long) * line_count);

    fseek(foe, 0, SEEK_SET);

    while (!feof(foe)) {

        fscanf(foe, "%s - - [%[^]]] \"%[^\"]\" %d %d", ref, time_of_request, data, &result, &data_weight);

        if ((result / 100) == 5) {

            ErrorCount++;

            printf("%d)  %s\n", ErrorCount, data);
        }

        sscanf(time_of_request,"%d/%3s/%d:%d:%d:%d", &time.tm_mday, month, &time.tm_year, &time.tm_hour, &time.tm_min, &time.tm_sec);

        short cur_month;

        for (cur_month = 0; cur_month < 12; cur_month++) {
            if (!strcmp(months[cur_month], month)) break;
        }

        time.tm_mon = cur_month;
        time.tm_year -= 1900;

        line_time[line] = (long)mktime(&time);

        line++;
    }

    int difference = 0, current_length = 1, maximum = 0, position = 1, left_bound_final, right_bound_final;

    for (int cur_line = 1; cur_line < line_count; cur_line++) {

        current_length++;
        difference += line_time[cur_line] - line_time[cur_line - 1];

        while ((difference > window) && (position + 1 < line_count)) {
            difference -= (line_time[position] - line_time[position - 1]);
            position++;
            current_length--;
        }

        if (current_length > maximum) {
            maximum = current_length;
            left_bound_final = position;
            right_bound_final = cur_line;
        }
    }

    struct tm *time_start;
    struct tm *time_end;

    printf("\nQuantity of requests with 5xx error (server errors): %d\n", ErrorCount);

    time_t left_bound_final_ll = line_time[left_bound_final];
    time_start = localtime(&left_bound_final_ll);

    printf("\nMaximum quantity of requests in %lld second(s) window: %d\nFrom %s", window, maximum, asctime(time_start));

    time_t right_bound_final_ll = line_time[right_bound_final];
    time_end = localtime(&right_bound_final_ll);

    printf("  to %s", asctime(time_end));

    fclose(foe);
    return 0;
}