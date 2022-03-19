#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef struct node{
    char* value;
    struct node* next;
}node;

void push(node** head, char* log){
    node* tmp = malloc(sizeof(node));
    tmp->value = calloc(strlen(log) + 1, sizeof(char));
    strcpy(tmp->value, log);
    tmp->next = *head;
    (*head) = tmp;
}

char* pop(node** head){
    if (*head == NULL){
        printf("Error: list is empty");
        exit(-1);
    }
    char* answer = (**head).value;
    *head = (*head)->next;
    return answer;
}


const int maxReqLen = 4096;
const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

typedef struct log {
    char *remoteAddr;
    char *localTime;
    char *timeZone;
    char *request;
    char *status;
    char *bytesSend;
} log;

log getlog(char *request) {
    log answer;

    answer.remoteAddr = strtok(request, "-");

    strtok(NULL, "[");
    answer.localTime = strtok(NULL, " ");

    answer.timeZone = strtok(NULL, "]");

    strtok(NULL, "\"");
    answer.request = strtok(NULL, "\"");

    answer.status = strtok(NULL, " ");
    answer.bytesSend = strtok(NULL, "\n");

    return answer;
}

int getMonthNumber(char *mon) {
    int answer;
    for (answer = 0; answer < 12 && strcmp(mon, months[answer]); answer++);
    return answer + 1;
}

int getTime(char *reqTime) {
    int answer;
    struct tm localTime = {};

    localTime.tm_mday = atoi(strtok(reqTime, "/"));
    localTime.tm_mon = getMonthNumber(strtok(NULL, "/"));
    localTime.tm_year = atoi(strtok(NULL, ":")) - 1900; // years since 1900
    localTime.tm_hour = atoi(strtok(NULL, ":"));
    localTime.tm_min = atoi(strtok(NULL, ":"));
    localTime.tm_sec = atoi(strtok(NULL, ":"));

    answer = mktime(&localTime);

    return answer;
}

void timeWindowSearch(long long **answer, long long ReqCnt, int tmWindow, int *tmReq) {

    long long mxLen = 0, j = 1;

    for (long long i = 0; i < ReqCnt; i++) {
        while (j < ReqCnt && tmReq[j] - tmReq[i] <= tmWindow) {
            j++;
        }
        if (tmReq[j] - tmReq[i] > tmWindow) {
            if (j - i > mxLen) {
                mxLen = j - i;
                (*answer)[0] = tmReq[i];
                (*answer)[1] = tmReq[j - 1];
                (*answer)[2] = mxLen;
            }
        } else {
            if (j - i > mxLen) {
                mxLen = j - i;
                (*answer)[0] = tmReq[i];
                (*answer)[1] = tmReq[j - 1];
                (*answer)[2] = mxLen;
            }
            break;
        }
    }

}

int logCorrect(log curlog) {
    return !(curlog.localTime == NULL || curlog.request == NULL || curlog.status == NULL ||
             curlog.timeZone == NULL || curlog.remoteAddr == NULL || curlog.bytesSend == NULL);
}

int main(int argc, char *argv[]) {

    FILE *logFile = fopen(argv[1], "r");
    if (argc < 3) {
        printf("invalid value of arguments");
        return 1;
    }

    if (logFile == NULL) {
        printf("Incorrect file name");
        return 2;
    }

    if (argv[2] < 0) {
        printf("Time window must be more then 0");
        return 3;
    }

    node *requests = NULL;
    int timeWindow = atoi(argv[2]);
    long long lines_step = 256; // Будем динамически выделять память, увеличивая её в два раза при переполнении. работает как вектор в c++
    long long string_counter = 0;
    long long errors_counter = 0;
    char *curReq = calloc(maxReqLen, sizeof(char));
    int *requestTimes = calloc(lines_step, sizeof(long long));

    while (!feof(logFile)) {

        fgets(curReq, maxReqLen, logFile);
        log curReqlog = getlog(curReq);

        if (logCorrect(curReqlog)) {
            if (string_counter == lines_step) {
                lines_step *= 2;
                requestTimes = realloc(requestTimes, lines_step * sizeof(long long));
            }

            requestTimes[string_counter] = getTime(curReqlog.localTime);

            if (curReqlog.status[0] == '5') {
                push(&requests, curReqlog.request); // помещаем искомый запрос в очередь. pop() - возвращает первый пришедший, popBack - последний пришедший.
                errors_counter++;
            }
            string_counter++;
        }
    }

    long long *tmWindowInterval = calloc(3, sizeof(long long)); // [0] - левая граница, [1] - правая граница, [2] - количество запросов.=
    timeWindowSearch(&tmWindowInterval, string_counter, timeWindow, requestTimes);

    printf("\nTime window: [t; t+%d]\nMax value of requests(%lld) from %lld to %lld\n\n",
           timeWindow, tmWindowInterval[2], tmWindowInterval[0], tmWindowInterval[1]);
    printf("There are %lld 5xx errors\n\nList of requests:\n\n", errors_counter);

    for (long long i = 0; i < errors_counter; i++)
        printf("%s\n", pop(&requests));

    return 0;

}