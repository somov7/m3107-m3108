#define W 64
#define H 64
#define Header 62
#define SizeRast 40
#define Density 2795

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <glut.h>

void CreateBMP( void )
{
	FILE * fin = fopen("/Users/siriusa/Desktop/ITMO_Practic/lol.bmp","wb");
	char symbol;

	fputc((int)'B', fin);
	fputc((int)'M', fin);

	for (int i = 0; i <= 3; i++)
    	fputc((int)(((((H * W) / 8) + Header) >> 8 * i) % 256), fin);
	
	for (int i = 0; i <= 3; i++)
        fputc(0, fin);
	
	for (int i = 0; i <= 3; i++)
        fputc((int)((Header >> 8 * i) % 256), fin);
    
	for (int i = 0; i <= 3; i++)
        fputc((int)((SizeRast >> 8 * i) % 256), fin);
    
	for (int i = 0; i <= 3; i++)
        fputc((int)((W >> 8 * i) % 256), fin);
    
	for (int i = 0; i <= 3; i++)
        fputc((int)((H >> 8 * i) % 256), fin);
	
	fputc(1, fin);
	fputc(0, fin);
	fputc(1, fin);
	fputc(0, fin);

	for (int i = 0;i <= 3; i++)
        fputc(0, fin);
	
	for (int i = 0; i <= 3; i++)
    	fputc((int)(((H * W / 8) >> 8 * i) % 256), fin);
    
	for (int i = 0; i <= 3; i++)
    	fputc((int)((Density >> 8 * i) % 256), fin);
    
	for (int i = 0; i <= 3; i++)
    	fputc((int)((Density >> 8 * i) % 256), fin);
    
	for (int i = 7; i >= 0; i--)
        fputc(0, fin);
    
	fputc(0, fin);
	fputc(0, fin);
	fputc(0, fin);
	fputc(0, fin);
	//////////////
	fputc(255, fin);
	fputc(255, fin);
	fputc(255, fin);
	fputc(0, fin);

	for (int y = 0; y < H * W; y++)
		fputc(rand() % 256, fin);
}

/*
void zapic()
{
	FILE*fin=fopen("","rb");
	FILE*fout=fopen("","wb");
	for (int i = 0;i<62;i++){
		fputc((int)fgetc(fin),fout);
	}
	while(!feof(fin) ){
		fputc(rand()% 255, fout);
		fgetc(fin);
	}
}
*/

struct GenData {
	unsigned long long We = 0, He = 0; 
	unsigned char ** Gen; 
	unsigned char Palitra[6]; 
};

typedef struct GenData GenData;

GenData ReadBMP( char *ffin ){
	FILE * fin = fopen(ffin,"rb");
	GenData GD;
	unsigned long long size = 0;
	 if (fin==NULL){
        printf("ERROR:File unfound\n");
        
    }
    else{
        printf("File found\n");
    }
=
	

	fgetc(fin);
	fgetc(fin);
	
	for (int i = 0; i <= 3; i++)
	{
		unsigned char byte = fgetc(fin);
        size += byte << 8 * i;
    }

	size -= 62;

	//printf("%u\n",size);

	for (int i = 0; i < 8;i++)
    	fgetc(fin);
    
	for (int i = 0; i < 4; i++)
    	fgetc(fin);

	for (int i = 0; i <= 3; i++)
	{
		unsigned char byte = fgetc(fin);
        GD.We += byte<< 8 * i;
    }
	for (int i = 0; i <= 3; i++)
	{
		unsigned char byte = fgetc(fin);
        GD.He += byte << 8 * i;
    }

	
	for (int i = 0; i < 28; i++)
    	fgetc(fin);
	
	///////////////////////

	for (int i = 0; i < 3; i++)
    	GD.Palitra[i] = fgetc(fin);

	fgetc(fin);

	for (int i = 3; i < 6; i++)
    	GD.Palitra[i] = fgetc(fin);

	fgetc(fin);
	
	GD.Gen = (unsigned char **)malloc(GD.He * sizeof(unsigned char *));
	for (int i = 0; i < GD.He; i++)
		GD.Gen[i] = (unsigned char *)malloc(GD.We * sizeof(unsigned char));
	unsigned long long checkWe = 0;

	if((GD.We%8)==0){
		checkWe=GD.We/8;
	}
	else{
		checkWe=GD.We/8 +1;
	}
    if(checkWe%4==0){
	for (int i = GD.He - 1; i >= 0; i--)
		for (int j = 0; j < checkWe; j++)
		{
			unsigned char byte = fgetc(fin);
        if(j!=checkWe-1)
			for (int bit = 0; bit < 8; bit++)
				GD.Gen[i][j * 8 + bit] = (byte>>(7-bit))%2;
				
		else if(j==checkWe-1){
			for (int bit = 0; bit < GD.We-(checkWe-1)*8; bit++)
				GD.Gen[i][j * 8 + bit] = (byte>>(7-bit))%2;
		}
		} 
	}
	else{
	for (int i = GD.He - 1; i >= 0; i--){
		for (int j = 0; j < checkWe; j++)
		{
			unsigned char byte = fgetc(fin);
        if(j!=checkWe-1)
			for (int bit = 0; bit < 8; bit++)
				GD.Gen[i][j * 8 + bit] = (byte>>(7-bit))%2;
		else if(j==checkWe-1){
			for (int bit = 0; bit < GD.We-(checkWe-1)*8; bit++)
				GD.Gen[i][j * 8 + bit] = (byte>>(7-bit))%2;
		}
		}
		for(int k = 0;k< 4-(checkWe%4);k++)
		fgetc(fin);

	}
	}

	for (int i = 0;i<GD.He;i++){
		for (int j = 0;j<GD.We;j++)
			printf("%d ",GD.Gen[i][j]);
		printf("\n");
	}

	return GD;
}
void Init( int (* arr)[W] )
{
	for (int y = 0; y < H; y++)
		for (int x = 0; x < W; x++)
			arr[y][x] = rand() % 2;
}

