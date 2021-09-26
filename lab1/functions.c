#define _CRT_SECURE_NO_WARNINGS
#include "WordCount.h"

int isspace(char c)
{
	return (c == ' ' || c == '\n' || c == '\t');
}
int WordCount(FILE* F)
{
	int cntw = 0;
	char ch;
	int len = SizeByte(F);
	int flag = 0;

	rewind(F);

	if (len == 0)
		return 0;
	else
	{
		while ((ch = fgetc(F)) != EOF)
		{
			if (isspace(ch))
				flag = 0;
			else
				if (flag == 0)
				{
					cntw++;
					flag = 1;
				}
		}
		return cntw;
	}
}
int LineCount(FILE* F)
{
	int cntl = 0;
	char ch;
	int len = SizeByte(F);

	rewind(F);

	if (len == 0)
		return 0;
	else
		while ((ch = fgetc(F)) != EOF)
		{
			if (ch == '\n')
				cntl++;
		}
		return cntl + 1;
}
int SizeByte(FILE* F)
{
	int cnt = 0;
	while (fgetc(F) != EOF)
		cnt++;

	return cnt;
}
