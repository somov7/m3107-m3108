//C:\Users\Алишер\Document\Lab\PR\main.exe -c C:\\Users\\Алишер\\Document\\Lab\\PR\\text.txt
//C:\Users\Алишер\Document\Lab\PR\main.exe -w C:\\Users\\Алишер\\Document\\Lab\\PR\\text.txt
//C:\Users\Алишер\Document\Lab\PR\main.exe -l C:\\Users\\Алишер\\Document\\Lab\\PR\\text.txt
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
int errors(int argc, char argv[], char path[])
{
	FILE* file = fopen(path, "r");
	if (argc != 3)
	{
		printf("Error! There must be 2 arguments!");
		return 1;
	}
	if (strcmp(argv, "-c") != 0 && strcmp(argv, "--bytes") != 0 && strcmp(argv, "-w") != 0 && strcmp(argv, "--words") != 0 && strcmp(argv, "-l") != 0 && strcmp(argv, "--lines") != 0 )
	{
		printf("Error! Invalid arguments entered!");
		return 1;
	}
	if (!file)
	{
		printf("Error! The path to the file was entered incorrectly.");
		return 1;
	}
	return 0;
}
int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "ru");
	int bytes = 0, words=0, lines=1, i=1, c;
	if (errors(argc, argv[1], argv[2]) != 0)
		return 1;
	FILE* file = fopen(argv[2],"r");
	if (strncmp(argv[1], "-c") == 0 || strncmp(argv[1], "--bytes") == 0)
	{
		while (fscanf(file, "%*c") != EOF)
			bytes++; 
		printf("%d", bytes);
	}
	else if (strncmp(argv[1], "-w") == 0 || strncmp(argv[1], "--words") == 0)
	{
		while (fscanf(file, "%*s ") != EOF)
			words++;
		printf("%d", words);
	}
	else if (strncmp(argv[1], "-l") == 0 || strncmp(argv[1], "--lines") == 0)
	{
		while ((c=fgetc(file))!= EOF)
				if (c=='\n') lines++;
		printf("%d",lines);
	}
	return 0;
} 