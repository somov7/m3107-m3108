#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const long MAX_AGE_TIME = 3153600000; // 1 век в секундах

enum ErrorsTypes {
    INCORRECT_OPTION = 1,
    INCORRECT_INTERVAL,
    FILE_NOT_FOUND,
};

void error(char errorType) {
    switch (errorType) {
        case INCORRECT_OPTION:
            printf("Incorrect options count");
            exit(INCORRECT_OPTION);
        case INCORRECT_INTERVAL:
            printf("Incorrect interval");
            exit(INCORRECT_INTERVAL);
        case FILE_NOT_FOUND:
            printf("File not found");
            exit(FILE_NOT_FOUND);
        default:
            printf("Error");
            exit(666);
    }
}

void checkOptions(int argumentsCount) {
    if (argumentsCount < 3)
        error(INCORRECT_OPTION);
}

long getInterval(char *number) {
    char *end;
    long long interval = strtol(number, &end, 10);
    if (interval > MAX_AGE_TIME || interval == INT64_MAX || interval == INT64_MIN)
        error(INCORRECT_INTERVAL);
    return interval;
}

FILE *getFile(char *filename) {
    FILE *input;
    input = fopen(filename, "r");
    if (input == NULL)
        error(FILE_NOT_FOUND);
    return input;
}

char *getSubstring(char *str) {
    char *result = malloc(26);
    int i = 0;
    while (*(str + i - 1) != '[' && *str + i != '\0')
        i++;
    if (*str == '\0')
        return NULL;
    strncpy(result, str + i, 26);
    return result;
}

time_t timeFromString(char *str) {
    if (str == NULL)
        return 0;
    char months[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    struct tm time = {0};
    for (int i = 0; i < 12; ++i)
        if (!strcmp(months[i], str + 3)) {
            time.tm_mon = i;
            break;
        }
    time.tm_year = atoi(str + 7) - 1900;
    time.tm_mday = atoi(str);
    time.tm_hour = atoi(str + 12);
    time.tm_min = atoi(str + 15);
    time.tm_sec = atoi(str + 18);
    time.tm_isdst = -1;
    time.tm_gmtoff = ((str[22] - '0') * 10 + (str[23] - '0')) * 3600 + ((str[24] - '0') * 10 + (str[25] - '0')) * 60;
    return mktime(&time);
}

char parseOnError(char *str) {
    if (str == NULL)
        return 0;
    unsigned i = 0, size = strlen(str) - 1;
    while (*(str + size - i) != ' ')
        i++;
    if (*(str + size - i - 3) != '5')
        return 0;
    unsigned k = i, j = k;
    while (1) {
        if (*(str + size - i) == '"') {
            if (j != k) {
                j = i;
                break;
            }
            j = i;
        }
        i++;
    }
    printf("%s", str + size - j);
    return 1;
}

void parseLogs(FILE *input, long long maxInterval) {
    fseek(input, 0, SEEK_END);
    long currentPosition1 = 0, currentPosition2 = 0, maxCount = 1, errorsCount = 0, end = ftell(input);
    char *maxStr1 = NULL, *maxStr2 = NULL, str1[2049], str2[2049];
    while (currentPosition2 < end) {
        fseek(input, currentPosition1, SEEK_SET);
        fgets(str1, 2049, input);
        char *subStr1 = getSubstring(str1);
        time_t firstTime = timeFromString(subStr1);
        currentPosition1 = ftell(input);
        if (currentPosition2 > currentPosition1)
            fseek(input, currentPosition2, SEEK_SET);
        while (currentPosition2 < end) {
            fgets(str2, 2049, input);
            currentPosition2 = ftell(input);
            if (parseOnError(str2))
                errorsCount++;
            char *subStr2 = getSubstring(str2);
            time_t lastTime = timeFromString(subStr2);
            if (difftime(lastTime, firstTime) > maxInterval)
                break;
            if (currentPosition2 >= end)
                break;
            maxCount++;
            maxStr2 = subStr2;
            maxStr1 = subStr1;
        }
    }
    if (errorsCount > 0) {
        printf("         /\\\n");
        printf("         ||\n");
    }
    printf("log's have %ld 5xx errors\n", errorsCount);
    printf("max log's window in %lld sec contain %ld queries\nfrom: %s\nto: %s", maxInterval, maxCount, maxStr1,
           maxStr2);
}

int main(int argc, char **argv) {
    checkOptions(argc);
    parseLogs(getFile(argv[1]), getInterval(argv[2]));
    return 0;
}
