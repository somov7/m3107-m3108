#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#define REQ_LEN 2500
//=======================================ERROR
void error_5xx(FILE* file1, FILE* file2) {

    char string[REQ_LEN];
    unsigned int size_of_string = 0;
    int count_errors = 0;
    int first_element = 0;
    int last_element = 0;
    while (!feof(file1)) {
        char *check = fgets(string, REQ_LEN, file1);
        if (check == NULL) {

        }
        size_of_string = strlen(string);
        for (int i = 0; i < size_of_string; i++) {
            if (string[i] == '"') {
                first_element = i;
                break;
            }
        }
        for (int j = first_element + 1; j < size_of_string; j++) {
            if (string[j] == '"') {
                last_element = j;
                break;
            }
        }
        if (string[last_element + 2] == '5') {
            fprintf(file2, "%s", string + first_element + 1);
            count_errors = count_errors + 1;
        }
    }
    printf("%d", count_errors);

}
//=======================================TIME

/*char Get_TimeStr(FILE *file){
    int start_pos;
    char string[REQ_LEN];
    char sub_str[20];
    unsigned int size_of_string = 0;
    char *check = fgets(string, REQ_LEN , file);
    if (check == NULL) {
        return 1 ;
    }
    size_of_string = strlen(string);
    for (int i = 0; i < size_of_string; i++) {
        if (string[i] == '[') {
            start_pos = i;
            break;
        }
    }
    strncpy(sub_str,string[start_pos + 1], 20);
    return sub_str;
}*/


    time_t Get_time(char *string) {
        char month[12][4]= {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug",
                            "Sep", "Oct", "Nov", "Dec"};
        struct tm time;
        time.tm_mday = atoi(string);
        for (int i = 0; i < 11; i++)
            if (!strcmp(month[i], string)) {
                time.tm_mon = i;
                break;
            }
        time.tm_year = atoi(string + 7) - 1900;
        time.tm_hour = atoi(string + 12);
        time.tm_min = atoi(string + 15);
        time.tm_sec = atoi(string + 18);
        return mktime(&time);
    }

char *Time_str(char *string){

char *str = malloc(20);
int i = 0;
while (*(string+i) != '[' ){
    i++;
}
strncpy(str, string + i + 1, 20);

return str;
}


void Time_Window(FILE *file, long long sec_period) {
    long int first_pos = 0, sec_pos = 0, count = 0;
    fseek(file, 0, SEEK_END);
    char first_string[REQ_LEN], second_string[REQ_LEN];
    int from_end = ftell(file);

    while (sec_pos < from_end) {
        fseek(file, first_pos, SEEK_SET);
        fgets(first_string, REQ_LEN, file);
        char *data = Time_str(first_string);
        int firstTime = Get_time(data);
        first_pos = ftell(file);
        if (sec_pos > first_pos) {
            fseek(file, sec_pos, SEEK_SET);
        }
        while (sec_pos < from_end) {
            fgets(second_string, REQ_LEN, file);
            sec_pos = ftell(file);
            char *data1 = Time_str(second_string);
            int lastTime = Get_time(data1);
            if (difftime(lastTime, firstTime) > sec_period){
                break;
            }
            count++;
        }
    }
    printf("\n");
    printf("window : %d count : %d \n", sec_period, count - 2);
}
//=======================================Main_Program

int main(int argc, char** argv) {
        int period;
    FILE *log_in = fopen("123.txt", "r");
    error_5xx(log_in, stdout);
    printf(" \n ");
    printf("Write time period in seconds: ");
    scanf("%d", &period);
    Time_Window(log_in, period);
    return 0;
}