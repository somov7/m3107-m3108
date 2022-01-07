#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void checkInput(unsigned short argc, FILE *file) {
    if (argc != 3) {
        fprintf(stderr, "Two arguments expected!");
        exit(1);
    }
    if (file == NULL) {
        fprintf(stderr, "Error opening file!");
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    struct tm requestTime;
    char monthsList[][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    int countErrors = 0;
    char month[4];
    char remote_addr[2048];
    char local_time[32];
    char request[2048];
    int status;
    int bytes_send;
    int convertedTimeSize = 2048;
    time_t* convertedTime = (time_t*)malloc(sizeof(time_t) * convertedTimeSize);
    int lineCount = 0;
    int timeWindow = atoi(argv[1]);

    FILE* log = fopen(argv[2], "r");
    checkInput(argc, log);

    printf("List of requests ended with error 5xx");
    while (!feof(log)) {
        fscanf(log, "%s - - [%[^]]] \"%[^\"]\" %d %d", &remote_addr, &local_time, &request, &status, &bytes_send);
        if (status / 100 == 5) {
            countErrors++;
            printf("\n%d. %s - error %d", countErrors, request, status);
        }
        sscanf(local_time, "%d/%3s/%d:%d:%d:%d", &requestTime.tm_mday, &month, &requestTime.tm_year, &requestTime.tm_hour, &requestTime.tm_min, &requestTime.tm_sec);
        requestTime.tm_year -= 1900;

        for(int i = 0; i < 12; i++){
            if (strcmp(monthsList[i], month) == 0){
                requestTime.tm_mon = i;
                break;
            }
        }

        if (lineCount == convertedTimeSize) {
            convertedTimeSize *= 2;
            convertedTime = realloc(convertedTime, sizeof(time_t) * convertedTimeSize);
        }
        convertedTime[lineCount] = mktime(&requestTime);
        lineCount += 1;
    }

    time_t supportArr = 0, counter = 1, maxRequest = 0, j = 1, first, last;
    for (int i = 1; i < lineCount; i++) {
        supportArr += convertedTime[i] - convertedTime[i-1];
        counter++;
        while (supportArr > timeWindow && j + 1 < lineCount) {
            supportArr -= (convertedTime[j] - convertedTime[j-1]);
            j++;
            counter--;

        }
        if (counter > maxRequest) {
            maxRequest = counter;
            first = i;
            last = j;
        }
    }


    char startTime[32] = {0};
    char endTime[32] = {0};
    ctime_s(startTime, sizeof startTime, &convertedTime[first]);
    ctime_s(endTime, sizeof endTime, &convertedTime[last]);
    free(convertedTime);
    printf("\n\nThere are %d 5xx errors in %s", countErrors, argv[2]);
    printf("\nThere are %d requests max in %d seconds, since %.24s to %.24s", maxRequest, timeWindow + 1, endTime, startTime);
}
