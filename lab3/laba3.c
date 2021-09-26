#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LL long long
LL precount_days[13]; //получить время в секундах марта это сумма дней до марта * 24 * 60 * 60 //LL тут как и далее для автоматических преобразований в long long
LL days_in_months[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
LL precount_days_leap[13]; //получить время в секундах марта это сумма дней до марта * 24 * 60 * 60 //LL тут как и далее для автоматических преобразований в long long
LL days_in_months_leap[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
LL years[2022];
LL years_precount[2022]; //предпосчет дней в году в зависимости високосного года

int isleap(int year)
{
    int leap = 0;
    if (year % 4) leap = 0;
    else if (year % 100) leap = 1;
    else if (year % 400) leap = 0;
    else leap = 1;
    return leap;
}

LL tosec(char* str, char* t)
{
    if (!strcmp(t, "sec")) return atoi(str);
    else if (!strcmp(t, "min")) return 60*atoi(str);
    else if (!strcmp(t, "hour")) return 60*60*atoi(str);
    else if (!strcmp(t, "day")) return 24*60*60*(atoi(str)-1); //первый день это ноль сек, не будет ситуации что январь 31 > февраля 1
    else if (!strcmp(t, "year")) return 24*60*60*1LL*years_precount[atoi(str)]; //отсчет От Рождества Христова
    else
    {
        int leap = isleap(atoi(str));
        if (!strcmp(t, "Jan")) 
        {
            if (leap) return precount_days_leap[0]*24*60*60;
            else return precount_days[0]*24*60*60;
        }
        else if (!strcmp(t, "Feb")) 
        {
            if (leap) return precount_days_leap[1]*24*60*60;
            else return precount_days[1]*24*60*60;
        }
        else if (!strcmp(t, "Mar")) 
        {
            if (leap) return precount_days_leap[2]*24*60*60;
            else return precount_days[2]*24*60*60;
        }
        else if (!strcmp(t, "Apr")) 
        {
            if (leap) return precount_days_leap[3]*24*60*60;
            else return precount_days[3]*24*60*60;
        }
        else if (!strcmp(t, "May")) 
        {
            if (leap) return precount_days_leap[4]*24*60*60;
            else return precount_days[4]*24*60*60;
        }
        else if (!strcmp(t, "Jun")) 
        {
            if (leap) return precount_days_leap[5]*24*60*60;
            else return precount_days[5]*24*60*60;
        }
        else if (!strcmp(t, "Jul")) 
        {
            if (leap) return precount_days_leap[6]*24*60*60;
            else return precount_days[6]*24*60*60;
        }
        else if (!strcmp(t, "Aug")) 
        {
            if (leap) return precount_days_leap[7]*24*60*60;
            else return precount_days[7]*24*60*60;
        }
        else if (!strcmp(t, "Sep")) 
        {
            if (leap) return precount_days_leap[8]*24*60*60;
            else return precount_days[8]*24*60*60;
        }
        else if (!strcmp(t, "Oct")) 
        {
            if (leap) return precount_days_leap[9]*24*60*60;
            else return precount_days[9]*24*60*60;
        }
        else if (!strcmp(t, "Nov")) 
        {
            if (leap) return precount_days_leap[10]*24*60*60;
            else return precount_days[10]*24*60*60;
        }
        else if (!strcmp(t, "Dec")) 
        {
            if (leap) return precount_days_leap[11]*24*60*60;
            else return precount_days[11]*24*60*60;
        }
    }
}

int main(int argc, char* argv[])
{
    precount_days[0] = 0;
    precount_days_leap[0] = 0;
    for (int i = 1; i < 13; i++)
    {
        precount_days[i] = precount_days[i-1] + days_in_months[i-1];
        precount_days_leap[i] = precount_days_leap[i-1] + days_in_months_leap[i-1];
    }
    years_precount[0] = 0;
    years[0] = 0;
    for (int i = 1; i < 2022; i++)
    {
        int leap = isleap(i);
        if (leap) years[i] = 366;
        else years[i] = 365;
    }
    for (int i = 1; i < 2022; i++)
        years_precount[i] = years_precount[i-1] + years[i-1];

    FILE* fp = fopen("test.txt", "r"); //название файла
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
    LL window;
    printf("Enter time window length in seconds.\n");
    scanf("%lld", &window);
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
        t += tosec(split, "year");
        t += tosec(split, month);
        c = 0;
        while (split != NULL) //для вычленения времени используем токенайзер strtok, переводим все в секунды, годы и месяцы в файле совпадают
        {
            c++;
            //printf("c: %d str: %s\n", c, split);
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
        //printf("time: %d\n", t);
        for (int i = 0; i < 3; i++)
            free(buf2[i]);

    }
    int lp = 0, rp = 0;
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