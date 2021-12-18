#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void show3( FILE *fin )
{
	fgetc(fin);

	unsigned char flags = fgetc(fin); 
	int isExpanded = 0;

	if ((flags << 1) >> 7 == 1)
		isExpanded = 1;

	unsigned long long metasize = 0;

	for (int i = 0; i < 4; i++)
	{
		unsigned char byte = fgetc(fin);

		metasize = (metasize << 7) + byte;
	}

	unsigned long long cnt = 0;

	/*
	if (isExpanded)
	{
		unsigned char exphead = 0;

		for (int i = 0; i < 4; i++)
		{
			unsigned char byte = fgetc(fin);

			cnt++;
			exphead = exphead << 7 + byte;
		}

		for (int i = 0; i < exphead; i++, fgetc(fin), cnt++)
			;
	}
	*/

	
	while (cnt < metasize)
	{
		unsigned char symbol = fgetc(fin);
		cnt++;

		if (symbol == NULL)
			break;
       
        if(symbol=='T'||symbol=='C'||symbol=='A'||symbol=='U')
printf("\n") ;

		printf("%c", symbol);
		for (int i = 0; i < 3; i++, cnt++)
        
			printf("%c", fgetc(fin)); 
		unsigned long long framesize = 0;

	
		for (int i = 0; i < 4; i++)
		{
			unsigned char byte = fgetc(fin);

			cnt++;
			framesize = (framesize << 7) + byte;
		}

		unsigned long long cnt_frm = 0;

		fgetc(fin);
		fgetc(fin); 
        fgetc(fin);

		cnt += 3;
		cnt_frm += 1;
if(symbol=='T'||symbol=='C'||symbol=='A'||symbol=='U')
		printf(":");

		//printf("%u / %u\n", cnt_frm, framesize);
int j =0;
		while (cnt_frm < framesize)
		{
			unsigned char byte = fgetc(fin);
			cnt_frm++;
			cnt++;

			if (symbol == 'T') 
				printf("%c", byte);
            else if(symbol == 'A' && j<10){
                printf("%c", byte);
                j++;

            }
            else if(symbol=='C')
                printf("%c", byte);
            else if(symbol == 'U'){
                printf("%c", byte);
            }
            else{
                //printf("No");
            }
		}

		//printf("\n%d / %d\n", cnt, metasize);
	}
}
void Getprop(FILE *fin,char *Frame,int *Sizeval){
    fgetc(fin);
    int presenceOfAFlag = 0;
    int n= 0;
	unsigned char flags = fgetc(fin); 
	int isExpanded = 0;

	if ((flags << 1) >> 7 == 1)
		isExpanded = 1;

	unsigned long long metasize = 0;

	for (int i = 0; i < 4; i++)
	{
		unsigned char byte = fgetc(fin);

		metasize = (metasize << 7) + byte;
	}

	unsigned long long cnt = 0;

	/*
	if (isExpanded)
	{
		unsigned char exphead = 0;

		for (int i = 0; i < 4; i++)
		{
			unsigned char byte = fgetc(fin);

			cnt++;
			exphead = exphead << 7 + byte;
		}

		for (int i = 0; i < exphead; i++, fgetc(fin), cnt++)
			;
	}
	*/

char buff[4];
	while (cnt < metasize)
	{
		unsigned char symbol = fgetc(fin);
		cnt++;

		if (symbol == 0)
			break;

	
        
		//printf("%c", symbol);
        buff[0]=symbol;
		for (int i = 0; i < 3; i++, cnt++){
            buff[i+1]=fgetc(fin);
            //printf("%c", buff[i+1]);
        }
        
         if (memcmp(Frame,buff,4)==0){
             for (int i = 0; i < 4; i++){
                 printf("%c",buff[i]);
             }
             presenceOfAFlag++;
         }
		unsigned long long framesize = 0;

	
		for (int i = 0; i < 4; i++)
		{
			unsigned char byte = fgetc(fin);

			cnt++;
			framesize = (framesize << 7) + byte;
		}

		unsigned long long cnt_frm = 0;

		fgetc(fin);
		fgetc(fin); 
        fgetc(fin);

		cnt += 3;
		cnt_frm += 1;

	 if (memcmp(Frame,buff,4)==0){
		printf(" : ");
     }
		//printf("%u / %u\n", cnt_frm, framesize);
int j =0;

		while (cnt_frm < framesize)
		{
			unsigned char byte = fgetc(fin);
			cnt_frm++;
			cnt++;
            
            /*if (Sizeval != NULL)
                Sizeval[n]++;*/
        if (memcmp(Frame,buff,4)==0){
            if (symbol == 'T'){
                printf("%c", byte);
            } 
				
            else if(symbol == 'A' && j<10){
                printf("%c", byte);
                j++;

            }
            else if(symbol=='C'){
                printf("%c", byte);
            }  
        }
      }
      n++;
	}	
    if(presenceOfAFlag==0){
        printf("Flag not found");
    }
}
void Setprop(FILE*fin,FILE*fout,char *name,char *value,int sizeNew){
    int SizeVal[5] = {0, 0, 0, 0, 0};
    fseek(fin,0,SEEK_SET);
    fputc((int)fgetc(fin),fout);
    fputc((int)fgetc(fin),fout);
    fputc((int)fgetc(fin),fout);
    fputc((int)fgetc(fin),fout);
    Getprop(fin, name, SizeVal);
    
    fseek(fin,4,SEEK_SET);
    fputc((int)fgetc(fin),fout);
	  fputc((int)fgetc(fin),fout);
	unsigned long long metasize = 0;

	for (int i = 0; i < 4; i++)
	{
		unsigned char byte = fgetc(fin);
        fputc((int)byte,fout);

		metasize = (metasize << 7) + byte;
	}
    int diff = 0;
   /* for(int i = 0;i<5 && SizeVal[i]>0;i++){
        diff+=SizeVal[i]-sizeNew;
    }
    metasize= (unsigned long long)(metasize - diff);
    for(int i = 3;i>=0;i--){
        putc((int)((metasize >> 7*i) % 128),fout);
    }*/

	unsigned long long cnt = 0;
char buff[4];
int n = 0;

	while (cnt < metasize)
	{
		unsigned char symbol;
        if(SizeVal[0]==0){
SizeVal[0]=='1';
fputs(name,fout);
symbol = name[0];
buff[1] = name[1];
buff[2] = name[2];
buff[3] = name[3];
fputs(value,fout);
} else{
        symbol = fgetc(fin);
        fputc((int)symbol,fout);
		cnt++;
		if (symbol == 0)
			break;
        buff[0]=symbol;
		for (int i = 0; i < 3; i++, cnt++){
            buff[i+1]=fgetc(fin);
            fputc((int)buff[i+1],fout);
        }
}
		unsigned long long framesize = 0;
        unsigned long long framesize2 = 0;


	
		/*for (int i = 0; i < 4; i++)
		{
			unsigned char byte = fgetc(fin);

			cnt++;
			framesize = (framesize << 7) + byte;
		}*/

		unsigned long long cnt_frm = 0;


    if(memcmp(buff,name,4)==0){
        for (int i = 0; i < 4; i++)
		{
			unsigned char byte = fgetc(fin);
            //fputc((int)byte,fout);


			cnt++;
			framesize = (framesize << 7) + byte;
		}
    framesize2 = framesize;

    printf("%d\n%d",SizeVal[n],sizeNew);
    framesize= (unsigned long long)(sizeNew+1);
    printf("%lld\n",framesize);
    for(int i = 3;i>=0;i--){
        fputc((int)((framesize >> 7*i) % 128),fout);
        }
             
		fputc((int)fgetc(fin),fout);
		fputc((int)fgetc(fin),fout);
        fputc((int)fgetc(fin),fout);
		cnt += 3;
        cnt_frm++;
        fputs(value,fout);
        fseek(fin,framesize2-1,SEEK_CUR);
        cnt+=framesize2-1;
        n++;
    }
else if(memcmp(buff,name,4)!=1)
{
    for (int i = 0; i < 4; i++)
		{
			unsigned char byte = fgetc(fin);
             fputc((int)byte,fout);


			cnt++;
			framesize = (framesize << 7) + byte;
		}


    	fputc((int)fgetc(fin),fout);
		fputc((int)fgetc(fin),fout);
        fputc((int)fgetc(fin),fout);

		cnt += 3;
		cnt_frm += 1;


int j =0;
		while (cnt_frm < framesize)
		{
		    fputc((int)fgetc(fin),fout);
			cnt_frm++;
			cnt++;
  
        }
        }
      }
      char lol = fgetc(fin);
    while(!feof(fin)){
        fputc((int)lol,fout);
        lol = fgetc(fin);
        }
	}

