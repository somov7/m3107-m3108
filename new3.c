#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
// char monthtoint[12][3] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
// int strtoint(char *month)
// {
//     int i = 0;
//     while ((strcmp(month, monthtoint[i]) != 0) && (i < 11))
//     {
//     // printf("% i \n", strcmp(month, monthtoint[i]));
//         i++;
//     }

//     // printf("% i", i);
//     return i;
// }
int strtoint(char *month)
{
    if (month[0] == 'J')
    {
        if (month[1] == 'u')
        {
            if (month[2] == 'n')
            {
                return 6;
            }
            else
            {
                return 7;
            }
        }
        else
        {
            return 1;
        }
    }
    else if (month[0] == 'M')
    {
        if (month[2] == 'r')
        {
            return 3;
        }
        else
        {
            return 5;
        }
    }
    else if (month[0] == 'A')
    {
        if (month[1] == 'p')
        {
            return 4;
        }
        else
        {
            return 8;
        }
    }
    else if (month[0] == 'F')
    {
        return 2;
    }
    else if (month[0] == 'S')
    {
        return 9;
    }
    else if (month[0] == 'O')
    {
        return 10;
    }
    else if (month[0] == 'N')
    {
        return 11;
    }
    else if (month[0] == 'D')
    {
        return 12;
    }
}
long long readingtime(FILE *fin)
{
    char symbol;
    struct tm *time;
    char month[3];
    while ((symbol != '[') && (symbol != EOF))
    {
        symbol = fgetc(fin);
    }
    time->tm_mday = (fgetc(fin) - 48) * 10 + fgetc(fin) - 48;
    symbol = fgetc(fin);
    month[0] = fgetc(fin);
    month[1] = fgetc(fin);
    month[2] = fgetc(fin);
    time->tm_mon = strtoint(month)-1;
    symbol = fgetc(fin);
    time->tm_year = (fgetc(fin) - 48) * 1000 + (fgetc(fin) - 48) * 100 + (fgetc(fin) - 48) * 10 + (fgetc(fin) - 48) - 1900;
    symbol = fgetc(fin);
    time->tm_hour = (fgetc(fin) - 48) * 10 + fgetc(fin) - 48;
    symbol = fgetc(fin);
    time->tm_min = (fgetc(fin) - 48) * 10 + fgetc(fin) - 48;
    symbol = fgetc(fin);
    time->tm_sec = (fgetc(fin) - 48) * 10 + fgetc(fin) - 48;
    return mktime(time);
}
int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "% s", "ERROR: invalid number of arguments");
        return 1;
    }
    FILE *fin;
    fin = fopen(argv[1], "r");
    FILE *fin1;
    fin1 = fopen(argv[1], "r");
    if (fin == NULL)
    {
        fprintf(stderr, "% s", "ERROR: the file does not exist");
        return 1;
    }
    long long max = 0;
    long long timemaxleft, timemaxright;
    long long timeleft, timeright;
    long long counter = 0, errorcounter = 0;
    struct tm *time;
    long long timewindow = atoi(argv[2]);
    char symbol;
    char request[500];
    long long requestlength;
    symbol = fgetc(fin);
    while (symbol != EOF)
    {
        requestlength = -1;
        if (counter == 0)
        {
            timeleft = readingtime(fin1);
            timemaxleft = timeleft;
        }
        timeright = readingtime(fin);
        if (timeright >= timeleft + timewindow)
        {
            if (counter > max)
            {
                max = counter;
                timemaxleft = timeleft;
            }
            timeleft = readingtime(fin1);
        }
        else
        {
            counter++;
        }
        while ((symbol != '"') && (symbol != EOF))
        {
            symbol = fgetc(fin);
        }
        symbol = fgetc(fin);
        while ((symbol != '"') && (symbol != EOF))
        {
            requestlength++;
            request[requestlength] = symbol;
            symbol = fgetc(fin);
        }
        symbol = fgetc(fin);
        symbol = fgetc(fin);
        if ((symbol != '1') && (symbol != '2'))
        {
            errorcounter++;
            if (symbol == '5')
            {
                for (int i = 0; i <= requestlength; i++)
                {
                    printf("% c", request[i]);
                    if (i == requestlength)
                    {
                        printf("\n");
                    }
                }
            }
        }
        while ((symbol != '\n') && (symbol != EOF))
        {
            symbol = fgetc(fin);
        }
        symbol = fgetc(fin);
    }
    if ((timeright < timemaxleft + timewindow) && (timeleft == timemaxleft))
    {
        max = counter;
        timemaxleft = timeleft;
    }
    timemaxright = timemaxleft + timewindow - 1;
    printf("%i requests with error \n", errorcounter);
    printf("maximum number of requests within% i seconds:", timewindow);
    printf("% lli \n", max);
    time = localtime(&timemaxleft);
    printf("from:\n% s", asctime(time));
    time = localtime(&timemaxright);
    printf("to:\n% s\n", asctime(time));
    return 0;
}