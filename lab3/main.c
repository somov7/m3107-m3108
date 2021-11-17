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
    struct tm time;
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
    int lineNumber = 0;

    FILE* log = fopen(argv[2], "r");
    checkInput(argc, log);

    printf("List of requests ended with error 5xx");
    while (!feof(log)) {
        fscanf(log, "%s - - [%[^]]] \"%[^\"]\" %d %d", &remote_addr, &local_time, &request, &status, &bytes_send);
        if (status / 100 == 5) {
            countErrors++;
            printf("\n%d. %s - error %d", countErrors, request, status);
        }
        sscanf(local_time, "%d/%3s/%d:%d:%d:%d", &time.tm_mday, &month, &time.tm_year, &time.tm_hour, &time.tm_min, &time.tm_sec);
        time.tm_year -= 1900;

        for(int i = 0; i < 12; i++){
            if (strcmp(monthsList[i], month) == 0){
                time.tm_mon = i;
                break;
            }
        }

        if (lineNumber == convertedTimeSize) {
            convertedTimeSize *= 2;
            convertedTime = realloc(convertedTime, sizeof(time_t) * convertedTimeSize);
        }
        convertedTime[lineNumber] = mktime(&time);
        lineNumber += 1;
    }

    printf("\n\nThere are %d 5xx errors in %s", countErrors, argv[2]);
}
