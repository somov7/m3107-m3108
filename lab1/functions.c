#define _CRT_SECURE_NO_WARNINGS
#include "WordCount.h"

int WordCount(FILE* F)
{
	int len, i, j, cntw = 0;
	char* ch;

	len = SizeByte(F);
	rewind(F);
	ch = (char*)malloc(len + 1);

	if (ch != NULL)
		for (i = 0; i < len; i++)
			ch[i] = fgetc(F);
	for (j = 1; j < len; j++)
	{
		if (ch[j] != ' ' && ch[j] != '\n' && ch[j] != '\t')
			if (ch[j - 1] == ' ' || ch[j - 1] == '\n' || ch[j - 1] == '\t')
				cntw++;
	}
	if (ch[0] == ' ' || ch[0] == '\n')
		return cntw;
	else
		return cntw + 1;
}
int LineCount(FILE* F)
{
	int cntl = 0;
	char ch;

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
