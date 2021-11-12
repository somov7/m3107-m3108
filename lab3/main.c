/* Orlov Aleksandr, 12-5, M3107,  29.10.2021 */

#include <stdio.h>
#include "nasa.h"

int main(void)
{
    FILE* f = fopen("access_log_Jul95", "r");
    long errors = 0, window;
    Timestamp* tail, * head;

    printf("Input window size in seconds: ");
    scanf("%d", &window);
    if (f != NULL)
    {
        parse(f, &errors, window);
        printf("Bad requests: %i", errors);
    }
    else
        printf("Problems with the file\n");
    return 0;
}