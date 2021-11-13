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

long long timeFromString(char *str) {
    if (str == NULL)
        return 0;
    long long result = 0;
    char months[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    for (int i = 0; i < 12; ++i)
        if (!strcmp(months[i], str + 3)) {
            result = i*2592000;
            break;
        }
//  изначально использовал функцию strptime() из time.h, которая автоматически парсит строку в time_t,
//  но ручной парс работает ± в 15 раз быстрее (по крайней мере на моём ПК), поэтому оставил его :)
    int d = atoi(str);
    int y = atoi(str + 7)-1995;
    int h = atoi(str + 12);
    int s = atoi(str + 18);
    int m = atoi(str + 15);
    int gh = (str[22] - '0') * 10 + (str[23] - '0');
    int gm = (str[24] - '0') * 10 + (str[25] - '0');
    result += y * 31536000 + d * 86400 + h * 3600 + m * 60 + s;
    if (str[21] == '-')
        result -= (gh * 3600 + gm * 60);
    else
        result += (gh * 3600 + gm * 60);
    return result;
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
        long long firstTime = timeFromString(subStr1);
        currentPosition1 = ftell(input);
        if (currentPosition2 > currentPosition1)
            fseek(input, currentPosition2, SEEK_SET);
        while (currentPosition2 < end) {
            fgets(str2, 2049, input);
            currentPosition2 = ftell(input);
            if (parseOnError(str2))
                errorsCount++;
            char *subStr2 = getSubstring(str2);
            long long lastTime = timeFromString(subStr2);
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
