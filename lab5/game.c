#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#ifdef __linux__ 
    #include <sys/types.h>
    #include <sys/stat.h>
#elif _WIN32
    #include <direct.h>
#else
#endif




#pragma pack(push,1)

typedef struct BITMAPFILEHEADER{
    uint16_t Signature;
    uint32_t FileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t DataOffset;

} BITMAPFILEHEADER;

typedef struct BITMAPINFOHEADER{
    uint32_t Size;
    uint32_t Width;
    uint32_t Height;
    uint16_t Planes;
    uint16_t BitsPerPixel;
    uint32_t Compression;
    uint32_t ImageSize;
    uint32_t XpixelsPerM;
    uint32_t YpixelsPerM;
    uint32_t ColorsUsed;
    uint32_t ColorTable;
    // uint8_t RandomData1;   ??
    // uint8_t RandomData2;   ?? 
    // uint8_t RandomData3;   ??
    // uint8_t RandomData4;   ?? 
    // uint32_t RandomData58; ??
    uint64_t RandomData;
} BITMAPINFOHEADER;

#pragma pack(pop)

BITMAPFILEHEADER o_header;
BITMAPINFOHEADER inf_header;

void print_matrix_CMD(void);
void fill_headers(void);
void save_img(void);
void GameOfLife(void);

FILE* input;
FILE* output;
char* dirname;

int CMD_PRINT_FLAG = 0;

int h;
int w;

uint8_t* XX = NULL;
uint8_t* XXnew = NULL;

int max_iter = 5;
int dump_freq = 3;

int line_alig_4;

int main(int argc, char* argv[]){

    if(argc < 5){
        printf("enter all args!");
        return 1;

    }

    for(int i = 1; i < argc;i++){
        if(!strcmp(argv[i], "--input")){
            input = fopen(argv[i+1], "rb");
        }
        else if (!strcmp(argv[i], "--output")){
           dirname = argv[i+1];
            _mkdir(dirname);
        }
        else if (!strcmp(argv[i], "--max_iter")){
            max_iter = atoi(argv[i+1]);
        }
        else if (!strcmp(argv[i], "--dump_freq")){
            dump_freq = atoi(argv[i+1]);
        }
        else if (!strcmp(argv[i], "--print-img")){
            CMD_PRINT_FLAG = 1;
        }
    }

    if(!input){
        printf("[-] cannot open input image!");
        return 1;
    }
    
    save_img();
    if(CMD_PRINT_FLAG)
        print_matrix_CMD();
    XXnew = calloc(1, w*h);


    // основной цикл
    for (int i = 0; i < max_iter; i++)
    {
        for(int j =0; j < dump_freq; j++)
            GameOfLife();
        
        
        char* output_file = (char*)malloc(sizeof(char)*200);
        strcpy(output_file, dirname);
        strcat(output_file, "/gen_");
        char t[3];
        itoa(i, t, 10);
        strcat(output_file, t);
        strcat(output_file, ".bmp");

        
        output = fopen(output_file, "a+");
        if(!output){
            printf("[-] cannot open output image!");
            return 1;
        }
        fill_headers();

        for(int y = 0; y < h; y++){
            char* line = calloc(line_alig_4, sizeof(char)); 

            for(int x = 0; x < w; x++){
                int pos = x / 8;  
                if (XX[y*w+x])
                    // little-endian only????
                    // если написать просто 1 << (x % 8), то выводит байты в обратном порядке
                    line[pos] = line[pos] + (1 << (7 - x % 8));   
            }

            fwrite(line, 1, line_alig_4, output); 
        }
    }
    
    
    return 0;
}

void print_matrix_CMD(void){
    printf("\t\t||================||\n");
    printf("\t\t||you input image:||\n");
    printf("\t\t||================||\n");
    for (int i = h-1; i >= 0; i--){
        for (int j = 0; j < w; j++)
            printf("%d ", XX[i*w + j]);

        printf("\n");

    }
    printf("\n");
}


