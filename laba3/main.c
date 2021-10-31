#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

typedef struct{
    char *remote_addr;
    char *local_time;
    char *request;
    uint status;
    uint bytes_send;
}Log;

Log *get_next_log(FILE* file) {
    char *line = NULL;
    size_t len = getline(&line, &len, file);
    char *line_cpy = line;
    if (len == -1 || line == NULL) {
        return NULL;
    }
    Log *next_log = (Log *)malloc(sizeof(Log));
    next_log->remote_addr = line;

    line  = strchr(line, ' ');
    *line++ = 0;
    next_log->local_time = strchr(line, '[') + 1;
    line  = strchr(line, ']');
    *line++ = 0;
    next_log->request = strchr(line, '\"') + 1;
    line = strrchr(line, '\"');
    *line++ = 0;
    line = strchr(line, ' ') + 1;
    char *tmp = line;
    line = strchr(line, ' ');
    *line++ = 0;
    next_log->status = atoi(tmp);
    next_log->bytes_send = atoi(line);
    return next_log;
}

void print_log(Log *log) {
    printf("%s - - [%s] \"%s\" %u %u\n", log->remote_addr, log->local_time, log->request, log->status, log->bytes_send);
}


int main(int argc, char** argv) {
    FILE *file = fopen("/Users/anton/Downloads/access_log_Jul95", "r");
    if (file == NULL) {
        perror("error: ");
        exit(EXIT_FAILURE);
    }
    Log *log;
    int i = 1;
    while (log = get_next_log(file)) {
        if (log->status / 100 == 5) {
            print_log(log);
            i++;
        }
        free(log);
    }
}


