#include <stdio.h>
#include <string.h>

int main(int argc, char ** argv) {

    if (argc != 3) {
        printf("%s", "error (wrong everything)");
        return -1;
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
    int symbol = fgetc(data);
    int is_letter = 0;
    while (symbol != EOF)
    {
        size_f++;
        if (symbol == '\n'){
            num_lines++;
            is_letter = 0;
        } else if (symbol == ' '|| symbol == '\t' || symbol == '\v' || symbol == '\r') {
            is_letter = 0;
        } else {
            if (is_letter == 0) {
                num_words++;
            }
            is_letter = 1;
        }
        symbol = fgetc(data);
    }


    fclose(data);

    if (strcmp(cmn, "-l") == 0 || strcmp(cmn, "--lines") == 0){
        printf("%d",num_lines);
    } else if (strcmp(cmn, "-c") == 0 || strcmp(cmn, "--bytes") == 0){
        printf("%d",size_f);
    } else if (strcmp(cmn, "-w") == 0 || strcmp(cmn, "--words") == 0){
        printf("%d",num_words);
    } else {
        printf ("%s", "error (wrong command i guess)\n");
        return -1;
    }


    return 0;
}
