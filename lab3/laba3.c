#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LL long long

int isleap(int year)
{
    if (year % 4) return 0;
    else if (year % 100) return 1;
    else if (year % 400) return 0;
    else return 1;
}

const char* months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

LL year_month_to_sec(int year, char* month)
{
    LL sec = 0;
    for (int i = 1; i < year; i++) {
        sec += isleap(i) ? 366*24*60*60 : 365*24*60*60;
    }
    int n = 0;
    while(strcmp(month, months[n])) {
        n++;
    }
    for (int i = 0; i < n; i++) {
        sec += days[i]*24*60*60;
        if (i == 1 && isleap(year)) sec += 24*60*60;
    }
    return sec;
}

#pragma pack(push,1)
typedef struct time {
    char day[3];
    char month[4];
    char year[5];
    char hour[3];
    char min[3];
    char sec[3];
} time;

void null_terminate(time* T)
{
    T->day[2] = '\0';
    T->month[3] = '\0';
    T->year[4] = '\0';
    T->hour[2] = '\0';
    T->min[2] = '\0';
    T->sec[2] = '\0';
}

LL time_to_sec(time* T)
{
    LL secs = 0;
    secs += atoi(T->sec);
    secs += atoi(T->min)*60;
    secs += atoi(T->hour)*60*60;
    secs += (atoi(T->day)-1)*24*60*60;
    secs += year_month_to_sec(atoi(T->year), T->month);
    return secs;
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        printf("Correct usage: --file=filename --window=timeinsec");
        return 1;
    }
    char* filename = NULL;
    LL window = 0;
    for (int i = 1; i < argc; i++)
    {
        if (strstr(argv[i], "--file")) {
            filename = (char*)calloc(strlen(argv[i]) - 6, 1);
            strcpy(filename, argv[i] + 7);
        }
        else if (strstr(argv[i], "--window")) {
            window = atoll(argv[i]+9);
        }
    }
    FILE* fp = fopen(filename, "r"); //название файла
    if (!fp)
    {
        printf("Can't open file, check name/path\n");
        return 1;
    }
    LL ans_size = 50;
    LL lines = 1000; //выделим на 1000, если надо будет, увеличим
    char** ans = (char**)malloc(ans_size*sizeof(char*)); //выделим сначала массив на 50 строк, если надо будет, сделаем realloc *= 2
    LL* times = malloc(lines*sizeof(LL)); //массив для хранения времени запроса в секундах для подсчета максимального количества запросов в промежутке [a; a+t]
    char buf[4096]; //с запасом
    LL error_count = 0;
    LL n = 0; //реальное количество строк
    while (fgets(buf, sizeof(buf), fp) != NULL) //читаем построчно 
    {
        time T;
        int i = 0;
        while (buf[i++] != '[');
        memcpy(&T, buf+i, 20); //все поля времени фиксированной длины 
        //если год <= 9999 (работает в пределах разумного)
        //предполагаем, что NASA логов не было до 1000 года, либо годы форматированы как и остальные поля, например, 0011 год
        null_terminate(&T);
        if (n == lines) {
            lines *= 2;
            times = realloc(times, lines*sizeof(LL));
        }
        times[n++] = time_to_sec(&T);

        for (int c = 0; c != 2;) {
            if (buf[i++] == '"') c++; //символ " в ссылке будет закодирован как %22, поэтому проблем нет
        }
        if (buf[i+1] == '5') {
            if (error_count == ans_size) {
                ans_size *= 2;
                ans = realloc(ans, ans_size*sizeof(char*));
            }
            ans[error_count] = malloc(strlen(buf)+1); //список неудачных запросов
            strcpy(ans[error_count++], buf);
        }
    }
    LL lp = 0;
    LL t = 0, mx = 0;
    LL lt = times[0], rt = times[0];
    LL c = 1;
    LL line_start = 0, line_end = 0;
    for (LL i = 1; i < n; i++) //метод двух указателей, sliding window 
    {
        t += times[i] - times[i-1];
        c++;
        while (t > window && lp+1 < n) {
            t -= times[lp+1] - times[lp];
            lp++;
            c--;
        }
        if (c > mx) {
            lt = times[lp];
            rt = times[i];
            mx = c;
            line_start = lp;
            line_end = i;
        }
    }
    fclose(fp);
    free(times);
    printf("max window of length %lld [t; t+%lld] in secs:\n%lld to %lld\nMax requests in window: %lld at lines: %lld-%lld\n", window, window, lt, rt, mx, line_start+1, line_end+1);
    printf("5xx count: %lld\n", error_count);
    /* чтобы не унесло строчки выше
    if (error_count) {
        for (LL i = 0; i < error_count; i++) {
            printf("%s\n", ans[i]);
        }
    }
    */
    return 0;
}
