#include <stdio.h>
#include <string.h>
#include <ctype.h> 
#include <locale.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>

long long max_right;
long long max_left;
int queue_size = 0;
typedef struct queue {
    long long value;
    struct queue *next;
} queue_c;

queue_c *head = NULL;
queue_c *tail = NULL;

queue_c *push(long long x) {
    queue_c *ptr = malloc(sizeof(queue_c));
    if (head != NULL) {
        tail->next = ptr;
        ptr->value = x;
        ptr->next = NULL;
        tail = ptr;
    }
    else {
        ptr->value = x;
        ptr->next = NULL;
        tail = ptr;
        head = ptr;
    }
    queue_size++;
}

void pop() {
    if (head != NULL) {
        queue_c *temp = head->next;
        free(head);
        head = temp;
    }
    queue_size--;
}

/*long long gettime(FILE *fin) {
    struct tm *time;
    char month[3];
    char months[12][3] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    char current_symbol;
    time -> tm_mday = (fgetc(fin)-48) * 10 + fgetc(fin) - 48;
    current_symbol = fgetc(fin);
    month[0] = fgetc(fin);
    month[1] = fgetc(fin);
    month[2] = fgetc(fin);
    int temp = 0;
    int checker = 1;
    while (checker == 1) {
        if (months[temp][0] == month[0]) {
            if (months[temp][1] == month[1]) {
                if (months[temp][2] == month[2]) {
                    time -> tm_mon = temp;
                    checker = 0;
                }
                else 
                    temp++;
            }
            else
                temp++;
        }
        else 
            temp++;
    }
    current_symbol = fgetc(fin);
    time -> tm_year = (fgetc(fin) - 48) * 1000 + (fgetc(fin) - 48) * 100 + (fgetc(fin) - 48) * 10 + (fgetc(fin) - 48) - 1900;
    current_symbol = fgetc(fin);
    time -> tm_hour = (fgetc(fin) - 48) * 10 + (fgetc(fin) - 48);
    current_symbol = fgetc(fin);
    time -> tm_min = (fgetc(fin) - 48) * 10 + (fgetc(fin) - 48);
    current_symbol = fgetc(fin);
    time -> tm_sec = (fgetc(fin) - 48) * 10 + (fgetc(fin) - 48);
    printf("мк тайм %lli ", mktime(time));
    return mktime(time);
}*/

int main(int argc, char **argv) {
    setlocale(LC_ALL, "Rus");
    if (argc != 3) {
        fprintf(stderr, "Ошибка!\nНеверное количество параметров.\n");
        return 1;
    }
    FILE *fin;
    fin = fopen(argv[1], "r");
    if (fin == NULL) {
        fprintf(stderr, "Ошибка!\nФайл не открылся.\n");
        return 1;
    }
    long long sec = atoi(argv[2]);
    char request[400];
    char current_symbol;
    int request_length;
    int counter_errors = 0;
    int counter_errors_5xx = 0;
    long long temp_time1 = 0;
    long long temp_time2 = 0;
    long long max_requests = 0;
    long long current_requests = 0;
    struct tm *time;
    char month[3];
    char months[12][3] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    current_symbol = fgetc(fin);
    printf("Список неудачных запросов: \n");
    while (current_symbol != EOF) {
        current_symbol = fgetc(fin);
        while ((current_symbol != '[') && (current_symbol != EOF)) {
            current_symbol = fgetc(fin);
        }
        if (current_symbol == EOF) 
            break;
        time -> tm_mday = (fgetc(fin)-48) * 10 + fgetc(fin) - 48;
        current_symbol = fgetc(fin);
        month[0] = fgetc(fin);
        month[1] = fgetc(fin);
        month[2] = fgetc(fin);
        int temp = 0;
        int checker = 1;
        while (checker == 1) {
            if (months[temp][0] == month[0]) {
                if (months[temp][1] == month[1]) {
                    if (months[temp][2] == month[2]) {
                        time -> tm_mon = temp;
                        checker = 0;
                    }
                    else 
                        temp++;
                }
                else
                    temp++;
            }
            else 
                temp++;
        }
        current_symbol = fgetc(fin);
        time -> tm_year = (fgetc(fin) - 48) * 1000 + (fgetc(fin) - 48) * 100 + (fgetc(fin) - 48) * 10 + (fgetc(fin) - 48) - 1900;
        current_symbol = fgetc(fin);
        time -> tm_hour = (fgetc(fin) - 48) * 10 + (fgetc(fin) - 48);
        current_symbol = fgetc(fin);
        time -> tm_min = (fgetc(fin) - 48) * 10 + (fgetc(fin) - 48);
        current_symbol = fgetc(fin);
        time -> tm_sec = (fgetc(fin) - 48) * 10 + (fgetc(fin) - 48);
        temp_time1 = mktime(time);
        if (max_requests == 0) {
            push(temp_time1); 
            current_requests++; 
            temp_time2 = temp_time1;
        }
        else {
            if(temp_time1-temp_time2<=sec) {
                push(temp_time1); 
                current_requests++; 
            }
            else {
                while ((temp_time1-temp_time2>sec) && (queue_size!=0)) {
                    pop();
                    if (queue_size != 0) { 
                        temp_time2 = head -> value;
                    }
                    current_requests--;
                }
                push(temp_time1); 
                current_requests++; 
                if (queue_size==1) {
                    temp_time2 = temp_time1;
                }
            }
        }
        while ((current_symbol != '"') && (current_symbol != EOF)) {
            current_symbol = fgetc(fin);
        }
        if (current_symbol == EOF) 
            break;
        request_length = 0;
        current_symbol = fgetc(fin);
        while ((current_symbol != '"') && (current_symbol != EOF)) {
            request[request_length] = current_symbol;
            request_length++;
            current_symbol = fgetc(fin);
        }
        if (current_symbol == EOF) 
            break;
        for (int j = 0; j<2; j++) {
            current_symbol = fgetc(fin);
        }
        if ((current_symbol != '1') && (current_symbol != '2')) {
            if ((current_symbol == '5') || (current_symbol == '4')) 
                counter_errors++;
            if (current_symbol == '5') {
                counter_errors_5xx++;
                for (int i = 0; i<=request_length-1; i++) {
                    printf("%c", request[i]);
                    if (i == request_length-1) {
                        printf("\n");
                    }
                }
            }
        }
        while ((current_symbol != '\n') && (current_symbol != EOF)) {
            current_symbol = fgetc(fin);
        }
        if (current_requests > max_requests) {
            max_requests = current_requests;
            max_left = temp_time2;
            max_right = temp_time1;
        }
    }
    printf("%i запросов с ошибкой \n", counter_errors);
    printf("%i запросов с ошибкой 5xx \n", counter_errors_5xx);
    printf("максимальное количество запросов за % lli секунд:", sec);
    printf("% lli \n", max_requests);
    time = localtime(&max_left);
    printf("c:\n %s", asctime(time));
    time = localtime(&max_right);
    printf("по:\n %s", asctime(time));
    return 0;
}