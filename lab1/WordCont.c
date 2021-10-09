#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>


int main(int argc, char* argv[]) {

	
	if (argc < 2) {
		printf("Usage: WordCount.exe [OPTIONS] filename\n\nFor a list of options use: WordCount.exe -h\n\n\n\n\n\n\n ");
		return 1;
	}
	char* option = argv[1];


	FILE* input;
	char* name;
	name = argv[argc - 1];

	input = fopen(name, "r");

	if (input == NULL) {
		printf("File not be opened");
		return 1;
	}

	

    for (int l = 1; l <= argc - 2; l++) {
		char* options = argv[l];
		int ch;

		if ((strcmp(options, "-l") == 0) || (strcmp(options, "--lines") == 0)) {
			int CountLines = 0;
			int word = 0;
			while ((ch = fgetc(input)) != EOF) {
				if (ch == '\n') {
					CountLines++;
					word = 0;
				}
				else {
					if (ch != EOF) {
						word = 1;
					}
				}
			}
			fseek(input, 0, SEEK_SET);
			printf("Opened file contains %d lines\n", CountLines + word);

		}

		else if ((strcmp(options, "-c") == 0) || (strcmp(options, "--bytes") == 0)) {
			int size = -1;
			do {
				ch = fgetc(input);
				size++;
			} while (ch != EOF);
			fseek(input, 0, SEEK_SET);
			printf("File size is %d bytes\n", size);
		}

		
		else if ((strcmp(options, "-w") == 0) || (strcmp(options, "--words") == 0)) {
			int ch = 0;
			int isspace = 0;
			int words = 0;
			while (ch != EOF) {
				ch = fgetc(input);
				if ((ch == ' ') || (ch == '\n') || (ch == '\t') || (ch == '\v') || (ch == '\f') || (ch == EOF) || (ch == '.') || (ch == ',')) {
					if (words == 1) {
						words = 0;
						isspace++;
					}
				}
				else {
					words = 1;
				}
			}
			fseek(input, 0, SEEK_SET);
			printf("The file has %d words\n", isspace);
		}
		else {
			printf("Invalid command.\n");
		}
	}
	fclose(input);
	return 0;
}