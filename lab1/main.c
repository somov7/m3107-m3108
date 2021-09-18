#include "Lab1.h"
#include <stdio.h>
#include <string.h>

void main(int argc, char *argv[])
{
	char *cmd[] = { "-l", "--lines", "-c", "--bytes", "-w", "--words" };
	FILE *file;

	if (argc != 3)
	{
		printf("Error: incorrect number of arguments\n");
		return;
	}

	if (!strcmp(argv[1], cmd[0]) || !strcmp(argv[1], cmd[1]))
	{
		file = open_file(argv[2], 0);
		printf("Number of lines: %i\n", count_lines(file));
		fclose(file);
	}
	else
		if (!strcmp(argv[1], cmd[2]) || !strcmp(argv[1], cmd[3]))
		{
			file = open_file(argv[2], 1);
			printf("Size of file: %lu bytes\n", count_bytes(file));
			fclose(file);
		}
		else
			if (!strcmp(argv[1], cmd[4]) || !strcmp(argv[1], cmd[5]))
			{
				file = open_file(argv[2], 0);
				printf("Number of words: %i\n", count_words(file));
				fclose(file);
			}
			else
			{
				printf("Error: unknown command\n");
				return;
			}
}