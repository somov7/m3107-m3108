#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "arc.h"


int main(int argc, char **argv)
{
    setlocale(LC_ALL, "Rus");

    if (argc < 2)
    {
        puts("Too few arguments for this program");
        exit(EXIT_FAILURE);
    }

    Arch arch;
    char *archName;

    for (int i = 1; i < argc;)
    {
        if (strcmp("--file", argv[i]) == 0)
        {
            archName = argv[++i];
        }

        else if (strcmp("--create", argv[i]) == 0)
        {
            i++;
            arch = init(archName);
            for (i; i < argc;)
            {
                create(&arch, argv[i++]);
            }
            close(&arch);
        }

        else if (strcmp("--list", argv[i]) == 0)
        {
            arch = open(archName);
            char **files = list(&arch);

            for (int j = 0; j < arch.header.fileCounter; j++)
            {
                printf("%d: %s\n", j + 1, files[j]);
            }
            close(&arch);
        }

        else if (strcmp("--extract", argv[i]) == 0)
        {
            Arch arch = open(archName);
            extract(&arch);
            close(&arch);
        }
        i++;
    }

    return 0;
}

