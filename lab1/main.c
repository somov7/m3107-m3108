#include <stdio.h>
#include <string.h>
#include <stdbool.h>


typedef struct Counts {
    int lines;
    int words;
    int bytes;
} Counts;

typedef struct CliArgs {
    char *filename;
    bool show_lines;
    bool show_bytes;
    bool show_words;
} CliArgs;

Counts count_data(FILE *fp) {
    Counts data = {0, 0, 0};
    int words_count = 0, lines_count = 0, characters_count = 0, n = 256, ch, reading_word = 0;
    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n') {
            lines_count++;
        }

        if (ch == ' ' || ch == '\t' || ch == '\n') {
            if (reading_word) {
                reading_word = 0;
                words_count++;
            }
        } else reading_word = 1;

        characters_count++;
    }

    words_count += reading_word;

    data.lines = lines_count;
    data.words = words_count;
    data.bytes = characters_count;

    return data;
}

CliArgs parse_cli(int argc, char *argv[]) {
    CliArgs cli_args = {.show_words=false, .show_bytes=false, .show_lines=false};
    if (argc <= 2) {
        cli_args.show_lines = true;
        cli_args.show_words = true;
        cli_args.show_bytes = true;
    } else {
        for (int i = 1; i < argc - 1; ++i) {
            if (strcmp(argv[i], "--lines") == 0 || strcmp(argv[i], "-l") == 0) {
                cli_args.show_lines = true;
            }
            if (strcmp(argv[i], "--bytes") == 0 || strcmp(argv[i], "-c") == 0) {
                cli_args.show_bytes = true;
            }
            if (strcmp(argv[i], "--words") == 0 || strcmp(argv[i], "-w") == 0) {
                cli_args.show_words = true;
            }
        }
    }

    cli_args.filename = argv[argc - 1];

    return cli_args;
}

void print_data(CliArgs args, Counts counts) {
    if (args.show_words) {
        printf("words = %d\n", counts.words);
    }
    if (args.show_bytes) {
        printf("bytes = %d\n", counts.bytes);
    }
    if (args.show_lines) {
        printf("lines = %d\n", counts.lines);
    }
}


int main(int argc, char *argv[]) {
    CliArgs args = parse_cli(argc, argv);
    FILE *fp;
    if ((fp = fopen(args.filename, "r")) == NULL) {
        perror("Error occured while opening file");
        return 1;
    }

    Counts counts = count_data(fp);;

    print_data(args, counts);

    return 1;
}
