#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

char months[] ="JanFebMarAprMayJunJulAugSepOctNovDec";

typedef struct tagLOG {
    char remote_addr[32];
    char local_time[32];
    time_t epoch;
    char request[340];
    int status;
    char bytes_send[16];
} log;

typedef struct tagWINDOW {
    time_t start, end;
    int size;
} window;

typedef struct elem_tag {
    time_t value;
    struct elem_tag *next;
} elem;

typedef struct tagQueue{
    elem *head, *tail;
} queue;

int count_quotes(char *s) {
    int cnt = 0;

    while (*s++)
        if (*s == '"')
            cnt++;
    return cnt;
}

void push(queue **cur, time_t x)
{
    elem *temp;
    temp = (elem*)malloc(sizeof(elem));
    temp->value = x;
    temp->next = NULL;

    if (((*cur)->head) == NULL) {
        *cur = (queue*)malloc(sizeof(queue));
        (*cur)->head = (*cur)->tail = temp;
    }
    else {
        (*cur)->tail->next = temp;
        (*cur)->tail = (*cur)->tail->next;
    }
}

void pop(queue **cur)
{
    if ((*cur)->head == (*cur)->tail)
        *cur = NULL;
    else
        (*cur)->head = (*cur)->head->next;
}

log parse_log(char *s) {
    log l;
    struct tm t;
    char month[4];
    char *pos;

    sscanf(s, "%s - - [%[^]]] \"%[^\"]\" %d %s", l.remote_addr, l.local_time,
           l.request, &l.status, l.bytes_send);

    t.tm_isdst = -1;
    sscanf(l.local_time, "%d/%3s/%d:%d:%d:%d", &t.tm_mday, month, &t.tm_year, &t.tm_hour, &t.tm_min, &t.tm_sec);
    pos = strstr(months, month);
    t.tm_year -= 1900;
    t.tm_mon = (pos - months) / 3;
    l.epoch = mktime(&t);
    return l;
}
char *convert_unixtime(time_t x){
    struct tm *tmp = localtime(&x);
    char *time_str = (char*)malloc(21);
    strftime(time_str, 21, "%d/%b/%Y:%H:%M:%S", tmp);
    return time_str;
}
int main() {
    FILE *F = fopen("access_log_Jul95.log", "r"),
         *err_file = fopen("error.log", "w");

    char s[513];
    int cnt = 0, diff, cntw = 0;
    log l;
    queue *q = malloc(sizeof(queue));
    window *win = malloc(sizeof(window));
    q->head = NULL;
    q->tail = NULL;
    win->size = 0;

    printf("Time interval (seconds):");
    scanf("%i", &diff);
    while (fgets(s, 513, F)) {
        if (count_quotes(s) > 2) {
            fprintf(err_file, "%s", s);
            continue;
        }
        if (q->head != NULL && q->tail != NULL) {
            while ((difftime(q->tail->value, q->head->value) > diff) && cntw > 0) {
                pop(&q);
                cntw--;
            }
        }
        if (cntw > win->size) {
            win->size = cntw;
            win->start = q->head->value;
            win->end = q->tail->value;
        }

        l = parse_log(s);
        push(&q, l.epoch);
        cntw++;
        if (l.status / 100 == 5) {
            printf("%s", s);
            cnt++;
        }
    }
    printf("5xx requests: %d\nMost active window from %s (-0400) to %s (-0400): %d requests", cnt, convert_unixtime(win->start), convert_unixtime(win->end), win->size);
    free(q);
    free(win);
    return 0;
}











