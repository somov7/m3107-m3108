
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    
    int word_count = 0;
    int string_count = 0;
    int bytes_count = 0;

    char ch;
    FILE* file;
    file = fopen(argv[2], "r");

    if (argc > 3 || file == NULL || argc == 1)
    {
        return 1;
    }
    else if (strcmp(argv[1], "-w") == 0 || strcmp(argv[1], "-words") == 0) 
    {
        char start = ' ';
        while ( (ch = getc(file)) != EOF )
        {
            if (!isspace(ch) && start == ' ') {
                start = ch;
            }
            else if (isspace(ch) && start != ' ') {
                start = ' ';
                word_count++;
            }
        }
        if (start != ' ')
            word_count++;
        printf("%d", word_count);
    }
    else if (strcmp(argv[1], "-l") == 0 || strcmp(argv[1], "-lines") == 0)
    {
        while ((ch = getc(file)) != EOF) {
            if (ch == '\n') {
                string_count++;
            }
        }
        if (ch == EOF)
            string_count++;
        printf("%d", string_count);
    }
    else if (strcmp(argv[1], "-c") == 0 || strcmp(argv[1], "-bytes") == 0)
    {
        while ((ch = getc(file)) != EOF){
            bytes_count++;
        }
        bytes_count++;
        printf("%d", bytes_count);
    }
    else
    {
        fclose(file);
        return 1;
    }

    fclose(file);
    return 0;
}