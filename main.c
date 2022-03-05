#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_REQUEST_LENGTH 4200

typedef struct {
    char remote_address[2048];
    char request[2048];
    int bytes_send;
    char local_time[32];
    int status;
} Log;


int countSeconds(FILE * input) {
    int strCount = 1;
    char c = fgetc(input);
    while (c != EOF) {
        if (c == '\n') strCount++;
        c = fgetc(input);
    }

    return strCount;
}

int main(int argc, char *argv[]) {
    int *secondsArr;
    int n = 0, months, timeWindow;
    char allMonths[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    printf("Enter interval\n");
    scanf("%d", &timeWindow);

    FILE *input;
    if (argv[1] != NULL) {
        input = fopen(argv[1], "r");
        if (input == NULL) {
            printf("%s\n", "File doesn't exist.");
            return 1;
        }
    } else {
        printf("%s\n", "Few arguments.");
        return 1;
    }

    int strCount = countSeconds(input);
    secondsArr = (int *) malloc(sizeof(int) * strCount);

    fseek(input, 0, SEEK_SET); // сдвигаем указатель в начало файла

    Log log;
    struct tm time;
    char month[4];
    int number = 0, errorsCount = 0;
    while (!feof(input)) {
        fscanf(input, "%s - - [%[^]]] \"%[^\"]\" %d %d", log.remote_address, log.local_time, log.request,
               &log.status, &log.bytes_send);

        log.status /= 100;
        if (log.status == 5) {
            number++;
            printf("%d) %s %d\n", number, log.request, errorsCount);
            errorsCount = errorsCount + 1;
        }

        sscanf(log.local_time, "%d/%3s/%d:%d:%d:%d", &time.tm_mday, month,
               &time.tm_year, &time.tm_hour, &time.tm_min, &time.tm_sec);
        time.tm_year -= 1900; // т.к отсчет начинается с 1900

        for (int i = 0; i < 12; i++) {
            if (allMonths[i] == month) {
                months = i;
                break;
            }
        }

        time.tm_mon = months;
        secondsArr[n] = mktime(&time);
        n++;
    }

    // находим максимальное временное окно
    int temp = 0, counter = 1, maxRequest = 0, j = 1, first, last;
    for (int i = 1; i < strCount; i++) {
        temp += secondsArr[i] - secondsArr[i - 1];
        counter++;
        while (temp > timeWindow && j + 1 < strCount) {
            temp -= (secondsArr[j] - secondsArr[j - 1]);
            j++;
            counter--;
        }
        if (counter > maxRequest) {
            maxRequest = counter;
            first = i;
            last = j;
        }
    }

    fseek(input, 0, SEEK_SET);

    char interval_start[32], interval_end[32];
    char *buf = malloc(MAX_REQUEST_LENGTH);
    for (int i = 0; i < strCount; i++) {
        fgets(buf, MAX_REQUEST_LENGTH, input);
        if (i == first)
            sscanf(buf, "%*s - - [%s]", interval_start);
        if (i == last)
            sscanf(buf, "%*s - - [%s]", interval_end);
    }

    printf("\n5xx errors count %d:\n", errorsCount);
    printf("\nMax requests in %d seconds window: %d \n", timeWindow, maxRequest);
    printf("%s to %s\n", interval_start, interval_end);

    return 0;
}
