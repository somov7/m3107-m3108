#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>


time_t timeinstr(char* str){
    time_t time = 0;

    for (int i = 0; i < strlen(str); i++){
        if (str[i] == '['){

            time += (str[i + 1] - '0') * 10 * 24 * 60 * 60;
            time += (str[i + 2] - '0') * 24 * 60 * 60;

            int month; //это месяца которые прошли с начала года видимо
            if (str[i + 4] == 'J' && str[i + 5] == 'a') {
                month = 31;
            } else if (str[i + 4] == 'F') {
                month = 59; // в високосных годах 60, но я не буду это проверять(
            } else if (str[i + 4] == 'M' && str[i + 6] == 'r') {
                month = 90;
            } else if (str[i + 4] == 'A' && str[i + 5] == 'p') {
                month = 120;
            } else if (str[i + 4] == 'M') {
                month = 151;
            } else if (str[i + 4] == 'J' && str[i + 6] == 'n') {
                month = 181;
            } else if (str[i + 4] == 'J') {
                month = 212;
            } else if (str[i + 4] == 'A') {
                month = 243;
            } else if (str[i + 4] == 'S') {
                month = 273;
            } else if (str[i + 4] == 'O') {
                month = 304;
            } else if (str[i + 4] == 'N') {
                month = 334;
            } else {
                month = 365;
            }

            time += month * 24 * 60 * 60;

            int yeardif = (str[i + 8] - '0') * 1000 + (str[i + 9] - '0') * 100 + (str[i + 10] - '0') * 10 + (str[i + 11] - '0');
            yeardif -= 1995; //я буду считать с 1995
            time += yeardif * 365 * 24 * 60 * 60 * 60;

            time += str[i + 13] * 10 * 60 * 60 + str[i + 14] * 60 * 60; // hours
            time += str[i + 16] * 10 * 60 + str[i + 17] * 60;
            time += str[i + 19] * 10 + str[i + 20];

            return time;
        }
    }
    return -1;
}

typedef struct List {
    time_t *arr;
    size_t size;
    size_t i;
    size_t last;
}List;

List init(size_t start_size) {
    List list = {
            malloc(start_size*sizeof(time_t)),
            start_size,
            0,
            0
    };
    return list;
}

void add(List *list, time_t elem) {
    if (list->i == list->size) {
        list->size *= 2;
        list->arr = realloc(list->arr, list->size);
    }
    list->arr[list->i++] = elem;
}

char is_empty(List *list) {
    return list->i <= list->last;
}
time_t pop_last(List *list) {
    if (is_empty(list)) {
        return -1;
    }
    return list->arr[list->last++];
}

time_t peek_last(List *list) {
    return list->arr[list->last];
}

void detete(List *list) {
    free(list->arr);
}

int main() {

    FILE *data;
    data = fopen("testdata.in.txt", "r");

    uint64_t time_win = 0; // размер временного окна в секундах
    scanf("%lld", &time_win);

    if (time_win < 0) {
        perror("Bad arg");
        return -1;
    }

    FILE *errors;
    errors = fopen("data.out", "w");

    int numerrors = 0;
    int max = 0; // максимальное количество запросов
    char str[2048]; // статический буффер
    char temp[2048];

    int lines = 0;
    while (!feof(data)){
        fgets(temp, 2048, data);
        lines++;
    }
    fseek(data, 0, SEEK_SET);

    List list = init(lines);

    while (!feof(data)){
        fgets(str, 2048, data);

        if (strstr(str, "\" 5")){
            numerrors ++;
            fprintf(errors, "%s\n", str);
        }

        time_t cur = timeinstr(str);
        add(&list, cur);

        while (cur - peek_last(&list) > time_win) {
            pop_last(&list);
        }
        if (max < list.i - list.last) {
            max = list.i - list.last;
        }

    }
    detete(&list);
    printf("%d\n", max);
    printf("there are %d errors in the logs\n", numerrors);
    fclose(data);
}
