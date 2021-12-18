#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BITRW.h"

typedef struct tagTREE TREE;
struct tagTREE
{
	char Ch;
	long Freq;
	TREE *Left, *Right;
};

typedef struct 
{
	int Code[47], Len;
} CODE;

static long Freq[256];

void CountFreq( FILE *F )
{
	int i;

	memset(Freq, 0, sizeof Freq);

	while ((i = fgetc(F)) != EOF)
		Freq[i]++;

	for (i = 0; i< 256; i++)
		if (Freq[i] > 0)
			printf("Symbol %c (%d) appears %ld times\n", i < 32 ? '.' : i, i, Freq[i]);
}

TREE *Nodes[256];
int Num = 0;

void PlaceNode( char Ch, long Freq, TREE *L, TREE *R )
{
	TREE *T;
	int i;

	if ((T = malloc(sizeof(TREE))) == NULL)
		return;
	T->Ch = Ch;
	T->Freq = Freq;
	T->Left = L;
	T->Right = R;

	i = Num - 1;
	while (i >= 0 && T->Freq > Nodes[i]->Freq)
		Nodes[i + 1] = Nodes[i], i--;
	Nodes[i + 1] = T;
	Num++;
}

void BuildTree( void )
{
	int i;

	for (i = 0; i < 256; i++)
		if (Freq[i] > 0)
			PlaceNode(i, Freq[i], NULL, NULL);

	while (Num > 1)
	{
		TREE
			*Last = Nodes[--Num],
		    *PreLast = Nodes[--Num];
        PlaceNode(' ', Last->Freq + PreLast->Freq, PreLast, Last);
	}
}

CODE NewCodes[256], CurCode;

void BuildCodes( TREE *T )
{
	if (T == NULL)
		return;
	CurCode.Code[CurCode.Len] = 0;
	CurCode.Len++;
	BuildCodes(T->Left);
	CurCode.Len--;

     CurCode.Code[CurCode.Len] = 1;
	 CurCode.Len++;
	 BuildCodes(T->Right);
	 CurCode.Len--;
    
	 NewCodes[(unsigned char)T->Ch] = CurCode;
	/* for (i = 0; i < CurCode.Len; i++)
		 printf("%i", CurCode.Code[i]);
     printf("%i\n", CurCode.Len); */
}

double Kof( void )
{
	int i, total = 0;
	double ratio, unpack = 0;

    for (i = 0; i < 256; i++)
	{
		total += NewCodes[i].Len * Freq[i];
	    unpack += Freq[i] * 8;
	}
	ratio = (double)unpack / total;

	return ratio;
}

void Stat( char *Filename )
{
	FILE *F;

	if ((F = fopen(Filename, "rb")) == NULL)
	{
		printf("File '%s' is not found\n", Filename);
		return;
	}
	CountFreq(F);
	BuildTree();
	CurCode.Len = 0;
	BuildCodes(Nodes[0]);
	printf("Ratio is %f\n", Kof()); 
}

void ClearTree( TREE **T )
{
  if ((*T) != NULL)
  {
    ClearTree(&(*T)->Left);
	ClearTree(&(*T)->Right);
	free(*T);
	(*T) = NULL;
  }
}

int Compress( char *OutFileName, char *InFileName )
{
	TREE *T;
	FILE *InF, *OutF;
	int ch;
	int i;

    if ((InF = fopen(InFileName, "rb")) == NULL || (OutF = fopen(OutFileName, "wb")) == NULL)
		return 0;

	CountFreq(InF);
	Num = 0;
	BuildTree();

	memset(NewCodes, 0, sizeof(NewCodes));
	CurCode.Len = 0;
    BuildCodes(Nodes[0]);
	rewind(InF);
	fwrite(Freq, 4, 256, OutF);

	for (int i = 0; i < 256; i++)
		printf("%ld ", Freq[i]);
    WriteBitInit(OutF);
	while ((ch = fgetc(InF)) != EOF)
	{
		/*printf("%c", ch);*/
		for (i = 0; i < NewCodes[ch].Len; i++)
			WriteBit(NewCodes[ch].Code[i]);
	}
	WriteBitClose();
	ClearTree(&Nodes[0]);

	fclose(InF);
	fclose(OutF);

	OutF = fopen(OutFileName, "rb");
	memset(Freq, 0, sizeof(Freq));
	fread(Freq, 4, 256, OutF);

	return 1;
}

int Decompress( char *OutFileName, char *InFileName )
{
	TREE *T;
	FILE *InF, *OutF;
	int n, i;

	if ((InF = fopen(InFileName, "rb")) == NULL || (OutF = fopen(OutFileName, "wb")) == NULL)
		return 0;

	fread(Freq, 4, 256, InF);
	Num = 0;
	BuildTree();
	T = Nodes[0];
	n = Nodes[0]->Freq;
	ReadBitInit(InF);
	while (n > 0)
		if (T->Left == NULL)
		{
			fputc(T->Ch, OutF);
			n--;
			T = Nodes[0];
		}
		else
			if (ReadBit())
				T = T->Right;
			else
				T = T->Left;
	/*ClearTree(&Nodes[0]);*/

	fclose(InF);
	fclose(OutF);

	return 1;
}

