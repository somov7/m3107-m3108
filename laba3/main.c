#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "list.h"
#include "parser.h"

typedef struct{
    char *remote_addr;
    time_t local_time;
    char *request;
    uint status;
    uint bytes_send;
}Log;

// 199.72.81.55 - - [01/Jul/1995:00:00:01 -0400] "GET /history/apollo/ HTTP/1.0" 200 6245

Log *get_log(FILE *file) {
    const static char *mon[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    char *line = NULL;
    size_t len = 0;

    getline(&line, &len, file);
    if (line == NULL) {
        return NULL;
    }

    Log *log = malloc(sizeof(Log));
    char *tok = strtok(line, " -");
    if (tok == NULL) {
        free(log);
        return NULL;
    }
    log->remote_addr = tok;
    strtok(NULL, "[");

    struct tm time = {0};
    tok = strtok(NULL, "/: ]");
    if (tok == NULL) {
        //free(time);
        free(log);
        return NULL;
    }
    time.tm_mday = atoi(tok);
    char *month =  strtok(NULL, "/: ]");
    if (month == NULL) {
        //free(time);
        free(log);
        return NULL;
    }
    time.tm_mon = -1;
    for (int i = 0; i < 11; i++) {
        if (strcmp(mon[i], month) == 0) {
            time.tm_mon = i;
            break;
        }
    }

    tok = strtok(NULL, "/: ]");
    if (tok == NULL) {
        //free(time);
        free(log);
        return NULL;
    }
    time.tm_year = atoi(tok) - 1900;

    tok = strtok(NULL, "/: ]");
    if (tok == NULL) {
        //free(time);
        free(log);
        return NULL;
    }
    time.tm_hour = atoi(tok);

    tok = strtok(NULL, "/: ]");
    if (tok == NULL) {
        //free(time);
        free(log);
        return NULL;
    }
    time.tm_min = atoi(tok);

    tok = strtok(NULL, "/: ]");
    if (tok == NULL) {
        //free(time);
        free(log);
        return NULL;
    }
    time.tm_sec = atoi(tok);

    tok = strtok(NULL, "/: ]");
    if (tok == NULL) {
        //free(time);
        free(log);
        return NULL;
    }
    time.tm_gmtoff =  atoi(tok);
    time.tm_isdst = -1;
    if ((log->local_time = mktime(&time)) == -1) {
        free(log);
        return NULL;
    }

    strtok(NULL, "\"");
    tok = strtok(NULL, "\"");
    if (tok == NULL) {
        //free(time);
        free(log);
        return NULL;
    }
    log->request = tok;

    tok = strtok(NULL, "/: ]");
    if (tok == NULL) {
        //free(time);
        free(log);
        return NULL;
    }
    log->status = atoi(tok);

    tok = strtok(NULL, "/: ]");
    if (tok == NULL) {
        //free(time);
        free(log);
        return NULL;
    }
    log->bytes_send = atoi(tok);
    return log;
}




void print_log(Log *log) {
    char *time = malloc(200);
    strftime(time, 200, "%d/%b/%Y:%X -0400", gmtime(&log->local_time));
    printf("%s - - [%s] \"%s\" %u %u\n", log->remote_addr, time, log->request, log->status, log->bytes_send);
    free(time);
}



int main(int argc, char** argv) {
    FILE *file = NULL;

    uint64_t time_win = 0;
    int status = 0;

    if (argc < 2) return -1;
    int c;
    while ((c = getopt(argc, argv, "t:s:f:")) != -1) {
        if (c == 't') {
            time_win = atol(optarg);
        } else if (c == 's') {
            status = atoi(optarg);
        } else if (c == 'f') {
            //printf("%s\n", optarg);
            file = fopen(optarg, "r");
            if (file == NULL) {
                perror("error: ");
                exit(EXIT_FAILURE);
            }
        } else {
            perror("bad arg");
            exit(EXIT_FAILURE);
        }

    }
    if (time_win < 0 || status < 0) {
        perror("Bad arg");
        exit(EXIT_FAILURE);
    }

    List list = init(sizeof(Log));
    Log *log;

    struct Max {
        Log *fist;
        Log *last;
        size_t max;
    }max;
    max.max = 0;
    int i = 1;
    while (!feof(file)) {
        if ((log = get_log(file)) == NULL) continue;

        if (log->status / 100 == status
         || log->status / 10 == status
         || log->status == status) {
            printf("%d: ", i++);
            print_log(log);
        }
        add_last(list, log);
        while (1) {
            Log *first = peek_fist(list);
            Log *last = peek_last(list);
            if (last->local_time - first->local_time <= time_win) {
                break;
            }
          pop_fist(list);
        }

        if (max.max < *list.list_size) {
            max.max = *list.list_size;
            max.last = peek_last(list);
            max.fist = peek_fist(list);
        }
    }

    printf("\n\n%zu\n", max.max);
    print_log(max.fist);
    print_log(max.last);
    delete(list);
    //delete(status);
}


