#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>

int main(int argc, char **argv)
{

    FILE* fp = NULL;


    if (argc < 2)
    {
        perror("Something went wrong. Maybe you have entered wrong arguments");
        return -1;
    }

    int c;

    uint64_t time_win = 0;
    //getopt используется для анализа аргументов командной строки
    while ((c = getopt(argc, argv, "t:f:")) != -1)
    {
        if (c == 't')
        {
            //atol: string -> long int
            time_win = atol(optarg);
        }
        else if (c == 'f')
        {
            fp = fopen(optarg, "r");
        }
        else
        {
            perror("Something went wrong. Maybe you have entered wrong arguments");
            //EXIT_FAILURE - макрос, который возвращает ОС код о неудачном завершении программы
            exit(EXIT_FAILURE);
        }

    }

    //Проверка на правильность введённого аргумента для временного окна
    if (time_win < 0)
    {
        perror("Bad argument for time window");
        exit(EXIT_FAILURE);
    }

    //Проверка на наличие файла
    if (fp == NULL)
    {
        perror("No such file in directory");
        return -1;
    }

    int req_cnt = 1;
    char remote_addr[2048];
    char request[2048];
    int status;
    int bytes_send;
    int day, year, h, m, s;
    char mon[2];
    size_t size = 4048, iterator = 0;
    time_t *times = malloc(size * sizeof(time_t));
    char *mons[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    while (!feof(fp))
    {
        int read = fscanf(fp, "%s%*[-- ][%d/%3s/%d:%d:%d:%d%*[^]]]%*[ \"]%[^\"]\" %d %d", remote_addr,  &day, mon, &year,
                          &h, &m, &s, request,
                          &status, &bytes_send);

        //Проверка статуса: если от 500 до 599, то выводим
        if ((status / 100) == 5)
        {
            //printf("%d %s\n", i++, buff);
            printf("%d %s %s %d %d\n", req_cnt++, remote_addr, request, status, bytes_send);
        }

        //Работа со структурой tm из time.h для удобства работы с временем
        struct tm time = {0};
        time.tm_year = year - 1900;
        time.tm_mday = day;
        time.tm_hour = h;
        time.tm_min = m;
        time.tm_sec = s;
        //Цикл для нахождения правильного месяца
        for (int i = 0; i < 11; i++)
        {
            if (strcmp(mons[i], mon) == 0)
            {
                time.tm_mon = i;
                break;
            }
        }

        //
        if (iterator >= size)
        {
            size *= 2;
            times = realloc(times, size * sizeof(time_t));
        }

        times[iterator++] = mktime(&time);

    }

    uint64_t max = 0;
    uint64_t lenght = 0;
    int k = 0;

    //Для поиска максимального кол-ва запросов используем метод двух указателей
    for (int j = 0; j < iterator; ++j)
    {
        while (k < iterator && times[j] - times[k] > time_win)
        {
            k++;
            lenght--;
        }
        lenght++;
        max = lenght > max ? lenght : max;
    }

    printf("\n%llu\n", max);
    return 0;
}
