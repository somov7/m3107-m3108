#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "utils.h"


int main(int argc, char *argv[]) {
    long long errorsNumber;
    int timeWindow = -1;
    char *filename = argv[argc - 1];
    char **errors = malloc(1000 * sizeof(char *));
    FILE *fp;

    if (argc < 2 || argc > 3) {
        printf("Input: \"<timeWindow> <filename>\" to get time window and errors\n or \"<filename>\" to get errors\n");
        exit(1);
    }

    if (argc == 3) {
        timeWindow = atoi(argv[1]);
    }

    for (int i = 0; i < 100; i++) {
        errors[i] = (char *) malloc(1000 * sizeof(char));
    }

    fp = fopen("access_log_Jul95", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    if (timeWindow != -1) {
        printMaxRequests(fp, timeWindow);
        rewind(fp);
        printf("\n");
    }

    errorsNumber = getErrors(fp, errors);
    printf("5xx errors: %lld\n", errorsNumber);

    for (long long i = 0; i < errorsNumber; i++) {
        printf("%s\n", errors[i]);
    }

    for (int i = 0; i < errorsNumber; i++) {
        free(errors[i]);
    }

    fclose(fp);

    return 0;
}