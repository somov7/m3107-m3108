#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>

const char *months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
int err_count = 0;
int line_num = 0;


int get_error_num(char *ptr) {
    char *ptr_copy = ptr;
    int error_num = -1;
    char *error_ptr;
    char error[4] = "\0";
    for (int i = 0; i < 2; i++) {
        error_ptr = strstr(ptr_copy, "\"");
        if (error_ptr != NULL) {
            ptr_copy = error_ptr + 1;
        }
    }
    if (error_ptr  != NULL) {
        while (!isdigit(*error_ptr))
        {
            error_ptr++;
        }
        for (int i = 0; i < 3; i++) {
            error[i] = *(error_ptr + i);
        }
        error[3] = '\0';
        error_num = atoi(error);
    }
    return error_num;
}

struct tm t_stamp(char *str) {
    struct tm time = {0};
    time.tm_sec = -1;
    time.tm_year = -1900;
    char *ptr;
    ptr = strstr(str, "[");
    if (ptr != NULL) {
        ptr++;
        char buff[2] = "\0";
        char mnth[4] = "\0";
        char year[5] = "\0";

        buff[0] = *(ptr);
        buff[1] = *(ptr + 1);

        time.tm_mday = atoi(buff);

        ptr = ptr + 3;
        mnth[0] = *ptr;
        mnth[1] = *(ptr + 1);
        mnth[2] = *(ptr + 2);
        for (int i = 0; i < 12; i++) {
            if (strcmp(mnth, months[i]) == 0) {
                time.tm_mon = i;
                break;
            }
        }
        ptr = ptr + 4;
        year[0] = *(ptr);
        year[1] = *(ptr + 1);
        year[2] = *(ptr + 2);
        year[3] = *(ptr + 3);
        time.tm_year += atoi(year);
        ptr = ptr + 5;

        buff[0] = *ptr;
        buff[1] = *(ptr + 1);
        time.tm_hour = atoi(buff);
        ptr = ptr + 3;
        buff[0] = *ptr;
        buff[1] = *(ptr + 1);
        time.tm_min = atoi(buff);
        ptr = ptr + 3;
        buff[0] = *ptr;
        buff[1] = *(ptr + 1);
        time.tm_sec = atoi(buff);
    }
    return time;
}

void request_parse(FILE *fptr, char *str, int error_num) {
    err_count++;
    char *beginning = strstr(str, "\"") + 1;
    if (beginning != NULL) {
        fprintf(fptr, "%d", err_count);
        fputs(". Line: ", fptr);
        fprintf(fptr, "%d", line_num);
        fputc(' ', fptr);
        while (*beginning != '"') {
            fputc(*beginning, fptr);
            beginning++;
        }
        fputs(" Error: ", fptr);
        fprintf(fptr, "%d", error_num);
        fputc('\n', fptr);
    }
}


int main(int argc, char *argv[]) {
    char str[512] = "\0";
    FILE *ptr = NULL;
    FILE *backing_ptr = NULL;
    FILE *wptr = NULL;
    if (argc == 4) {
        ptr = fopen(argv[1], "r");
        backing_ptr = fopen(argv[1], "r");
    }
    if (argc >= 3) {
        wptr = fopen(argv[2], "w");
    }
    //Переменные для параметризации окна
    struct tm upper_bound_t;
    struct tm lower_bound_t;
    long long request_count = 0;
    long long diff_t;
    long long max_request_count = 0;
    struct tm upper_time;
    struct tm lower_time;


    if (ptr == NULL || backing_ptr == NULL) {
        fprintf(stderr, "The program has encountered a problem whilst trying to open a '.log' file\nError code:%d",
                errno);
    } else if (wptr == NULL) {
        fprintf(stderr, "The program has encountered a problem whilst trying to open an output file\n");
    } else {
        if (isdigit(argv[3][0]) && argv[3][0] > '0') {
            fgets(str, 512, backing_ptr);
            struct tm temp = t_stamp(str);
            upper_bound_t = temp;
            lower_bound_t = temp;
            diff_t = atoll(argv[3]) - 1;
        }

        while (fgets(str, 512, ptr) != NULL) {
            line_num++;
            int i = get_error_num(str);
            if (i >= 500 && i < 600) {
                request_parse(wptr, str, i);
            }
            if (isdigit(argv[3][0]) && argv[3][0] > '0') {
                struct tm temp = t_stamp(str);
                if (temp.tm_sec > -1) {
                    lower_bound_t = temp;
                }
                request_count++;
                if ((mktime(&lower_bound_t) - mktime(&upper_bound_t)) <= diff_t) {
                    if (request_count > max_request_count) {
                        max_request_count = request_count;
                        upper_time = upper_bound_t;
                        lower_time = lower_bound_t;
                    }
                } else {
                    while ((mktime(&lower_bound_t) - mktime(&upper_bound_t)) > diff_t) {
                        fgets(str, 512, backing_ptr);
                        temp = t_stamp(str);
                        if (temp.tm_sec > -1) {
                            upper_bound_t = temp;
                        }
                        request_count--;
                    }
                }
            }
            if (request_count > max_request_count) {
                max_request_count = request_count;
                upper_time = upper_bound_t;
                lower_time = lower_bound_t;
            }
        }
        if (argc >= 3 && isdigit(argv[3][0]) && argv[3][0] > '0') {
            printf("Max requests window in given time is\nFrom: %s", asctime(&upper_time));
            printf("Request count: %lli\n", max_request_count);
            printf("To: %s", asctime(&lower_time));

        }
        fclose(ptr);
        fclose(backing_ptr);
        fclose(wptr);
        return 0;
    }

}