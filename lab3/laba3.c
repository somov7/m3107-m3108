#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LL long long

int isleap(int year)
{
    int leap = 0;
    if (year % 4) leap = 0;
    else if (year % 100) leap = 1;
    else if (year % 400) leap = 0;
    else leap = 1;
    return leap;
}

const char* months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

LL year_month_to_sec(int year, char* month)
{
    LL sec = 0;
    for (int i = 1; i < year; i++)
    {
        sec += isleap(i) ? 366*24*60*60 : 365*24*60*60;
    }
    int n = 0;
    while(strcmp(month, months[n]))
        n++;
    for (int i = 0; i < n; i++)
    {
        sec += days[i]*24*60*60;
        if (i == 1 && isleap(year)) sec += 24*60*60;
    }
    return sec;
}

LL tosec(char* str, char* t)
{
    if (!strcmp(t, "sec")) return atoi(str);
    else if (!strcmp(t, "min")) return 60*atoi(str);
    else if (!strcmp(t, "hour")) return 60*60*atoi(str);
    return 24*60*60*(atoi(str)-1); //ничего другого больше не могли передать, кроме дня
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
        if (strstr(argv[i], "--file"))
        {
            filename = (char*)calloc(strlen(argv[i]) - 6, 1);
            strcpy(filename, argv[i] + 7);
        }
        else if (strstr(argv[i], "--window"))
        {
            window = atoll(argv[i]+9);
        }
    }
    FILE* fp = fopen(filename, "r"); //название файла
    if (!fp)
    {
        printf("Can't open file, check name/path\n");
        return 1;
    }
    int ans_size = 50;
    char buf[4096]; //с запасом
    LL c = 0;
    char** ans = (char**)malloc(ans_size*sizeof(char*)); //выделим сначала массив на 50 строк, если надо будет, сделаем realloc *= 2
    char* buf2[3];
    int error_count = 0;
    int lines = 1000; //выделим на 1000, если надо будет, увеличим
    LL* times = malloc(lines*sizeof(LL)); //массив для хранения времени запроса в секундах для подсчета максимального количества запросов в промежутке [a; a+t]
    int n = 0; //реальное количество строк
    while (fgets(buf, sizeof(buf), fp) != NULL) //читаем построчно 
    {
        c = 0;
        char* split = strtok(buf, "\""); //используем токенайзер strtok
        while (split != NULL) 
        {
            //printf("%s\n", split);
            buf2[c] = malloc(strlen(split)+1);
            strcpy(buf2[c], split);
            split = strtok(NULL, "\"");
            c++;
        }
        split = strtok(buf2[2], " ");
        if (split[0] == '5') //неудачный запрос 5хх
        {
            if (error_count == ans_size) 
            {
                ans = realloc(ans, 2*ans_size*sizeof(char*));
                ans_size *= 2;
            }
            
            ans[error_count] = malloc(strlen(buf2[1])+1); //список неудачных запросов
            strcpy(ans[error_count++], buf2[1]);
        }
        split = strtok(buf2[0], "[]");
        split = strtok(NULL, "[]");
        char* temp = split;
        split = strtok(temp, "/: ");
        LL t = 0;
        t += tosec(split, "day");
        split = strtok(NULL, "/: "); //месяц
        char* month = split;
        split = strtok(NULL, "/: "); //год
        //t += tosec(split, "year");
        t += year_month_to_sec(atoi(split), month);
        //t += tosec(split, month);
        c = 0;
        while (split != NULL) //для вычленения времени используем токенайзер strtok, переводим все в секунды, годы и месяцы в файле совпадают
        {
            c++;
            if (c == 2) t += tosec(split, "hour");
            else if (c == 3) t += tosec(split, "min");
            else if (c == 4) t += tosec(split, "sec");
            split = strtok(NULL, "/: ");
        }
        if (n == lines) 
        {
            times = realloc(times, lines*2*sizeof(LL));
            lines *= 2;
        }
        times[n++] = t;
        for (int i = 0; i < 3; i++)
            free(buf2[i]);

    }
    int lp = 0;
    LL t = 0, mx = 0;
    LL lt = times[0], rt = times[0];
    c = 1;
    int tl = 0, tr = 0;
    for (int i = 1; i < n; i++) //метод двух указателей, sliding window 
    {
        t += times[i] - times[i-1];
        c++;
        while (t > window && lp+1 < n)
        {
            t -= times[lp+1] - times[lp];
            lp++;
            c--;
        }

        if (c > mx)
        {
            lt = times[lp];
            rt = times[i];
            mx = c;
            tl = lp;
            tr = i;
        }
    }
    fclose(fp);
    free(times);
    printf("max window of length %lld [t; t+%lld] in secs:\n%lld to %lld\nMax requests in window: %lld at lines: %d-%d\n", window, window, lt, rt, mx, tl+1, tr+1);
    printf("5xx count: %d\n", error_count);
    /* список запросов (именно запросов, т.е. GET POST) закамментил, чтобы было не унесло строчки выше
    if (error_count)
    {
        for (int i = 0; i < error_count; i++)
            printf("%s\n", ans[i]);
    }
    */
    return 0;
}