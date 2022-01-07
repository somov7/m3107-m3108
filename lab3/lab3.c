#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define SIZE 300//размер строки запроса
long timestamp[1891714]; int n = 0;


long utime(char *stime);
long get_time(char[]);
int get_error(char[]);
void get_window(int);


int main (int argc, char* argv[]) {

    //FILE* f = fopen("access_log_Jul95.txt", "r");
    
    FILE* f = fopen(argv[1], "r");
    int errorCount = 0;
    int wsize = atoi(argv[2]);
    char str[SIZE];

    if (argc != 3) {
        printf("ERROR: not enough arguments");
        return -1;
    }


    while (fgets(str, SIZE, f)) {
        timestamp[n++] = get_time(str);
        if ((get_error(str) / 100) == 5){
            ++errorCount;
        }
    }

    get_window(wsize);
    

    printf("\n5XX errors: %d\n", errorCount);

    return 0;
}


void get_window(int width) {
    long m = 0;//макс. окно
    long mp1 = 0; long mp2 = 0;//индексы границ макс. окна
    long p1 = 0; long p2 = 0;//индексы границ текущего окна
    while (p2 < n - 1) {
        
        if (timestamp[p2] - timestamp[p1] <= width) {
            ++p2;
        } else {
            if (p2 - p1 > m) {
                m = p2 - p1;
                mp1 = p1 + 1;
                mp2 = p2;
            }
            p1++;
        }
    }
    if ((timestamp[p2] - timestamp[p1] <= width) && (p2 - p1 > m)) {
        m = p2 - p1 + 1;
        mp1 = p1 + 1;
        mp2 = p2 + 1;
    }
    printf("count = %d, from %d to %d", m, mp1, mp2);
}

long get_time(char str[]) {
    
    char buff[40];//хранит строку со временем
    int ptr = 0;//индекс для времени
    int i = 0;//индекс строки
    while (str[i++] != '[');
        
    while (str[i] != ']')
        buff[ptr++] = str[i++];

    return utime(buff);
}


int get_error(char str[]) {
    char buff[4];//хранит код ошибки
    int ptr = 2;//индекс для кода ошибки
    int i = strlen(str) -  1;//индекс строки
    while (str[i--] != ' ');
        
    while (str[i] != ' ')
        buff[ptr--] = str[i--];
    return atof(buff);
}


long utime(char* stime) {
    char time_format[10] = "//::: ";//разделители строки времени

    char buff[7][6] = {""};//хранит разбиение строки stime
    int i = 0;//индекс строки stime
    int bp = 0; int wp = 0;//индексы buff
    int fp = 0; //индекс time_format
    while (stime[i]) {
        if (stime[i] == time_format[fp]) {
            ++bp;
            ++fp;
            wp = 0;
        } else
            buff[bp][wp++] = stime[i]; 
        ++i;
    }

    struct tm tdate = {
        .tm_mday = atof(buff[0]),
        .tm_mon = atof(buff[1]) - 1,
        .tm_year = atof(buff[2]) - 1900,
        .tm_hour = atof(buff[3]),
        .tm_min = atof(buff[4]),
        .tm_sec = atof(buff[5]),
        .tm_isdst = 0
    };

    time_t time = mktime(&tdate);
    return time;
}