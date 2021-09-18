#include "Lab1.h"
#include <stdio.h>

FILE *open_file(char *filename, int bin)
{
	FILE* file;

	if (bin)
		file = fopen(filename, "rb");
	else
		file = fopen(filename, "r");

	if (file == NULL)
	{
		printf("Error: can't open the file\n");
		return;
	}
	return file;
}

int count_lines(FILE* file)
{
	int res = 0, flag = -1;
	char c;

	while ((c = fgetc(file)) != EOF)
		if (c == '\n')
			++res, flag = 1;
		else
			flag = 0;
	if (flag == 0)
		++res;
	return res;
}

unsigned long int count_bytes(FILE* file)
{
	unsigned long int res = 0;

	while (fgetc(file) != EOF)
		++res;
	return res;
}

int count_words(FILE* file)
{
	int flag = 0, res = 0;
	char c;

	while ((c = fgetc(file)) != EOF)
		if (c == ' ' || c == '\n' || c == '\t')
			flag = 0;
		else
			if (!flag)
				flag = 1, ++res;
	return res;
}