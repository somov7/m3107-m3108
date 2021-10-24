#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int i;
    int l = 0;
    int c = 0;
    int w = 0;
    long num_of_lines = 0;
    long num_of_bytes = 0;
    long num_of_words = 0;
    int is_word = 0;
    int chr;
    char *file_name;
    FILE *file;

    if (argc < 3 || argc > 5){
        printf("wrong number of parameters: argc = %d\n", argc);
        puts("3 <= argc <= 5");
        return 1;
    }

    for (i = 1; i < argc - 1; i++){

        if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--lines") == 0){
            switch (l) {
                case 1:
                    puts("this parameter has already been entered:");
                    puts(argv[i]);
                case 0:
                    l = 1;
            }
        }

        else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--bytes") == 0){
            switch (c) {
                case 1:
                    puts("this parameter has already been entered:");
                    puts(argv[i]);
                case 0:
                    c = 1;
            }
        }

        else if (strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "--words") == 0){
            switch (w) {
                case 1:
                    puts("this parameter has already been entered:");
                    puts(argv[i]);
                case 0:
                    w = 1;
            }
        }
    }

    //необходимые флаги подняты

    file_name = argv[argc - 1];
    file = fopen(file_name, "rb");
    if (file == NULL){
        printf("can not open file %s\n", file_name);
        return 1;
    }

    if (l == 0 && c == 0 && w == 0){
        puts("no valid options");
        return 1;
    }

    chr = fgetc(file);

    while (chr != EOF) {

        if (chr == '\n'){
            num_of_lines += 1;
        }

        if ((chr == '\t' || chr == '\n' || chr == '\r' || chr == ' ') && is_word == 1){
            num_of_words ++;
            is_word = 0;
        }
        else{
            is_word = 1;
        }

        num_of_bytes += 1;

        chr = fgetc(file);
    }

    fclose(file);

    if (l != 0) printf("number of lines = %ld\n", num_of_lines);
    if (c != 0) printf("number of bytes = %ld\n", num_of_bytes);
    if (w != 0) printf("number of words = %ld\n", num_of_words);

    return 0;
}