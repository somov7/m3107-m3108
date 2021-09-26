#define _CRT_SECURE_NO_WARNINGS
#include "WordCount.h"
int main(int argc, char* argv[])
{
	int option = 0;
	FILE* F;
	if (argv[1] == NULL || argv[2] == NULL)
	{
		printf("Not enough arguments");
		return 1;
	}
	if (((strcmp(argv[1], "-l")) == 0) || ((strcmp(argv[1], "--lines")) == 0))
		option = 1;
	else if (((strcmp(argv[1], "-c")) == 0) || ((strcmp(argv[1], "--bytes")) == 0))
		option = 2;
	else if (((strcmp(argv[1], "-w")) == 0) || ((strcmp(argv[1], "--words")) == 0))
		option = 3;
	else
	{
		printf("No such option exists");
		return 1;
	}
	
	if (F = fopen(argv[2], "r"))
	{
		switch (option) {
		case 1:
			printf("%i", LineCount(F));
			break;
		case 2:
			printf("%i", SizeByte(F));
			break;
		case 3:
			printf("%i", WordCount(F));
			break;
		}
		fclose(F);
	}
	else 
	{
		printf("Cannot open this file");
		return 1;
	}
	return 0;
}