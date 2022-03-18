#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "utils.h"

long long getErrors(FILE *filePointer, char **errors) {
    char *line = NULL;
    long long count = 0;
    char *request = malloc(1000 * sizeof(char *));

    size_t size = 0;

    while ((size = getline(&line, &size, filePointer)) != -1) {
        char *ptr = strtok(line, "\"");

        int part = 0;

        while (ptr != NULL) {
            part++;

            if (part == 2) {
                request = ptr;
            }

            if (part == 3 && ptr[1] == '5') {
                strcpy(errors[count], request);
                count++;
            }
            ptr = strtok(NULL, "\"");
        }
    }

    return count;
}

long long toSeconds(char *str) {
    const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    struct tm time = {
            .tm_year = atoi(str + 7) - 1900,
            .tm_mday = atoi(str),
            .tm_hour = atoi(str + 12),
            .tm_min = atoi(str + 15),
            .tm_sec = atoi(str + 18)
    };

    str[6] = '\0';

    int month;

    for (month = 0; month < 12; month++) {
        if (!strcmp(str + 3, months[month])) {
            break;
        }
    }

    time.tm_mon = month;

    return mktime(&time);
}

void printMaxRequests(FILE *filePointer, long long seconds) {
    char *line = NULL;
    size_t size = 0;

    long long maxStartIndex, maxEndIndex, startTime, endTime, startIndex, endIndex = 1;

    long long count = 0;
    long long maxRequests = 0;
    long long requests = 1;

    long long timesSize = 1000;

    long long *times = malloc(timesSize * sizeof(long long));

    while ((size = getline(&line, &size, filePointer)) != -1) {
        char *ptr = strtok(line, "[");

        int part = 0;

        while (ptr != NULL) {
            part++;

            if (part == 2) {
                if (count >= timesSize) {
                    times = realloc(times, sizeof(long long) * timesSize * 2);
                    timesSize *= 2;
                }
                times[count] = toSeconds(ptr);
            }

            ptr = strtok(NULL, "]");
        }

        if (count > 0) {
            endIndex = count;

            for (long long i = count - 1; i >= 0; i--) {
                if (times[count] - times[i] <= seconds && times[count] - times[i] >= 0) {
                    requests += 1;
                    startIndex = i;
                } else {
                    break;
                }
            }
        }

        if (requests > maxRequests) {
            maxRequests = requests;

            maxStartIndex = startIndex;
            maxEndIndex = endIndex;

            startTime = times[maxStartIndex];
            endTime = times[maxEndIndex];
        }

        requests = 1;

        count++;
    }

    free(times);

    printf("Max lines in %lld seconds window: %lld\n", seconds, maxRequests);
    printf("Seconds: %lld - %lld\n", startTime, endTime);
    printf("Lines: %lld - %lld\n", maxStartIndex, maxEndIndex);
}