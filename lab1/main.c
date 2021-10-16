#include <stdio.h>
#include <string.h>

size_t count_lines(FILE *fp) {
    size_t cnt = 1;
    int current;
    current = fgetc(fp);
    while (current != EOF) {
        if (current == '\n') {
            cnt++;
        }
        current = fgetc(fp);
    }
    fclose(fp);
    return cnt;
}

size_t count_file_size(FILE *fp) {
    size_t cnt = 0;
    while (getc(fp) != EOF) {
        cnt++;
    }
    fclose(fp);
    return cnt;
}

size_t count_words(FILE *fp) {
    size_t cnt = 0;
    int current;
    current = fgetc(fp);
    while (current != EOF) {
        if (current != ' ' && current != '\n') {
            int help = fgetc(fp);
            while (help != ' ' && help != '\n' && help != EOF) {
                help = fgetc(fp);
            }
            cnt++;
        }
        current = fgetc(fp);
    }
    fclose(fp);
    return cnt;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Format input: \"WordCount.exe [OPTION] filename\"");
        return -1;
    }
    FILE *fp;
    if ((fp = fopen(argv[2], "r")) == NULL) {
        return -1;
    }
    if (strcmp(argv[1], "-l") == 0 || strcmp(argv[1], "--lines") == 0) {
        printf("file lines: %zu\n", count_lines(fp));
    } else if (strcmp(argv[1], "-c") == 0 || strcmp(argv[1], "--bytes") == 0) {
        printf("file size: %zu\n", count_file_size(fp));
    } else if (strcmp(argv[1], "-w") == 0 || strcmp(argv[1], "--words") == 0) {
        printf("file words: %zu\n", count_words(fp));
    } else {
        printf("Commands:\n"
               "-l / --lines: number of lines\n"
               "-c / --bytes: size of file\n"
               "-w / --words: number of words\n");
        return -1;
    }

    return 0;
}