#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "listSL.h"
#include <iso646.h>


const int maxReqLen = 4096;
const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

typedef struct data {
    char *remoteAddr;
    char *localTime;
    char *timeZone;
    char *request;
    char *status;
    char *bytesSend;
} data;

void printUsage(char err[]) {
    printf("An error has occurred: %s\n", err);
    printf("Command template: -program.exe access_log_file time_window");
}

data getData(char *request) {
    data res;

    res.remoteAddr = strtok(request, "-");

    strtok(NULL, "[");
    res.localTime = strtok(NULL, " ");

    res.timeZone = strtok(NULL, "]");

    strtok(NULL, "\"");
    res.request = strtok(NULL, "\"");

    res.status = strtok(NULL, " ");
    res.bytesSend = strtok(NULL, "\n");

    return res;

}

int getMonthNumber(char *mon) {
    int res;
    for (res = 0; res < 12 and strcmp(mon, months[res]); res++);
    return res + 1;
}

int getTime(char *reqTime) {
    int res;
    struct tm localTime = {};

    localTime.tm_mday = atoi(strtok(reqTime, "/"));
    localTime.tm_mon = getMonthNumber(strtok(NULL, "/"));
    localTime.tm_year = atoi(strtok(NULL, ":")) - 1900; // years since 1900
    localTime.tm_hour = atoi(strtok(NULL, ":"));
    localTime.tm_min = atoi(strtok(NULL, ":"));
    localTime.tm_sec = atoi(strtok(NULL, ":"));

    res = mktime(&localTime);

    return res;
}

void timeWindowSearch(long long **res, long long ReqCnt, int tmWindow, int *tmReq) {

    long long mxLen = 0, j = 1;

    //for (long long i = 0; i < ReqCnt; i++)
    //    printf("%d\n", (tmReq)[i]);
    for (long long i = 0; i < ReqCnt; i++) {
        while (j < ReqCnt and tmReq[j] - tmReq[i] <= tmWindow) {
            j++;
        }
        if (tmReq[j] - tmReq[i] > tmWindow) {
            if (j - i > mxLen) {
                mxLen = j - i;
                (*res)[0] = tmReq[i];
                (*res)[1] = tmReq[j - 1];
                (*res)[2] = mxLen;
            }
        } else {
            if (j - i > mxLen) {
                mxLen = j - i;
                (*res)[0] = tmReq[i];
                (*res)[1] = tmReq[j - 1];
                (*res)[2] = mxLen;
            }
            break;
        }
    }

}

int dataCorrect(data curData) {
    return !(curData.localTime == NULL or curData.request == NULL or curData.status == NULL or
             curData.timeZone == NULL or curData.remoteAddr == NULL or curData.bytesSend == NULL);
}

int main(int argc, char *argv[]) {

    FILE *logFile = fopen(argv[1], "r");
    if (argc < 3) {
        printUsage("invalid value of arguments");
        return 1;
    }

    if (logFile == NULL) {
        printUsage("Incorrect file name");
        return 2;
    }

    if (argv[2] < 0) {
        printUsage("Time window must be more then 0");
        return 3;
    }

    node *requests = NULL;
    int timeWindow = atoi(argv[2]);
    long long linesLimit = 128; // Будем динамически выделять память, увеличивая её в два раза при переполнении. работает как ветор в c++
    long long stringCnt = 0;
    long long ErrCnt = 0;
    char *curReq = calloc(maxReqLen, sizeof(char));
    int *requestTimes = calloc(linesLimit, sizeof(long long));

    while (!feof(logFile)) {

        fgets(curReq, maxReqLen, logFile);
        data curReqData = getData(curReq);

        if (dataCorrect(curReqData)) {
            if (stringCnt == linesLimit) {
                linesLimit *= 2;
                requestTimes = realloc(requestTimes, linesLimit * sizeof(long long));
            }

            requestTimes[stringCnt] = getTime(curReqData.localTime);

            if (curReqData.status[0] == '5') {
                push(&requests,
                     curReqData.request); // помещаем искомый запрос в очередь. pop() - возвращает первый пришедший, popBack - последний пришедший.
                ErrCnt++;
            }
            stringCnt++;
        }
    }

    long long *tmWindowInterval = calloc(3,
                                         sizeof(long long)); // [0] - левая граница, [1] - правая граница, [2] - количество запросов.=
    timeWindowSearch(&tmWindowInterval, stringCnt, timeWindow, requestTimes);

    printf("\nTime window: [t; t+%d]\nMax value of requests(%lld) from %lld to %lld\n\n",
           timeWindow, tmWindowInterval[2], tmWindowInterval[0], tmWindowInterval[1]);
    printf("There are %lld 5xx errors\n\nList of requests:\n\n", ErrCnt);

    for (long long i = 0; i < ErrCnt; i++)
        printf("%s\n", pop(&requests));

    return 0;

}





