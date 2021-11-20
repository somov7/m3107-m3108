#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
int MonthNum (char *month_name) {
    if (strcmp(month_name, "Jan") == 0) return 0;
    else if (strcmp(month_name, "Feb") == 0) return 1;
    else if (strcmp(month_name, "Mar") == 0) return 2;
    else if (strcmp(month_name, "Apr") == 0) return 3;
    else if (strcmp(month_name, "May") == 0) return 4;
    else if (strcmp(month_name, "Jun") == 0) return 5;
    else if (strcmp(month_name, "Jul") == 0) return 6;
    else if (strcmp(month_name, "Aug") == 0) return 7;
    else if (strcmp(month_name, "Sep") == 0) return 8;
    else if (strcmp(month_name, "Oct") == 0) return 9;
    else if (strcmp(month_name, "Nov") == 0) return 10;
    return 11;
}
long change_time (FILE *in_file) {
    struct tm m_time;
    char temp_arr_month[4], current_symbol = '\0';
    long long result_time = 0;
    while ((current_symbol != '[') && (current_symbol != EOF)) current_symbol = fgetc(in_file);
    m_time.tm_mday = (fgetc(in_file) - 48) * 10 + (fgetc(in_file) - 48);
    current_symbol = fgetc(in_file);
    for (int i = 0; i < 3; i++) {
        temp_arr_month[i] = fgetc(in_file);
    }
    m_time.tm_mon = MonthNum(temp_arr_month);
    current_symbol = fgetc(in_file);
    m_time.tm_year = (fgetc(in_file) - 48) * 1000 + (fgetc(in_file) - 48) * 100 + (fgetc(in_file) - 48) * 10 + (fgetc(in_file) - 48) - 1900;
    current_symbol = fgetc(in_file);
    m_time.tm_hour = (fgetc(in_file) - 48) * 10 + (fgetc(in_file) - 48);
    current_symbol = fgetc(in_file);
    m_time.tm_min = (fgetc(in_file) - 48) * 10 + (fgetc(in_file) - 48);
    current_symbol = fgetc(in_file);
    m_time.tm_sec = (fgetc(in_file) - 48) * 10 + (fgetc(in_file) - 48);
    result_time = mktime(&m_time);
    return result_time;
}

int main(int argc, const char * argv[]) {
    FILE *fin, *fin2;
    long int N = 2048, counter_current_request = 0;
    char current_symbol, current_request [N], current_symbol2;
    long long first_time = 0, second_time = 0, counter_failure_requests = 0, counter = 0, counter_max = 0;
    time_t max_range_time_first, max_range_time_second;
    struct tm *m_time;
//    fin2 = fopen ("/Users/artemparfenov/Downloads/access_log_Jul95-2", "r");
//    fin = fopen ("/Users/artemparfenov/Downloads/access_log_Jul95-2", "r");
//    fin2 = fopen ("/Users/artemparfenov/Downloads/access_log_Jul95 -> crash", "r");
//    fin = fopen ("/Users/artemparfenov/Downloads/access_log_Jul95 -> crash", "r");
    fin = fopen(argv[2], "r");
    fin2 = fopen(argv[2], "r");
    if (argc != 3) {
            fprintf(stderr, "Проверьте количество параметров\n");
            return EXIT_FAILURE;
        }
    if (argv[1] == NULL) {
            fprintf(stderr, "Проверьте входной файл");
            return EXIT_FAILURE;
        }
    int time_range = atoi(argv[2]);
//    int time_range = 10;
//    fin = fopen ("/Users/artemparfenov/Desktop/nasa -> txt", "r");
//    fin2 = fopen ("/Users/artemparfenov/Desktop/nasa -> txt", "r");
//    time_range = argv[1];
    current_symbol = fgetc(fin);
    current_symbol2 = fgetc(fin2);
    while (current_symbol != EOF) {
        
        if (counter == 0) {
            second_time = change_time(fin2);
            max_range_time_second = second_time;
        }
        first_time = change_time(fin);
//            if (second_time >= first_time + time_range)
        if (first_time - second_time >= time_range) {
            if (counter > counter_max) {
                counter_max = counter;
                max_range_time_second = second_time;
            }
            second_time = change_time(fin2);
        }
        else {
            counter++;
        }
        while ((current_symbol != '"') && (current_symbol != EOF)) current_symbol = fgetc(fin);
        current_symbol = fgetc(fin);
        while (((current_symbol != '"') || (current_request[counter_current_request - 1] != '0')) && (current_symbol != EOF)) {
            current_request[counter_current_request] = current_symbol;
            counter_current_request++;
            current_symbol = fgetc(fin);
        }
        current_symbol = fgetc(fin);
        current_symbol = fgetc(fin);
        if (current_symbol == '5') {
            for (int j = 0; j < counter_current_request; j++) printf ("%c", current_request[j]);
            printf("\n");
            counter_failure_requests++;
        }
        counter_current_request = 0;
        while (current_symbol != '\n' && current_symbol != EOF) current_symbol = fgetc(fin);
        
    }
    if ((first_time - max_range_time_second < time_range) && (second_time == max_range_time_second)) {
        counter_max = counter;
        max_range_time_second = second_time;
    }
    max_range_time_first = max_range_time_second + time_range - 1;
    printf ("Requests with error: %llu\n", counter_failure_requests);
    m_time = localtime(&max_range_time_second);
    printf ("From: %s\n", asctime(m_time));
//    printf ("To: \n%", );
    m_time = localtime(&max_range_time_first);
    printf ("To: %s\n", asctime(m_time));
    printf ("Max requests within %d seconds: %llu\n", time_range, counter_max);
    return 0;
}


