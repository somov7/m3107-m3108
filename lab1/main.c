//C:\Users\Obidj\Documents\Lab\1\Debug\1.exe -c C:\\Users\\Obidj\\Documents\\Lab\\1\\text.txt
//C:\Users\Obidj\Documents\Lab\1\Debug\1.exe -w C:\\Users\\Obidj\\Documents\\Lab\\1\\text.txt
//C:\Users\Obidj\Documents\Lab\1\Debug\1.exe -l C:\\Users\\Obidj\\Documents\\Lab\\1\\text.txt
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
int errors(int argc, char argv[], char way[])
{
	FILE* file = fopen(way, "r");
	if (argc != 3)
	{
		printf("Ошибка! Аргументов должно быть 2!");
		return 1;
	}
	if (strcmp(argv, "-c") != 0 && strcmp(argv, "--bytes") != 0 && strcmp(argv, "-w") != 0 && strcmp(argv, "--words") != 0 && strcmp(argv, "-l") != 0 && strcmp(argv, "--lines") != 0 )
	{
		printf("Ошибка! Введены неправильные аргументы!");
		return 1;
	}
	if (!file)
	{
		printf("Ошибка! Неправильно введён путь к файлу.");
		return 1;
	}
	return 0;
}
int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "ru");
	char f[1000000];
	int bytes = 0, words=0, lines=0, i=1;
	if (errors(argc, argv[1], argv[2]) == 0)
	{
		FILE* file = fopen(argv[2],"r");
		if (strncmp(argv[1], "-c") == 0 || strncmp(argv[1], "--bytes") == 0)
		{
			while (fscanf(file, "%c", f) != EOF)
				bytes++; 
			printf("%d", bytes);
		}
		else if (strncmp(argv[1], "-w") == 0 || strncmp(argv[1], "--words") == 0)
		{
			while (fscanf(file, "%s", f) != EOF)
				words++;
			printf("%d", words);
		}
		else if (strncmp(argv[1], "-l") == 0 || strncmp(argv[1], "--lines") == 0)
		{
			while (fgets(f,10000,file)!=NULL)
				lines++;
			printf("%d",lines);
		}
	}
	return 0;
}