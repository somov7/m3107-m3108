#include <stdio.h>
#include <string.h>

int main(int argc, char ** argv) {

    if (argc != 3) {
        printf("%s", "error (wrong everything)");
    }

    char* cmn;
    cmn = argv [1];

    char* filename;
    filename = argv[2];

    FILE *data;
    data = fopen(filename, "r");
    if (data == NULL) {
        printf ("%s", "error (there is no file!!)\n");
        return -1;
    }

    int num_lines = 1;
    int size_f = 0;
    int num_words = 0;
    int symbol = 5;
    int flag = 0;
    while (symbol != EOF)
    {
        symbol = fgetc(data);
        size_f++;
        if (symbol == '\n' || symbol == '\t' || symbol == '\v' || symbol == '\r'){
            num_lines++;
            flag = 0;
        } else {
            if (symbol == ' ') {
                flag = 0;
            } else {
                if (flag == 0) {
                    num_words++;
                }
                flag = 1;
            }
        }

    }
    fclose(data);
    size_f--;

    int cmnok = 0;

    if (strcmp(cmn, "-l") == 0 || strcmp(cmn, "--lines") == 0){
        printf("%d",num_lines);
        cmnok = 1;
    }
    if (strcmp(cmn, "-c") == 0 || strcmp(cmn, "--bytes") == 0){
        printf("%d",size_f);
        cmnok = 1;
    }
    if (strcmp(cmn, "-w") == 0 || strcmp(cmn, "--words") == 0){
        printf("%d",num_words);
        cmnok = 1;
    }

    if (cmnok != 1) {
        printf("%s", "error (wrong command i guess)");
    }

    return 0;
}
