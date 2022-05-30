#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "heap.h"


Node* Build_Huffman(Vector* v)
{
    Node *left, *right, *top;
    while (v->sz != 1)
    {
        left = extractmin(v);
        right = extractmin(v);
        top = Node_new('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        Node_insert(v, top);
    }
    return extractmin(v);
}

int isLeaf(Node* x)
{
    return (x->left == NULL) && (x->right == NULL);
}

int arr[500];
char* bytetobit[256]; //коды для каждого байта
char** FILE_NAMES = NULL; //файлы что нужно запаковать
char* ARC_NAME = NULL; //название архива
int FILE_COUNT = 0; //количество файлов для архивации
int HEADER_SIZE = 0; //размер всех метаданных
long long freq[256]; //частота байтов
long long* FILE_SIZES = NULL; //размеры закодированных файлов
char* PADDING_BITS = NULL; //сколько битов скипаем в последнем байте файла

void printHuff(Node* x, int arr[], int top)
{
    if (isLeaf(x))
    {
        bytetobit[x->byte] = malloc(top+1);
        for (int i = 0; i < top; i++)
            bytetobit[x->byte][i] = (arr[i] ? '1' : '0');
        bytetobit[x->byte][top] = '\0';
        return;
    }
    if (x->left)
    {
        arr[top] = 0;
        printHuff(x->left, arr, top+1);
    }
    if (x->right)
    {
        arr[top] = 1;
        printHuff(x->right, arr, top+1);
    }

}

void ReadAndWrite(FILE* r, long long filesize, FILE* w, Node* x, int padding)
{
    int c;
    Node* t = x;
    long long bits = 0;
    for (int i = 0; i < filesize; i++)
    {
        c = fgetc(r);
        for (int count = 0; count < 8; count++)
        {
            if (++bits > 8 * filesize - padding) //прочитали все биты
                break;
            int bit = (c & (1 << 7-count));
            if (bit) t = t->right;
            else t = t->left;
            if (isLeaf(t))
            {
                fwrite(&t->byte, 1, 1, w);
                t = x;
            }
        }
    }
}

void BuildHeapAndHuffmanAndWrite(char* filename, int ind, FILE* wr)
{
    memset(freq, 0, sizeof(freq));
    memset(arr, 0, sizeof(arr));
    memset(bytetobit, 0, sizeof(bytetobit));
    FILE* f = fopen(filename, "rb");
    if (!f)
    {
        printf("Cant open file");
        exit(1);
    }
    Vector* v = Vector_new();
    int c;
    while ((c = fgetc(f)) != EOF)
        freq[c]++; //считаем частоты символов

    for (int i = 0; i < 256; i++)
        if (freq[i] > 0)
            Vector_push(v, Node_new(i, freq[i])); //добавим в вектор вершины с частотой > 0
    buildheap(v); //построим мин-кучу из вектора
    Node* huff = Build_Huffman(v); //построим из кучи дерево Хаффмена
    int top = 0;
    printHuff(huff,arr,top); //получим коды для символов
    fwrite(freq, sizeof(long long), 256, wr); //запишем частоты, чтобы при декодировании построить дерево Хаффмена
    int bit_count = 0;
    char write_byte = 0;
    fseek(f, 0, 0);
    while((c = fgetc(f)) != EOF)
    {
        char* code = bytetobit[c];
        for (int i = 0; i < strlen(code); i++)
        {
            if (code[i] == '1') write_byte |= (1 << (7-bit_count)); //формируем байт для записи
            bit_count++;
            if (bit_count == 8)
            {
                fwrite(&write_byte, 1, 1, wr);
                FILE_SIZES[ind]++;
                bit_count = 0;
                write_byte = 0;
            }
        }
    }
    
    if (bit_count) //не записали последний байт
    {
        fwrite(&write_byte, 1, 1, wr);
        FILE_SIZES[ind]++;
        PADDING_BITS[ind] = 8 - bit_count;
    }

    fclose(f);
    Vector_free(v);
    Node_free(huff);
}

void CreateArc()
{
    FILE* arc = fopen(ARC_NAME, "wb");
    if (!arc)
    {
        printf("Can't create file");
        exit(1);
    }
    FILE_SIZES = calloc(FILE_COUNT, sizeof(long long));
    PADDING_BITS = calloc(FILE_COUNT, 1);
    for (int i = 0; i < FILE_COUNT; i++)
    {
        BuildHeapAndHuffmanAndWrite(FILE_NAMES[i], i, arc);
    }
    //HEADER
    //LAST 8 BYTES = header size + file count
    int total_strlen = 0;
    for (int i = 0; i < FILE_COUNT; i++)
    {
        fwrite(&FILE_SIZES[i], sizeof(long long), 1, arc); //пишем размер файла (8 байт)
        fwrite(&PADDING_BITS[i], sizeof(char), 1, arc);  //падинг биты в ласт байте файла (1 байт)
        short len = strlen(FILE_NAMES[i]);
        total_strlen += len;
        fwrite(&len, sizeof(short), 1, arc); //длину строки с названием файла (2 байта)
        fwrite(FILE_NAMES[i], 1, len, arc); //название файла
    }
    HEADER_SIZE = total_strlen + FILE_COUNT * 11 + 8;
    fwrite(&HEADER_SIZE, sizeof(int), 1, arc);
    fwrite(&FILE_COUNT, sizeof(int), 1, arc);
    //для каждого файла его размер (8 байт), паддинг биты в последнем байте (1 байт)
    //длина строки названия файла (2 байта), всего 11 на каждый
    fclose(arc);
}

void ListAndExtractArc(int extract)
{
    FILE* f = fopen(ARC_NAME, "rb");
    if (!f)
    {
        printf("Cant open file");
        exit(1);
    }
    fseek(f, -8, SEEK_END);
    fread(&HEADER_SIZE, sizeof(int), 1, f);
    fread(&FILE_COUNT, sizeof(int), 1, f);
    fseek(f, -HEADER_SIZE, SEEK_END);
    FILE_NAMES = malloc(FILE_COUNT*sizeof(char*));
    FILE_SIZES = calloc(FILE_COUNT, sizeof(long long));
    PADDING_BITS = calloc(FILE_COUNT, 1);
    for (int i = 0; i < FILE_COUNT; i++)
    {
        fread(&FILE_SIZES[i], sizeof(long long), 1, f);
        fread(&PADDING_BITS[i], 1, 1, f);
        short len;
        fread(&len, sizeof(short), 1, f);
        FILE_NAMES[i] = malloc(len+1);
        fread(FILE_NAMES[i], 1, len, f);
        FILE_NAMES[i][len] = '\0';
    }
    if (extract == 0)
    {
        for (int i = 0; i < FILE_COUNT; i++)
            printf("File %d: Name: %s Size: %lld\n", i+1, FILE_NAMES[i], FILE_SIZES[i]);
        return;
    }
    fseek(f, 0, 0);
    for (int i = 0; i < FILE_COUNT; i++)
    {
        FILE* w = fopen(FILE_NAMES[i], "wb");
        if (!w)
        {
            printf("Cant create file");
            exit(1);
        }
        memset(freq, 0, sizeof(long long) * 256);
        fread(freq, sizeof(long long), 256, f);
        Vector* v = Vector_new();
        for (int i = 0; i < 256; i++)
            if (freq[i] > 0)
                Vector_push(v, Node_new(i, freq[i])); //добавим в вектор вершины с частотой > 0
        buildheap(v); //построим мин-кучу из вектора
        Node* huff = Build_Huffman(v); //построим из кучи дерево Хаффмена
        ReadAndWrite(f, FILE_SIZES[i], w, huff, PADDING_BITS[i]);
        Vector_free(v);
        Node_free(huff);
    }
}

int main(int argc, char* argv[])
{
    if (argc <= 3)
    {
        printf("Correct usage: \n--file <name.arc> --extract\n--file <name.arc> --list \n--file <name.arc> --create <file1> <file2> ... <filen>\n");
        return 1;
    }
    int extract = 0, list = 0, create = 0;
    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "--file"))
        {
            if (i+1<argc) 
            {
                ARC_NAME = (char*)malloc(strlen(argv[i+1])+1);
                strcpy(ARC_NAME, argv[i+1]);
            }
            else 
            {
                printf("Wrong format");
                return 1;
            }
        }
        else if (!strcmp(argv[i], "--extract"))
        {
            extract = 1;
        }
        else if (!strcmp(argv[i], "--list"))
        {
            list = 1;
        }
        else if (!strcmp(argv[i], "--create"))
        {
            create = 1;
        }
        else if(!strstr(argv[i], ".arc")) //если не аргумент --create --list --extract и не название архива, что читаем название свободных файлов
        {
            if (FILE_NAMES == NULL) FILE_NAMES = (char**)malloc((argc-4)*sizeof(char*)); //если еще не создали, создаим массив строк с названиями файлов
            int len = strlen(argv[i]);
            FILE_NAMES[FILE_COUNT] = (char*)malloc(len);
            strcpy(FILE_NAMES[FILE_COUNT], argv[i]);
            FILE_COUNT++;
        }
    }

    if (create) CreateArc();
    else if (extract) ListAndExtractArc(1);
    else if (list) ListAndExtractArc(0);


    return 0;
}