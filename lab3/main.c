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
    int countErrors = 0;
    char month[4];
    char remote_addr[2048];
    char local_time[32];
    char request[2048];
    int status;
    int bytes_send;

    FILE *log;
    log = fopen(argv[2], "r");
    checkInput(argc, log);

    printf("List of requests ended with error 5xx");
    while (!feof(log)) {
        fscanf(log, "%s - - [%[^]]] \"%[^\"]\" %d %d", &remote_addr, &local_time, &request, &status, &bytes_send);
        if (status / 100 == 5) {
            countErrors++;
            printf("\n%d. %s - error %d", countErrors, request, status);
        }
    }
    printf("\n%d", countErrors);
}
