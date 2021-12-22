#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct List {
    struct List *prev;
    struct List *next;
    time_t t;
} List;

List *first = NULL;
List *last = NULL;

int add_last(time_t t)
{
    List *list = malloc(sizeof(List));
    if (list == NULL)
        return -1;

    list->prev = last;
    list->next = NULL;
    list->t = t;

    if (first == NULL)
        first = list;
    if (last != NULL)
        last->next = list;
    last = list;

    return 0;
}

int del_first()
{
    List *list = first;

    if (first != NULL)
        first = first->next;
    if (first != NULL)
        first->prev = NULL;
    if (last == list)
        last = NULL;
    if (list != NULL)
        free(list);
    return 0;
}

int list_count(List *list1, List *list2)
{
    int count = 0;
    List *list = list1;
    while (list != NULL)
    {
        count++;
        if (list == list2)
            break;
        list = list->next;
    }
//    printf("count %d\n", count);
    return count;
}

void list_clean()
{
    while(first != NULL)
        del_first();
}

void print_5xx(FILE *file)
{
    int line = 0;
    char s[500];
    char *p;
    int count = 0;

    while (!feof(file))
    {
        line++;
        if (fgets(s, 500, file) == NULL)
        {
            printf("fgets error in line %d\n", line);
            break;
        }

        // find: " 5xx ...
        p = strrchr(s, '"');
        if (p == NULL)
        {
            printf("error in line %d\n", line);
            break;
        }
        if ((p[1] == ' ') && (p[2] == '5'))
        {
            printf("%d %s\n", line, s);
            count++;
        }
    }
    printf("=======\n");
    printf("count 5XX: %d\n", count);
}

//[01/Jul/1995:00:00:01 -0400]
int read_time(char *s, time_t* t)
{
    char *p;
    char *q;
    char *months[12] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
    struct tm tm = {};

    // day
    p = strchr(s, '[');
    if (p == NULL)
        return -1;
    p++;
    q = strchr(p, '/');
    *q = '\0';
    tm.tm_mday = atoi(p);

    // month
    p = q+1;
    q = strchr(p, '/');
    *q = '\0';
    for (int i = 0; i < 12; i++)
    {
        if (strcmp(p, months[i]) == 0)
        {
            tm.tm_mon = i;
            break;
        }
    }

    // year
    p = q+1;
    q = strchr(p, ':');
    *q = '\0';
    tm.tm_year = atoi(p) - 1900;

    // hour
    p = q+1;
    q = strchr(p, ':');
    *q = '\0';
    tm.tm_hour = atoi(p);

    // minutes
    p = q+1;
    q = strchr(p, ':');
    *q = '\0';
    tm.tm_min = atoi(p);

    // seconds
    p = q+1;
    q = strchr(p, ' ');
    *q = '\0';
    tm.tm_sec = atoi(p);

//    printf("tm: %s\n", asctime(&tm));

    *t = mktime(&tm);
    return 0;
}

// w window in seconds
void find_max(FILE *file, int w)
{
    int line = 0;
    char s[500];

    time_t max_time = 0;
    int max_count = 0;
    time_t last_time = 0;

    time_t t; // time in current line

    while (!feof(file))
    {
        line++;
        //if ((line % 1000) == 0)
        //     printf("### %d ... max_count=%d %s\n", line, max_count, ctime(&max_time));
        if (fgets(s, 500, file) == NULL)
        {
            printf("fgets error in line %d\n", line);
            break;
        }

        if (read_time(s, &t) != 0)
        {
            printf("time error in line %d\n", line);
            break;
        }

        add_last(t);

        if (difftime(last->t, first->t) >= w)
        {
            int count = list_count(first, last->prev);
            if (count > max_count)
            {
                max_time = first->t;
                last_time = last->prev->t;
                max_count = count;
                //printf("... %d ... max_count=%d %s\n", line, max_count, ctime(&max_time));
            }
            del_first();
        }
    }

    if ((max_count == 0) && (first != NULL))
    {
        max_time = first->t;
        last_time = last->t;
        max_count = list_count(first, last);
    }

    list_clean();

    printf("=======\n");
    printf("max count %d\n%s\n", max_count, ctime(&max_time));
    printf("%s", ctime(&last_time));
}

// $remote_addr - - [$local_time] "$request" $status $bytes_send

int main(int argc, char *argv[])
{
    FILE *file = fopen(argv[1], "r");

    if (argc == 2)
        print_5xx(file);
    else if (argc == 3)
        find_max(file, atoi(argv[2]));
    else
        printf("Invalid args");


    fclose(file);
    return 0;
}