void fill_headers(void){

    fseek(input,0,0);
    fseek(output,0,0);

    fread(&o_header, sizeof(BITMAPFILEHEADER), 1, input);
    fwrite(&o_header, sizeof(BITMAPFILEHEADER), 1, output);

    fread(&inf_header, sizeof(BITMAPINFOHEADER), 1, input);
    fwrite(&inf_header, sizeof(BITMAPINFOHEADER) , 1, output);


    
    // fseek(output, 62, 0);
    // char seed[] = {0x80, 0x00, 0x00,0x00 ,0x0F, 0x0F, 0x0F, 0x80};
    // fwrite(&seed, sizeof(seed),1, output); 


}


void save_img(void){
    fseek(input,0,0);
    fseek(output,0,0);

    fread(&o_header, sizeof(BITMAPFILEHEADER),1,input); 
    fread(&inf_header, sizeof(BITMAPINFOHEADER), 1, input);
    fseek(input, o_header.DataOffset, 0);  

    h = inf_header.Height;
    w = inf_header.Width;

    XX = (uint8_t*)malloc(w*h); 
    

    /* https://stackoverflow.com/questions/48957742/how-to-calculate-bitmap-size
       https://docs.microsoft.com/ru-ru/windows/win32/gdi/capturing-an-image?redirectedfrom=MSDN

    Биты, представляющие пиксели растрового изображения,
    упакованы в строки. Размер каждой строки округляется до кратного 4 байтам */
    line_alig_4 = ((w + 31) / 32) * 4; 

    int matrix = line_alig_4 * h;

    char* data = calloc(matrix, sizeof(char));
    fread(data, 1, matrix, input);

    for(int y = h - 1; y >= 0; y--)
    {
        /*Сама картинка записывается построчно. Во-вторых, картинка идет как бы перевернутая вверх ногами.
        То есть сначала записана нижняя строка, потом предпоследняя и так далее до самого верха.
        И, в-третьих, как написано в [1], если размер строки растра не кратен 4, то
        она дополняется от 1 до 3 пустыми (нулевыми) байтами, чтобы длина строки оказалась кратна параграфу. 
        (с) цитата откуда-то с инета */ 

        for(int x = 0; x < w; x++)
        {
            int cur = y * line_alig_4 + x / 8;
            // little-endian only????
            // если написать просто 1 << (x % 8), то выводит байты в обратном порядке                
            int bit = 1 << (7 - x % 8); 

            int val;
            if((data[cur] & bit) > 0)
                val = 1;
            else
                val = 0;


            XX[y * w + x] = val; 
        }
    }
  
}



void GameOfLife(void){

    int live = 0 ;
    int matrix_size = w*h;
    for(int x = 0; x < h; x++){
        for(int y = 0; y < w; y++){

            int sum = 0;

            for (int i = -1; i <= 1; i++)
                for (int j = -1; j <= 1; j++)
                    if (!(i == 0 && j == 0) // не считаем сам элемент
                        && XX[(x+i)*w + y+j] // если элемент существует
                        && ((x+i) >= 0) && ((x+i) < h) // граница по высоте 
                        && ((y+j) >= 0) && ((y+j) < w)){ // граница по ширине 

                                sum++;
                }

            
            int coords_1dim = x*w + y; 
            if(XX[coords_1dim]) live = 1;

            if(live && sum < 2) //underpopulation
                XXnew[coords_1dim] = 0;
            else if(live && (sum == 2 || sum == 3)) // generation
                XXnew[coords_1dim] = 1; 

            else if(live && sum > 3) //overpopulation
                XXnew[coords_1dim] = 0;
            
            else if(!live && sum == 3) // reproduction
                XXnew[coords_1dim] = 1;

            else XXnew[coords_1dim] = 0;
        }
    }

    memcpy(XX, XXnew, matrix_size);

    memset(XXnew, 0, matrix_size);

}
