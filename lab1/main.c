#include <stdio.h>
#include <string.h>

size_t count_lines(char *filename) {
    FILE *fp;
    size_t cnt = 0;
    if ((fp = fopen(filename, "r")) == NULL) {
        return -1;
    }
    int current, last = EOF;
    current = fgetc(fp);
    while (current != EOF) {
        last = current;
        if (current == '\n') {
            cnt++;
        }
        current = fgetc(fp);
    }
    if (last != '\n') {
        cnt++;
    }
    fclose(fp);
    return cnt;
}

size_t count_file_size(char *filename) {
    FILE *fp;
    size_t cnt = 0;
    if ((fp = fopen(filename, "r")) == NULL) {
        return -1;
    }
    while (getc(fp) != EOF) {
        cnt++;
    }
    fclose(fp);
    return cnt;
}

size_t count_words(char *filename) {
    FILE *fp;
    size_t cnt = 0;
    if ((fp = fopen(filename, "r")) == NULL) {
        return -1;
    }
    int current, last = EOF;
    current = fgetc(fp);
    while (current != EOF) {
        last = current;
        if (current == ' ' || current == '\n') {
            cnt++;
            int help = fgetc(fp);
            while (help == ' ' || help == '\n') {
                help = fgetc(fp);
            }
        }
        current = fgetc(fp);
    }
    fclose(fp);
    if (last != '\n') {
        cnt++;
    }
    return cnt;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Format input: \"WordCount.exe [OPTION] filename\"");
        return 0;
    }

    if (strcmp(argv[1], "-l") == 0 || strcmp(argv[1], "--lines") == 0) {
        printf("file lines: %zu\n", count_lines(argv[2]));
    } else if (strcmp(argv[1], "-c") == 0 || strcmp(argv[1], "--bytes") == 0) {
        printf("file size: %zu\n", count_file_size(argv[2]));
    } else if (strcmp(argv[1], "-w") == 0 || strcmp(argv[1], "--words") == 0) {
        printf("file words: %zu\n", count_words(argv[2]));
    } else {
        printf("Commands:\n"
               "-l / --lines: number of lines\n"
               "-c / --bytes: size of file\n"
               "-w / --words: number of words\n");
    }

    return 0;
}