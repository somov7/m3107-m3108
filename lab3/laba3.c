#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define LL long long
const char* months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

LL tosecs(char* str)
{
    struct tm time = { .tm_year = atoi(str+7) - 1900, .tm_mday = atoi(str), .tm_hour = atoi(str+12), 
    .tm_min = atoi(str+15), .tm_sec = atoi(str+18), .tm_isdst = -1};
    str[6] = '\0';
    int n = 0;
    while(strcmp(str+3, months[n]))
        n++;
    time.tm_mon = n;
    return mktime(&time);
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
        char str[21];
        int i = 0, j = strlen(buf)-1;
        while (buf[i++] != '[');
        memcpy(str, buf+i, 20);
        str[20]='\0';
        if (n == lines) {
            lines *= 2;
            times = realloc(times, lines*sizeof(LL));
        }
        times[n++] = tosecs(str);

        while (buf[j--] != ' ');
        if (buf[j-2] == '5') {
            if (error_count == ans_size) {
                ans_size *= 2;
                ans = realloc(ans, ans_size*sizeof(char*));
            }
            while (buf[i++] != '"');
            while (buf[j--] != '"');
            buf[j+1] = '\0';
            ans[error_count] = malloc(strlen(buf+i)+1); //список неудачных запросов
            strcpy(ans[error_count++], buf+i);
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
        c++; //добавляем время в окно t, увеличиаем счетчик на 1
        while (t > window && lp+1 < n) { //пока окно t больше нужного окна window, убираем время с левого указателя, уменьшаем счетчик
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
    printf("  _      _______  _____  ____ _      __\n");
    printf(" | | /| / /  _/ |/ / _ \\/ __ \\ | /| / /\n");
    printf(" | |/ |/ // //    / // / /_/ / |/ |/ / \n");
    printf(" |__/|__/___/_/|_/____/\\____/|__/|__/  \n");
    printf("\nmax window of length %lld [t; t+%lld] in secs:\n%lld to %lld\nMax requests in window: %lld at lines: %lld-%lld\n", window, window, lt, rt, mx, line_start+1, line_end+1);

    printf("    __________  ____  ____  ____     _______  ___  __\n");
    printf("   / ____/ __ \\/ __ \\/ __ \\/ __ \\   / ____/ |/ / |/ /\n");
    printf("  / __/ / /_/ / /_/ / / / / /_/ /  /___ \\ |   /|   / \n");
    printf(" / /___/ _, _/ _, _/ /_/ / _, _/  ____/ //   |/   |  \n");
    printf("/_____/_/ |_/_/ |_|\\____/_/ |_|  /_____//_/|_/_/|_|  \n");

    printf("\n5xx count: %lld\n\n", error_count);

    if (error_count) {
        for (LL i = 0; i < error_count; i++) {
            printf("%s\n", ans[i]);
        }
    }
    return 0;
}
