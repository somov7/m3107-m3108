#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get()
{

}

int set()
{

}

int show()
{

}

int main(int argc, char *argv[])
{
    FILE *foe;

    char *path = NULL, *flagOld = NULL, *flagNew = NULL;

    if (!argv[1] || !argv[2])
    {
        printf("Missing arguments\n");
        return -1;
    }

    if (strstr(argv[1], "--filepath"))
    {
        char *getPath = strtok(argv[1], "=");
        getPath = strtok(NULL, "=");
        path = (char *) malloc(strlen(getPath));
        strcpy(path, getPath);
    }
    else
    {
        printf("Command error: --filepath not found or is in the wrong place\n");
        return -1;
    }

    short command = 0;
    //1 = get, 2 = set, 3 = show

    if (strstr(argv[2], "--get"))
    {
        char *getTheFlag = strtok(argv[2], "=");
        getTheFlag = strtok(NULL, "=");
        flagOld = (char *) malloc(strlen(getTheFlag));
        strcpy(flagOld, getTheFlag);
        command = 1;
    }
    else if (strstr(argv[2], "--set") && strstr(argv[3], "--value"))
    {
        char *getValue;
        getValue = strtok(NULL, "=");

        flagOld = (char *) malloc(strlen(getValue));
        flagNew = (char *) malloc(strlen(getValue));

        strcpy(flagOld, getValue);
        strcpy(flagNew, getValue);

        printf("Set value of %s to %s\n", flagOld, flagNew);
        command = 2;
    }
    else if (strstr(argv[2], "--show"))
        command = 3;
    else
    {
        printf("Command error: unknown command\n");
        return 1;
    }

    foe = fopen(path, "rb");

    if (!foe)
    {
        printf("File opening failure\n");
        return -1;
    }

    switch (command)
    {
        case 1:
            get();
            break;
        case 2:
            set();
            break;
        case 3:
            show();
            break;
    }

    //in development

    fclose(foe);

    system("pause");
    return 0;
}