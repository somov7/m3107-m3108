#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define MaxStrLen 500
#define MaxNumOfWords 15

long long get_max(long long a, long long b){
    if(a >= b){
        return a;
    }
    else{
        return b;
    }
}

int detect_month(char *mon){
    char months[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    for (int i = 0; i < 12; i++){
        if (strcmp(mon, months[i]) == 0){
            return i + 1;
        }
    }
}


struct tm *parse_date(char *date_str, struct tm *res){
    char date_cpy[MaxStrLen];
    strcpy(date_cpy, date_str);
    char sep_date[3] = {'/', ':'};
    char *temp;
    int i = 0;
    temp = strtok(date_cpy, sep_date);
    while (temp != NULL){
        // printf("%d %s\n", i, temp);
        if (i == 0){
            res->tm_mday = atoi(temp);
        }
        else if (i == 1){
            res->tm_mon = detect_month(temp);
        }
        else if (i == 2){
            res->tm_year = (atoi(temp) - 1900);
        }
        else if (i == 3){
            res->tm_hour = atoi(temp);
        }
        else if (i == 4){
            res->tm_min = atoi(temp);
        }
        else if (i == 5){
            res->tm_sec = atoi(temp);
        }
        temp = strtok(NULL, sep_date);
        i++;
    }
    return res;
}

char sep[5] = {' ', '[', ']', '\"'};

char buf[MaxStrLen];

int get_parts_fixed(char *str, int index, char *part){
    int num = 0;
    char *res;
    char temp[MaxNumOfWords][MaxStrLen];
    res = strtok(str, sep);
    while(res != NULL){
        strcpy(temp[num], res);
        num++;
        res = strtok(NULL,sep);
    }
    if (index >= 0) {
        strcpy(part, temp[index]);
    }
    else {
        strcpy(part, temp[num + index]);
    }
    return num;
}

char code[MaxStrLen];

char *get_code(char *str){
    char s[strlen(str)];
    strcpy(s, str);
    get_parts_fixed(s, -2, code);
    return code;
}

char *get_date(FILE *filename, char *str){
    fgets(str, MaxStrLen, filename);
    str[strlen(str) - 1] = 0;
    char s[strlen(str)];
    strcpy(s, str);
    get_parts_fixed(s, 3, buf);
    return buf;
}

time_t get_time(char *date_str){
    //printf("FIX %s\n", date_str);
    struct tm date_tm;
    parse_date(date_str, &date_tm);
    time_t res = mktime(&date_tm);
    //printf("%d %d %d %d %d %d\n", date_tm.tm_year, date_tm.tm_mon, date_tm.tm_mday, date_tm.tm_hour, date_tm.tm_min, date_tm.tm_sec);
    //printf("res is %ld\n", res);
    return res;
}


int main() {
    FILE  *fin1, *fin2;
//    fin1 = fopen("logtest.txt", "r");
//    fin2 = fopen("logtest.txt", "r");
    fin1 = fopen("access_log_Jul95.txt", "r");
    fin2 = fopen("access_log_Jul95.txt", "r");
    char str1[MaxStrLen];
    char str2[MaxStrLen];
    char *start_str;
    char *end_str;
    time_t time_start;
    time_t time_end;
    time_t window = 5;
    long long amount_of_5xx = 0;
    // scanf("%ld", &window);
    // printf("%ld\n",window);
    long long ans = 0;
    long long cnt = 0;
    start_str = get_date(fin1, str1);
    end_str = get_date(fin2, str2);
    time_start = get_time(start_str);
    time_end = get_time(end_str);
    long int dif_time = time_end - time_start;

    get_code(str1);
    if (atoi(code) >= 500 && atoi(code) < 600){
        printf("%s\n", str1);
        amount_of_5xx++;
    }

    printf("str1 is %s\n", str1);
    printf("str2 is %s\n", str2);
    printf("dif_time is %ld\n", dif_time);
    printf("begin\n");
    while(1){
        // printf("%s\n", end_str);
        if (feof(fin1)){
            cnt++;
            ans = get_max(ans, cnt);
            break;
        }

        //        printf("cnt is %d\n", cnt);
        dif_time = time_end - time_start;
        //        printf("dif_time is %ld\n", dif_time);
        if (dif_time <= window){
            cnt++;
            memset(end_str, 0, sizeof(end_str));
            end_str = get_date(fin1, str1);
            get_code(str1);
            if (atoi(code) >= 500 && atoi(code) < 600){
                printf("%s\n", str1);
                amount_of_5xx++;
            }
            time_end = get_time(end_str);
        }
        else{
            ans = get_max(ans, cnt);
            cnt--;
            memset(start_str, 0, sizeof(start_str));
            start_str = get_date(fin2, str2);
            time_start = get_time(start_str);
        }

    }
    printf("completed successfully\n");
    printf("amount of 5xx codes: %lld\n", amount_of_5xx);
    printf("answer is %lld\n", ans);
    return 0;
}