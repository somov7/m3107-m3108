#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char **argv) {

    if (argc != 3) {
        printf("Wrong number of arguments. Must be three - executed program name, command and filename.");
        return -1;
    }

    FILE *current = fopen(argv[2], "rb");
    if (!current) {
        printf("File couldn't be opened");
        return -1;
    }

    if (!strcmp("-l", argv[1]) || !strcmp("-L", argv[1]) || !strcmp("--lines", argv[1])) {
        getc(current);
        if (feof(current)) {
            printf("This file has 0 lines");
            return 0;
        }
        fseek(current, 0, SEEK_SET);

        int line_count = 1;
        int now;
        while (!feof(current)) {
            now = getc(current);
            if (now == '\n') line_count++;
        }
        if (line_count == 1) printf("This file has 1 line");
        else printf("This file has %d lines", line_count);
        fclose(current);
        return 0;
    }

    if (!strcmp("-b", argv[1]) || !strcmp("-B", argv[1]) || !strcmp("--bytes", argv[1])) {
        int byte_count = 0;
        while (getc(current)) {
            if (feof(current)) break;
            byte_count++;
        }
        if (byte_count == 1) printf("This file has 1 byte");
        else printf("This file has %d bytes", byte_count);
        fclose(current);
        return 0;
    }

    if (!strcmp("-w", argv[1]) || !strcmp("-W", argv[1]) || !strcmp("--words", argv[1])) {
        int word_count = 0;
        int now, prev = ' ';

        while (!feof(current)) {
            now = getc(current);
            if ((isgraph(now)) && (!isgraph(prev)))
                word_count++;
            prev = now;
        }

        if (word_count == 1) printf("This file has 1 word");
        else printf("This file has %d words", word_count);
        fclose(current);
        return 0;
    }

    printf("Wrong command");
    fclose(current);
    return -1;
}
