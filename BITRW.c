#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BITRW.h"

void WriteBitInit( FILE *F )
{
	BitF = F;
	BitAccum = 0;
	BitPos = 7;
}

void WriteBit( int Bit )
{
	BitAccum |= Bit << BitPos--;

	if (BitPos < 0)
	{
		fputc(BitAccum, BitF);
		BitAccum = 0;
		BitPos = 7;
	}
}

void WriteBitClose( void )
{
	if (BitPos < 7)
	  fputc(BitAccum, BitF);
}

void ReadBitInit(FILE *F)
{
	BitF = F;
	BitPos = -1;
}

int ReadBit( void )
{
	if (BitPos < 0)
		BitAccum = fgetc(BitF), BitPos = 7;
	return (BitAccum >> BitPos--) & 1;
}