int main(int argc,char **argv){
    FILE *fin;
    FILE *fout;
   
    if(memcmp(argv[1],"--filepath=",10)==0){
    for(int i = 0;i<strlen(argv[1]);i++){
        argv[1][i]=argv[1][i+11];
    }
    fin = fopen(argv[1],"rb");

    if (fin==NULL){
        printf("ERROR:File unfound\n");
        return 1;
    }
    else{
        printf("File found\n");
    }
    unsigned char version,bytes[3];
    
    for (int i = 0;i < 3;i++)
    bytes[i] = fgetc(fin);
    version = fgetc(fin);
    if(memcmp(argv[2],"--show",6)==0){
    if (bytes[0]=='I' && bytes[1]=='D' && bytes[2]=='3'){
        
        printf("ID3.v2.%d\n",version);
        
        if (version == 0){
      
        }
        else if (version == 1){

        }
         else if (version == 2){

        }
         else if ((int)version == 3){
            show3(fin); 
            
        }
         else{
        }
    }
    }
    else if(memcmp(argv[2],"--get=",6)==0){
    for(int i = 0;i<strlen(argv[2]);i++)
    {
        argv[2][i]=argv[2][i+6];
    }
      char arr[5];
      Getprop(fin,argv[2],arr);
    }
       /* if(memcmp(argv[1],"--filepath=",10)==0){
    for(int i = 0;i<strlen(argv[1]);i++){
        argv[1][i]=argv[1][i+11];
    }
    fin = fopen(argv[1],"rb");
        }
    if (fin==NULL){
        printf("ERROR:File unfound\n");
        return 1;
    }
    else{
        printf("File found\n");
    }*/
   /* else if(memcmp(argv[2],"--set=",5)==0 && memcmp(argv[3],"--value=",7)==0){
          for(int i = 0;i<strlen(argv[2])&& argv[2][i+6]!=NULL;i++)
    {
        argv[2][i]=argv[2][i+6];
    }
    for(int i = 0;i<strlen(argv[3])&& argv[3][i+8]!=NULL;i++)
    {
        argv[3][i]=argv[2][i+8];
    }
    Setprop(fin,fout,argv[2],argv[3],strlen(argv[3]));
    }
    else{
        printf("ERORR arguments\n");
    }
    }*/
    
    
    
   
   else if(memcmp(argv[2],"--set=",6)==0){
       for(int i = 0;i<strlen(argv[2])+6;i++)
    {
        argv[2][i]=argv[2][i+6];
        //printf("%s\n",argv[2]);
    }
    if (memcmp(argv[3],"--value=",8)==0){
        for(int i = 0;i<strlen(argv[3])+8;i++)
    {
        argv[3][i]=argv[3][i+8];
        //printf("%s\n",argv[3]);
    }
       fin=fopen(argv[1],"rb");
    fout = fopen("Test2.mp3","wb");
    Setprop(fin,fout,argv[2],argv[3],strlen(argv[3]));
    fclose(fin);
    fclose(fout);
    fout=fopen(argv[1],"wb");
   fin = fopen("Test2.mp3","rb");
    char lol = fgetc(fin);
    while(!feof(fin)){
        fputc((int)lol,fout);
        lol = fgetc(fin);
        }
    }
  }
    }
  // Setprop(fin,fout,argv[2],argv[3],strlen(argv[3]));
  /* fclose(fin);
   fclose(fout);
    fout = fopen("Test2.mp3","rb");
    fin = fopen(argv[1],"wb");

   char symbol;
    symbol = fgetc(fout);
    while(!feof(fout)){
        fputc((int)symbol,fin);
        symbol = fgetc(fout);
    }*/
    /*long res;
    long tag[15];
    int j = 0;
    typedef struct{
    char tag[3];
    char title[30];
    char artist[30];
    char album[30];
    char year[4];
    char comment[30];
    unsigned char genre;
    }MP3inf;
    for(int i = 0 ;i<6;i++){
        version = fgetc(fin);
      //  printf("%i\n",version);
    }
    for(int i = 0;i<4;i++){
         version = fgetc(fin);
         res <<= 7;
         res += (int)version;
         //printf("%d\n",version);
    }
   // printf("%i",res);
    unsigned char res2;
    for(int i = 0;i<5;i++){
        version = fgetc(fin);
    }
    version = fgetc(fin);
    res2 =((version << 3)>>6);
    printf("version index %i\n",res2);
    res2 =((version << 5)>>6);
    printf("layer index %i\n",res2);
    res2 =((version << 7)>>7);
    printf("bit protect %i\n",res2);
    version = fgetc(fin);
    res2 =((version << 4));
    printf("bitrate %i\n",res2);
    res2 =((version << 4)>>6);
    printf("bit frequency %i\n",res2);
    res2 =((version << 6)>>7);
    printf("shift bit %i\n",res2);
    res2 =((version << 7)>>7);
    printf("bit private %i\n",res2);
    version = fgetc(fin);
    res2 =((version )>>6);
    printf("index chanel %i\n",res2);
    res2 =((version<<2 )>>6);
    printf("expansion  %i\n",res2);
    res2 =((version <<4)>>7);
    printf("bit copyright %i\n",res2);
     res2 =((version <<4)>>7);
    printf("bit original %i\n",res2);
     res2 =((version <<6)>>7);
    printf("bit accent %i\n",res2);
    printf("%i",version);*/
return 0;
}