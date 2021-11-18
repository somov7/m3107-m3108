#include "Lab1.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	char *cmd[] = { "-l", "--lines", "-c", "--bytes", "-w", "--words" };
	FILE *file, *binf;
	int res;

	if (argc != 3) {
		printf("Error: incorrect number of arguments\n");
		return 1;
	}
	if ((file = open_file(argv[2], 0)) == NULL || (binf = open_file(argv[2], 1)) == NULL) {
		printf("Error: can't open the file\n");
		return 2;
	}
	if (strcmp(argv[1], cmd[0]) == 0 || strcmp(argv[1], cmd[1]) == 0) 
		res = count_lines(file);
	else if (strcmp(argv[1], cmd[2]) == 0 || strcmp(argv[1], cmd[3]) == 0)
		res = count_bytes(binf);
	else if (strcmp(argv[1], cmd[4]) == 0 || strcmp(argv[1], cmd[5]) == 0)
		res = count_words(file);
	else {
		printf("Error: unknown command\n");
		return 3;
	}
	fclose(file);
	fclose(binf);
	printf("%i", res);
	return 0;
}