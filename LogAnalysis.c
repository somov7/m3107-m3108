#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "header.h"
#include <windows.h>

const int MAX_STRING_SIZE = 4096;
const short CODE = 3;
const short DATA_LENGTH = 28;
const short SINGLE_REQUEST = 8;
const short FAILURE_REQUEST = 3;

long long if_opened_file(FILE *variable, char *filename)
{
    (variable == NULL) ? (printf("\nSome kinda error occured during file opening")) : (printf("\n%s opened\nAnalyzing in process...\n[..........]\r[", filename));

    if (variable == NULL)
        return 0;

    long long string_counter = 0;

    char *string = (char *)(malloc(MAX_STRING_SIZE * sizeof(char)));

    while (!feof(variable))
    {
        fgets(string, MAX_STRING_SIZE, variable);

        string_counter++;
    }

    fclose(variable);
    free(string);
    variable = fopen(filename, "r");

    return string_counter;
}

void instruction_manual()
{
    printf("\n"
           " WWWW         WWWW   EEEEEEEE  LLL        CCCCCCC   OOOOO   MM     MM  EEEEEEEE\n"
           " WWWW   WWW   WWWW   EEE       LLL       CC        OO   OO  MMMM MMMM  EEE     \n"
           "  WWW  WW WW  WWW    EEEEEEEE  LLL       CC        OO   OO  MM MMM MM  EEEEEEEE\n"
           "   WWWWW   WWWWW     EEE       LLL       CC        OO   OO  MM     MM  EEE     \n"
           "    WWW     WWW      EEEEEEEE  LLLLLLLL   CCCCCCC   OOOOO   MM     MM  EEEEEEEE\n"

           "\n\nCall '5xx' to get the list and quantity of 5xx errors\n"
           "Call 'maximal window active status' and enter time range to get time window when "
           "the number of requests to the server was maximum\n"
           "example: [01/Jan/1970:02:21:34 -0100] to [02/Jan/1970:02:21:34 -0100] (half-interval)"
           "\nCall 'maximal window active status' to get the number of requests from the whole file\n"
           "example: [13:23:40 -0400], [01/Aug/1991]");

    exit(1);
}

void analyze_date(char *seek_from, char *seek_until, char **argv, int argc)
{

    strncat(argv[6], " ", 2);
    memcpy(seek_from, argc <= SINGLE_REQUEST ? (argv[6] + 1) : argv[6], argc > SINGLE_REQUEST ? ((int)strlen(argv[6]) - 1) : (int)strlen(argv[6]));
    strncat(seek_from, argv[7], (int)strlen(argv[7]));

    if (argc > SINGLE_REQUEST)
    {
        strncat(argv[9], " ", 2);
        memcpy(seek_until, argv[9], (int)strlen(argv[9]));
        strncat(seek_until, argv[10], (int)strlen(argv[10]));
    }

    if (argc <= SINGLE_REQUEST)
        seek_from = strtok(seek_from, "]");
}

void analyze_status(char *string_for_analysis, char *status_string, list *data_base)
{
    int left = 0, right = (int)(strlen(string_for_analysis) - 1);

    while (string_for_analysis[right--] != ' ')
        ;
    right++;
    left = right - 2;

    strncpy(status_string, string_for_analysis + left - 1, right - left + 1);

    if (status_string[0] == '5')
    {
        push(string_for_analysis, data_base);
    }
}

void analyze_local_time(char *string_for_analysis, char *seek_from, char *seek_until, list *data_base, bool *flag, int argc, char *previous_date)
{
    int left = 0;
    while (string_for_analysis[left++] != '[')
        if (left >= (int)(strlen(string_for_analysis)))
            return;

    left--;

    char *local_time_string = (char *)(calloc(DATA_LENGTH, sizeof(char)));

    strncpy(local_time_string, string_for_analysis + left, DATA_LENGTH);

    if (argc > SINGLE_REQUEST)
    {
        if (strcmp(local_time_string, seek_until) >= 0)
        {
            return;
        }

        if (strcmp(local_time_string, seek_from) == 0)
        {
            *flag = true;
        }

        if (flag)
            if (data_base->data_counter == 0 || strcmp(local_time_string, previous_date) != 0)
            {
                push(string_for_analysis, data_base);
            }
            else
            {
                if (strcmp(local_time_string, previous_date) == 0)
                    data_base->last_node->query_quantity++;
            }
    }
    else
    {
        char *substring = strstr(string_for_analysis, seek_from);

        if (substring != NULL)
        {
            if (data_base->data_counter == 0 || strcmp(local_time_string, previous_date) != 0)
            {
                push(string_for_analysis, data_base);
            }
            else
            {
                if (strcmp(local_time_string, previous_date) == 0)
                    data_base->last_node->query_quantity++;
            }
        }
    }

    memcpy(previous_date, local_time_string, (int)(strlen(local_time_string)));
}

int main(int argc, char **argv)
{
    SetConsoleOutputCP(CP_UTF8);

    if (argc == 1)
    {
        instruction_manual();
    }
    long long rows_quantity = 0;

    FILE *file_for_analysis = fopen(argv[1], "r");
    rows_quantity = if_opened_file(file_for_analysis, argv[1]);

    char *seek_from = (char *)(calloc(DATA_LENGTH, sizeof(char)));
    char *seek_until = (char *)(calloc(DATA_LENGTH, sizeof(char)));
    char *previous_date = (char *)(calloc(DATA_LENGTH, sizeof(char)));
    if (argc > FAILURE_REQUEST)
        analyze_date(seek_from, seek_until, argv, argc);

    list failures = {NULL, 0}, date = {NULL, 0};

    char *status = (char *)(calloc(CODE, sizeof(char)));

    bool can_be_executed = false;

    long long program_status = 0, previous_value = 0;

    while (!feof(file_for_analysis))
    {
        program_status++;

        char *current_string = (char *)(calloc(MAX_STRING_SIZE, sizeof(char)));

        fgets(current_string, MAX_STRING_SIZE, file_for_analysis);

        if (argc == FAILURE_REQUEST)
            analyze_status(current_string, status, &failures);
        else
            analyze_local_time(current_string, seek_from, seek_until, &date, &can_be_executed, argc, previous_date);
        if (program_status * 100 / rows_quantity % 10 == 0 && program_status * 100 / rows_quantity / 10 != 0 && previous_value != program_status * 100 / rows_quantity)
        {
            printf("▊");
            previous_value = program_status * 100 / rows_quantity;
        }
    }
    printf("] 100%%\n");
    long long max_queries = 0;
    node *temp = date.last_node;
    while (temp != NULL)
    {
        max_queries = max(max_queries, temp->query_quantity);
        temp = temp->next;
    }
    temp = NULL;

    if (strcmp(argv[2], "5xx") == 0)
    {
        printf("\nthere were %d errors\n", failures.data_counter);
        pop(&failures);
    }
    else
    {
        printf("Maximal window actitvity is %ld\nFound requests:\n", max_queries);
        pop(&date);
    }

    fclose(file_for_analysis);
    clear(&failures);
    clear(&date);
    free(seek_from);
    free(seek_until);

    return 0;
}