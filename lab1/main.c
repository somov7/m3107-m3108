#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void ShowUsage(char *arg) {
    printf("Usage: %s <option(s)>\n", arg);
    printf("\t-h, --help\t\tShow this help message\n");
    printf("\t-c, --bytes\t\tOutput of the file size in bytes\n");
    printf("\t-l, --lines\t\tOutput the number of rows\n");
    printf("\t-w, --words\t\tOutput of the number of words\n");

}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Error: Invalid number of arguments\n");
        ShowUsage(argv[0]);
        return 2;
    } else {

        char *cmd = argv[1];
        if (strcmp(cmd, "-h") == 0 || strcmp(cmd, "--help") == 0) {
            ShowUsage(argv[0]);
            return 0;
        }

        FILE *TextFile;
        TextFile = fopen(argv[2], "r");
        if (TextFile == NULL) {
            printf("Error: Error occurred while opening a file");
            return 1;
        }

        int lines = 1, chars = 0, words = 0;
        int TempChar = fgetc(TextFile), LastChar;
        int WFlag = 1;

        if (!((TempChar == ' ') || (TempChar == '\t') || (TempChar == '\n')))
            words = 1;
        if (TempChar == EOF)
            words = 0;

        while (TempChar != EOF){

            if ((TempChar == ' ') || (TempChar == '\t') || (TempChar == '\n')) {
                if (WFlag) {
                    words++;
                    WFlag = 0;
                }
                if (TempChar == '\n')
                    lines++;
            }
            else WFlag = 1;

            chars++;
            LastChar = TempChar;
            TempChar = fgetc(TextFile);
        }

        if ((LastChar == ' ') || (LastChar == '\t') || (LastChar == '\n'))
            words--;

        if (strcmp(cmd, "-l") == 0 || strcmp(cmd, "--lines") == 0) {
            printf("%d", lines);
        } else if (strcmp(cmd, "-c") == 0 || strcmp(cmd, "--bytes") == 0) {
            printf("%d", chars);
        } else if (strcmp(cmd, "-w") == 0 || strcmp(cmd, "--words") == 0) {
            printf("%d", words);
        } else{
            printf("Error: This command does not exist\n");
            ShowUsage(argv[0]);
            return 3;
        }
    }
    return 0;
}