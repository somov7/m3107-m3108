/* Orlov Aleksandr, 12-5, M3107,  29.10.2021 */

#include <stdio.h>
#include <string.h>
#include "nasa.h"

int main(void)
{
    long errors = 0, window;
    Timestamp* tail, * head;
    char file[FILENAME_SIZE];

    printf("Input filename: ");
    scanf("%s", file);

    FILE* f = fopen(file, "r");

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