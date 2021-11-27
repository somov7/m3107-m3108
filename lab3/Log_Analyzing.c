//Made in Visual Studio 2019

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

int main() 
{
    char ref[54];
    char time_of_request[27];
    char data[2048];
    int result;
    char data_weight[4];
    int ErrorCount = 0;

    FILE *foe = fopen("access_log_Jul95", "r");

    if (!foe) 
    {
        printf("%s", "File did not open.");
        return 1;
    }

    fseek(foe, 0, SEEK_SET);

    printf("Counting requests with 5xx error (server errors)...\n\n");

    while (!feof(foe)) 
    {
        fscanf(foe, "%s - - [%[^]]] \"%[^\"]\" %d %d", ref, time_of_request, data, &result, &data_weight);
        if ((result / 100) == 5)
        {
            ErrorCount++;
            printf("%2d)  %s\n", ErrorCount, data);
        }
    }

    printf("\nQuantity of requests with 5xx error (server errors): %d\n", ErrorCount);
    return 0;
}