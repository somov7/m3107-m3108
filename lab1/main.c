#include <stdio.h>
#include <string.h>

#include "header.h"

int main( int argc, char* argv[] )
{
	FILE* file;
	FILE* file2;
	int res;
	char* cmd[] = { "-l", "--lines", "-c", "--bytes", "-w", "--words" };

	if (argc != 3)
	{
		printf("error\n");
		return 1;
	}

	if ((file = fopen(argv[2], "r")) == NULL)
	{
		printf("error\n");
		return 2;
	}

	if ((file2 = fopen(argv[2], "rb")) == NULL)
	{
		printf("error\n");
		return 2;
	}

	if (strcmp(argv[1], cmd[0]) == 0 || strcmp(argv[1], cmd[1]) == 0)
		res = Lines(file);
	else if (strcmp(argv[1], cmd[2]) == 0 || strcmp(argv[1], cmd[3]) == 0)
		res = Bytes(file2);
	else if (strcmp(argv[1], cmd[4]) == 0 || strcmp(argv[1], cmd[5]) == 0)
		res = Words(file);
	else 
	{
		printf("error\n");
		return 3;
	}

	fclose(file);
	fclose(file2);

	printf("%i", res);

	return 0;
}