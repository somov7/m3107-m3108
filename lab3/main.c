#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

long long counter_of_5xx = 0;

int to_parse_month(char *number) {
    char month[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    int cnt = 0;
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 3; j++) {
            if (number[j] == month[i][j]) {
                cnt++;
            } else {
                break;
            }
        }
        if (cnt == 3) {
            return i;
        } else {
            cnt = 0;
        }
    }
    return 11;
}

long long to_parse_time(FILE *fp) {
    struct tm time;
    int count = 0, i = 0;
    char current;
    char number[5];
    current = (char) getc(fp);
    while (current != '[' && current != EOF) {
        current = (char) getc(fp);
    }
    current = (char) getc(fp);
    while (current != '-' && current != EOF) {
        if (current == '/' || current == ':') {
            if (count == 0) {
                time.tm_mday = atoi(number);
            } else if (count == 1) {
                time.tm_mon = to_parse_month(number);
            } else if (count == 2) {
                time.tm_year = atoi(number) - 1900;
            } else if (count == 3) {
                time.tm_hour = atoi(number);
            } else if (count == 4) {
                time.tm_min = atoi(number);
            }
            memset(number, '\0', 5);
            count++;
            i = 0;
        } else {
            number[i] = current;
            i++;
        }
        current = (char) getc(fp);
    }
    time.tm_sec = atoi(number);
    long long time_in_long = mktime(&time);
//    printf("day: %d\nmonth: %d\nyear: %d\nhour: %d\nminute: %d\nsecond: %d\n", time.tm_mday, time.tm_mon, time.tm_year,
//           time.tm_hour, time.tm_min, time.tm_sec);
//    printf("time: %ld\n", time_in_long);
    return time_in_long;
}

void to_parse_request(FILE *fp) {
    char current = (char) getc(fp);
    char request[2048];
    int i = 0;
    while (current != '"' && current != EOF) {
        current = (char) getc(fp);
    }
    current = (char) getc(fp);
    while (current != '"' && current != EOF) {
        request[i] = current;
        current = (char) getc(fp);
        i++;
    }
    request[i] = '\0';
    current = (char) getc(fp);
    char status[3];
    int j = 0;
    while (current != '\n' && current != EOF) {
        if (current >= '0' && current <= '9') {
            status[j] = current;
            j++;
        }
        if (j == 3 && status[0] == '5') {
            counter_of_5xx++;
            printf("     %lld) %s\n", counter_of_5xx, request);
            break;
        }
        current = (char) getc(fp);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("enter filename and time window");
        return -1;
    }
    FILE *fp1;
    if ((fp1 = fopen(argv[1], "r")) == NULL) {
        return -1;
    }
    FILE *fp2;
    if ((fp2 = fopen(argv[1], "r")) == NULL) {
        return -1;
    }
    int count = 0;
    long long time_window = atoi(argv[2]);
    long long time1, time2, max_time1, max_time2;
    long long max = 0;

    while (!feof(fp1)) {
        if (count == 0) {
            time1 = to_parse_time(fp2);
            max_time1 = time1;
        }
        time2 = to_parse_time(fp1);
        if (time2 >= time1 + time_window) {
            if (count > max) {
                max = count;
                max_time1 = time1;
            }
            time1 = to_parse_time(fp2);
        } else {
            count++;
        }
        to_parse_request(fp1);
    }
    if (time2 < max_time1 + time_window) {
        max = count;
    }
    max_time2 = max_time1 + time_window - 1;
    struct tm *ans;
    ans = localtime (&max_time1);
    printf("Time window:\nfrom %s", asctime(ans));
    ans = localtime (&max_time2);
    printf("  to %s", asctime(ans));
    printf("max number of requests: %lld\nnumber of requests with 5xx errors: %lld\n", max, counter_of_5xx);
    if (fclose(fp1) == EOF && fclose(fp2) == EOF) {
        printf("error");
        return -1;
    }
    return 0;
}
// access_log_Jul95