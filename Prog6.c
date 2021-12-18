#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HUF.h"
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
void CreateARC(char *Archive,char filename[100][100],int num)
{
	FILE *arc=fopen(Archive,"wb");
	fclose(arc);
    FILE*fout = fopen("Test.txt","wb");
    fwrite(&num,sizeof(int),1,fout);
    
    for(int i = 0;i<num;i++)
    {   
       printf("%s",filename[i]);
       int len = (int)strlen(filename[i]);
        
        fwrite(&len,sizeof(int),1,fout);
        fwrite(filename[i],sizeof(char),len,fout);
        FILE*fin = fopen(filename[i],"rb");
        fseek(fin, 0L, SEEK_END);
        long sz = (long)ftell(fin);
        printf("%ld ",sz);
        fwrite(&sz,sizeof(long),1,fout);
        rewind(fin);
        char symbol;
       //Stat(filename[i]);
       //Compress(Archive,filename[i]);
    while(!feof(fin))
    { 

        symbol = fgetc(fin);
        fputc((int)symbol,fout);
       
    }
    fclose(fin);
    }

Compress(Archive,"Test.txt");

}
void DecreateArc(char *Archive){
	
	 Decompress(Archive,"Test.txt");
     FILE*fin = fopen("Test.txt","rb");
     int num = 0;
     fread(&num,sizeof(int),1,fin);
      
    for(int i = 0;i<num;i++)
    {   
        printf("%d \n",num);
        int len;
        int trash;
        char filename[100];
        fread(&len,sizeof(int),1,fin);
        fread(filename,sizeof(char),len,fin);
       printf("len %d ",len);
        FILE*fout = fopen(filename,"wb");
        long sz = 0;
        fread(&sz,sizeof(long),1,fin);
        
        printf("sz %ld ",sz);
        
        int count = 0;
       // Decompress(filename,Archive);
    while(count<sz)
    { 
        char symbol;
        symbol = fgetc(fin);
        fputc((int)symbol,fout);
        count++;
    }
    fread(&trash,1,1,fin);
    fclose(fout);
     
    }
     
fclose(fin);
}
void ListOfArchive(char * Archive)
{
	Decompress(Archive,"Test.txt");
     FILE*fin = fopen("Test.txt","rb");
     int num = 0;
     fread(&num,sizeof(int),1,fin);
      
    for(int i = 0;i<num;i++)
    {   
       // printf("%d \n",num);
        int len;
        int trash;
        char filename[100][100];
        fread(&len,sizeof(int),1,fin);
        fread(filename[i],sizeof(char),len,fin);
       //printf("len %d ",len);
        FILE*fout = fopen(filename[i],"wb");
        long sz = 0;
        fread(&sz,sizeof(long),1,fin);
        
        printf("File name : %s \n",filename[i]);
        
        int count = 0;
        
    while(count<sz)
    { 
        char symbol;
        symbol = fgetc(fin);
        fputc((int)symbol,fout);
        count++;
    }
    fread(&trash,1,1,fin);
    fclose(fout);
    }
	Compress(Archive,"Test.txt");
}
int main( int argc,char **argv){
    char filename[100][100];
    if (memcmp(argv[1],"--file",6)==0 && memcmp(argv[3],"--create",8)==0){
        for(int i = 0;i<argc-4;i++){
            strcpy(filename[i],argv[i+4]);
			printf("%s\n",filename[i]);
        }
		
        CreateARC(argv[2],filename,argc-4);
    }
    else if (memcmp(argv[1],"--file",6)==0 && memcmp(argv[3],"--extract",9)==0){
         DecreateArc(argv[2]);
         
     }
     else if (memcmp(argv[1],"--file",6)==0 && memcmp(argv[3],"--list",6)==0){
         ListOfArchive(argv[2]);
     }
     else{
         printf("ERORR!");
         return 0;
     }
    //CreateARC("lol.arc",filename,1);
    //DecreateArc("lol.arc");
    //ListOfArchive("lol.arc");
    //Stat("stek.txt");
	//Compress("lol.arc", "stek.txt");
	//Decompress("stek.txt", "lol.arc");
    
    return 0;
}