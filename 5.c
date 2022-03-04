#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

char* arr;
int size = 8;
int x, y;
int image_path;
int bit_span;
int byte_span;

FILE* fin;
FILE* fout;

int n=-1, inp=-1, out=-1, dumb_freg=-1;

int get_O(int X, int Y)
{
    return (X - 1) + (Y - 1) * x;
}
void print()
{
    for (int j = y; j >= 1; j--)
    {
        for (int i = 1; i <= x; i++)
        {
            printf("%i ", arr[get_O(i, j)]);
        }
        printf("\n");
    }
}
void check_color()
{
    int color = 0;
    fseek(fin, 59, SEEK_SET);
    fread(&color, 1, 1, fin);
    if (color != 0)
    {
        for (int j = 1; j <= y; j++)
            for (int i = 1; i <= x - bit_span; i++)
                arr[get_O(i, j)] = (arr[get_O(i, j)] - 1) * (-1);
    }
}
void get_10_2(int data)
{
    for (int i = size - 1; i >= size - 8; i--)
    {
        arr[i] = data % 2;
        data /= 2;
    }
    size += 8;
    arr = realloc(arr, size);
}
int get_2_10(int k, int Y)
{
    int data = 0;
    int j = 1;
    for (int i = x - k * 8; i > x - 8 - 8 * k; i--)
    {
        if (arr[get_O(i, Y)] == 1)
            data += j;
        j *= 2;
    }
    // printf("%i ", data);
    return data;
}
void scan_file()
{
    //Узнаём длину
    fseek(fin, 18, SEEK_SET);
    fread(&x, 1, 2, fin);

    //Узнаём высоту
    fseek(fin, 22, SEEK_SET);
    fread(&y, 1, 2, fin);

    //Узнаём количество лишних бит
    bit_span = (8 - x % 8) % 8;
    //узнаём настоящую длину строки
    x += bit_span;
    //узнаём кл-во промежуточных байтов
    byte_span = (4 - (x / 8 % 4)) % 4;
    // printf("\n|%i %i %i|\n",x,  byte_span, bit_span);
    //Узнаём место начала изображения
    fseek(fin, 10, SEEK_SET);
    fread(&image_path, 1, 1, fin);
    //считываем изображение
    fseek(fin, image_path, SEEK_SET);
    for (int j = 0; j < y; j++)
    {
        for (int i = 0; i < x / 8; i++)
        {
            int data = 0;
            fread(&data, 1, 1, fin);
            get_10_2(data);
        }
        fseek(fin, byte_span, SEEK_CUR);
    }
    //проверяем палитру
    check_color();
}
void print_file()
{
    char ch;
    fseek(fin, 0, SEEK_SET);
    fseek(fout, 0, SEEK_SET);
    while (!feof(fin))
    {
        ch = getc(fin);
        putc(ch, fout);
    }
    fseek(fout, image_path, SEEK_SET);
    for (int i = 1; i <= y; i++)
    {
        for (int k = x / 8 - 1; k >= 0; k--)
        {
            putc(get_2_10(k, i), fout);
        }
        fseek(fout, byte_span, SEEK_CUR);
    }
}
bool check(int X, int Y)
{
    int lifes = 0;
    if (X > 1 && arr[get_O(X - 1, Y)] == 1)
        lifes++;
    if (X > 1 && Y < y && arr[get_O(X - 1, Y + 1)] == 1)
        lifes++;
    if (Y < y && arr[get_O(X, Y + 1)] == 1)
        lifes++;
    if (X < x - bit_span && Y < y && arr[get_O(X + 1, Y + 1)] == 1)
        lifes++;
    if (X < x - bit_span && arr[get_O(X + 1, Y)] == 1)
        lifes++;
    if (X < x - bit_span && Y>1 && arr[get_O(X + 1, Y - 1)] == 1)
        lifes++;
    if (Y > 1 && arr[get_O(X, Y - 1)] == 1)
        lifes++;
    if (X > 1 && Y > 1 && arr[get_O(X - 1, Y - 1)] == 1)
        lifes++;
    if (arr[get_O(X, Y)] == 1 && (lifes == 2 || lifes == 3))
        return 1;
    else if (lifes == 3)
        return 1;
    else
        return 0;
}
void go_life()
{
    if (n==-1)
        n=1000000;
    
    int loading=n/20; 
    int load_dash=20;
    if (n<20)
        loading = 20/n;
    printf("loading:\n");
    char a = 177, b = 219;
    for (int i = 0; i < 20; i++)
        printf("%c", a);
    printf("\r");
    for (int k = 0; k < n; k++)
    {
        char* arr_clone = malloc(x * y);
        for (int i = 0; i < x * y; i++)
            arr_clone[i] = 0;

        for (int j = 1; j <= y; j++)
        {
            for (int i = 1; i <= x; i++)
            {
                arr_clone[get_O(i, j)] = check(i, j);
            }
        }
        

        bool uniformity=true;
        if (n==1000000)
        {
            for (int i = 0; i < x * y; i++)
                if (arr[i] != arr_clone[i])
                    {
                        uniformity=false;
                        break;
                    }
            if (uniformity)
                break;
        }

        for (int i = 0; i < x * y; i++)
            arr[i] = arr_clone[i];
        
        if(k%loading==0 && load_dash>1 && n>=20)
        {
            printf("%c", b);
            load_dash--;
        }
        if(n<20)
        {
            for (int i=0; i<loading; i++)
            {
                if (load_dash<=1)
                    break;
                printf("%c", b);
                load_dash--;
            }

        }
    }
    for (int i=0; i<load_dash-1; i++)
        printf("%c", b);
    printf("%c", b);
    printf("\n");
    check_color();
}
bool check_args()
{
    if(inp==-1)
    {
        printf("NO INPUT FILE ENTERED\n--ERROR--\n____________________\n");
        return 1;
    }
    if(out==-1)
    {
        printf("NO OUTPUT FILE ENTERED\n--ERROR--\n____________________\n");
        return 1;
    }
    if(n==-1)
        printf("* WARNING! NO MAX ITERATOR\n");
    if(dumb_freg==-1)
        printf("* WARNING! NO DUMB FREG. IT WILL BE 1\n");
    return 0;
}
int main(int argc, char** argv)
{
    int i=1;
    bool error=false;
    printf("____________________\n");
    while (i<argc)
    {
        if (strcmp(argv[i], "--input")==0)
        {
            fin = fopen(argv[i+1], "rb");
            if (fin)
                printf("INPUT FILE: %s\n", argv[i+1]);
            else
            {
                error=true;
                printf("NOT FOUND FILE\n");
            }
            inp=i+1;
        }
        if (strcmp(argv[i], "--output")==0)
        {
            fout = fopen(argv[i+1], "wb");
            if (fin)
                printf("OUTPUT FILE: %s\n", argv[i+1]);
            else
            {
                error=true;
                printf("ERROR OUTPUT FILE\n");
            }
            out=i+1;
        }
        if (strcmp(argv[i], "--max_iter")==0)
        {
            n=0;
            for (int j=0; j<strlen(argv[i+1]); j++)
                n=n*10+argv[i+1][j]-'0';
            printf("MAX ITERATOR: %i\n", n);
        }
        if (strcmp(argv[i], "--dumb_freg")==0)
        {
            dumb_freg=0;
            for (int j=0; j<strlen(argv[i+1]); j++)
                dumb_freg=dumb_freg*10+argv[i+1][j]-'0';
            printf("DUMB FREG: %i\n", dumb_freg);
        }
        i+=2;
    }
    if (error)
    {
        printf("--ERROR--\n____________________\n");
        return 0;
    }
    arr=malloc(size);
    if(check_args())
        return 0;
    scan_file();
    go_life(n);
    print_file();
    printf("--NICE--\n____________________\n");
    return 0;
}