int NeighborsCount( unsigned char** old, int y, int x ,GenData Gen)
{
	int cnt = 0;

	for (int dy = -1; dy <= 1; dy++)
		for (int dx = -1; dx <= 1; dx++)
			if (!(dy == 0 && dx == 0) && old[(y + dy + Gen.He) % Gen.He][(x + dx + Gen.We) % Gen.We] == 1)
				cnt++;

	return cnt;
}
void nexttGeneration( unsigned char** old, unsigned char **nextt,GenData Gen)
{
	int cnt;

	for (int y = 0; y <Gen.He; y++)
		for (int x = 0; x < Gen.We; x++)
		{
			cnt = NeighborsCount(old, y, x,Gen);

			if (old[y][x] == 1)
			{
				if (cnt >= 2 && cnt < 4)
					nextt[y][x] = 1;
				else
					nextt[y][x] = 0;
			}

			else
			{
				if (cnt == 3)
					nextt[y][x] = 1;
				else
					nextt[y][x] = 0;
			}
		}
}

void CopyGeneration( unsigned char **old, unsigned char **nextt, GenData Gen)
{
	for (int y = 0; y < Gen.He; y++)
		for (int x = 0; x < Gen.We; x++)
			old[y][x] = nextt[y][x];
}
void RecreateBmp(GenData Gen, char *ffin, char *ffout){
	FILE * fout = fopen(ffout,"wb");
	FILE * fin = fopen(ffin,"rb");
	
	for(int i=0;i<Header;i++){
		fputc(fgetc(fin),fout);
	}
		unsigned long long checkWe = 0;

	if((Gen.We%8)==0){
		checkWe=Gen.We/8;
	}
	else{
		checkWe=Gen.We/8 +1;
	}
    if(checkWe%4==0){
	for (int i = Gen.He - 1; i >= 0; i--)
		for (int j = 0; j < checkWe; j++)
		{
			unsigned char byte = 0;
        if(j!=checkWe-1)
			for (int bit = 7; bit >= 0; bit--)
				byte += Gen.Gen[i][j * 8 + 7-bit] << bit;
				
		else if(j==checkWe-1){
			for (int bit = 7; bit >= 8-(int)(Gen.We-(checkWe-1)*8) ; bit--){
				byte += Gen.Gen[i][j * 8 + 7-bit] << bit;
			}
		}
		fputc(byte,fout);
		} 
	}
	else{
	for (int i = Gen.He - 1; i >= 0; i--){
		for (int j = 0; j < checkWe; j++)
		{
			unsigned char byte = 0;
        if(j!=checkWe-1)
			for (int bit = 7; bit >= 0; bit--)
				byte += Gen.Gen[i][j * 8 + 7-bit] << bit;
				
		else if(j==checkWe-1){
			for (int bit = 7; bit >= 8-(int)(Gen.We-(checkWe-1)*8) ; bit--)
				byte += Gen.Gen[i][j * 8 + 7-bit] << bit;
		}
		fputc(byte,fout);
		}
		for(int k = 0;k< 4-(checkWe%4);k++)
		fputc(0,fout);

	}
	}
	
}
int main( int argc, char **argv )
{	
	
	if(memcmp(argv[1],"--input",7)==0 && memcmp(argv[3],"--output",8)==0 && memcmp(argv[5],"--max_iter",10)==0)
	{
    GenData Gen;
	Gen = ReadBMP( argv[2] );
	printf("%llu %llu\n",Gen.We,Gen.He);
	unsigned long long Weight,Height;
	Weight = Gen.We;
	Height = Gen.He;
	char text[20];
	char direct[40];
	int N = atoi(argv[6]);
	int Freq = 1;
	if(memcmp(argv[7],"--dump_freq",11)==0){
		Freq = atoi(argv[8]);
	}
	
	unsigned char old[Height][Weight]; unsigned char **nextt;
	nextt = (unsigned char **)malloc(Height * sizeof(unsigned char *));
	for (int i = 0; i < Height; i++)
		nextt[i] = (unsigned char *)malloc(Weight * sizeof(unsigned char));
    for(int i = 0;i<N;i++){
		memmove(direct,argv[4],strlen(argv[4])+1);
		sprintf(text, "%d", i+1);
		if((i+1)%Freq==0){
		strncat(text,".bmp",4);
		strncat(direct,text,strlen(text));
		printf("%s ", direct);
		RecreateBmp(Gen, argv[2], direct);
		nexttGeneration(Gen.Gen, nextt,Gen);
		CopyGeneration(Gen.Gen, nextt,Gen);
		}
	}

	}
	/*for (int i = 0;i<Height;i++){
		for (int j = 0;j<Weight;j++)
			printf("%d ",Gen.Gen[i][j]);
	    printf("\n");
	}
	}
	//RecreateBmp();
	//int old[Height][Weight], nextt[Height][Weight];
    //nexttGeneration(old, nextt);
    /*
	Init(old);

	for (int y = 0; y < H; y++)
	{
		for (int x = 0; x < W; x++)
			printf("%i ", old[y][x]);

		printf("\n");
	}

	for (int cnt = 0; cnt < 3; cnt++)
	{
		nexttGeneration(old, nextt);

		printf("\n");

		for (int y = 0; y < H; y++)
		{
			for (int x = 0; x < W; x++)
				printf("%i ", nextt[y][x]);

			printf("\n");
		}

		CopyGeneration(old, nextt);
	}*/
	
	return 0;
}