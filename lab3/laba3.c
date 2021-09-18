#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int tosec(char* str, char* t)
{
    if (!strcmp(t, "sec")) return atoi(str);
    else if (!strcmp(t, "min")) return 60*atoi(str);
    else if (!strcmp(t, "hour")) return 60*60*atoi(str);
    else if (!strcmp(t, "day")) return 24*60*60*(atoi(str)-1);
}

int main(int argc, char* argv[])
{
    FILE* fp = fopen("test.txt", "r"); //название файла
    if (!fp)
    {
        printf("Can't open file, check name/path\n");
        return 1;
    }
    char buf[1000];
    int c = 0;
    char* ans[100]; //неудачных запросов 5хх в файле менее 100
    char* buf2[3];
    int error_count = 0;
    //int window = argv[1];
    int window;
    printf("Enter time window length in seconds.\n");
    scanf("%d", &window);
    const int lines = 1891714; //всего строк в файле
    int* times = malloc(lines*sizeof(int)); //массив для хранения времени запроса в секундах для подсчета максимального количества запросов в промежутке [a; a+t]
    int n = 0;
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
            ans[error_count] = malloc(strlen(buf2[1])+1); //список неудачных запросов
            strcpy(ans[error_count++], buf2[1]);
        }
        split = strtok(buf2[0], "[]");
        split = strtok(NULL, "[]");
        char* temp = split;
        split = strtok(temp, "/: ");
        int t = 0;
        t += tosec(split, "day");
        c = 0;
        while (split != NULL) //для вычленения времени используем токенайзер strtok, переводим все в секунды, годы и месяцы в файле совпадают
        {
            c++;
            //printf("c: %d str: %s\n", c, split);
            if (c == 4) t += tosec(split, "hour");
            else if (c == 5) t += tosec(split, "min");
            else if (c == 6) t += tosec(split, "sec");
            split = strtok(NULL, "/: ");
        }
        times[n++] = t;
        //printf("time: %d\n", t);
        for (int i = 0; i < 3; i++)
            free(buf2[i]);

    }
    int lp = 0, rp = 0;
    int t = times[0], mx = 0;
    int lt = times[0], rt = times[0];
    c = 1;
    int tl = 0, tr = 0;
    for (int i = 1; i < lines; i++) //метод двух указателей, sliding window 
    {
        t += times[i] - times[i-1];
        c++;
        while (t > window)
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
    printf("max window of length %d in secs:\n%d to %d\nMax requests in window: %d at lines: %d-%d\n", window, lt, rt, mx, tl+1, tr+1);
    printf("5xx count: %d", error_count);
    return 0;
}