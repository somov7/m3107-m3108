#include <stdio.h>

#include "header.h"

int Lines( FILE* file )
{
	int cnt = 0;
	char c;

	while ((c = fgetc(file)) != EOF)
		if (c == '\n')
			++cnt;

	return cnt + 1;
}

int Bytes( FILE* file )
{
	int cnt = 0;

	while (fgetc(file) != EOF)
		++cnt;

	return cnt;
}

int Words( FILE* file )
{
	int flag = 0, cnt = 0;
	char c;

	while ((c = fgetc(file)) != EOF)
		if (c == ' ' || c == '\n' || c == '\t')
			flag = 0;
		else
			if (!flag)
				flag = 1, ++cnt;
	return cnt;
}