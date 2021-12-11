#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <direct.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>

typedef struct {
    unsigned char info[62];
    int width;
    int height;
    int widwidth;
} Head;

void Life(Head h, bool** sh)
{
    bool result[h.height][h.widwidth];

    for (int i = h.height - 1; i >= 0; i--)
        for (int j = 0; j < h.widwidth; j++)
            result[i][j] = sh[i][j];
    //MAKING THE NEXT STEP
    for (int i = h.height - 1; i >= 0; i--)
    {
        for (int j = 0; j < h.width; j++)
        {
            int alive_cnt = 0;

            if (i != 0)
            {
                alive_cnt += (int) sh[i - 1][j];
                if (j != 0)
                    alive_cnt += (int) sh[i - 1][j - 1];
                if (j != h.width - 1)
                    alive_cnt += (int) sh[i - 1][j + 1];
            }

            if (i != h.height - 1)
            {
                alive_cnt += (int) sh[i + 1][j];
                if (j != 0)
                    alive_cnt += (int) sh[i + 1][j - 1];
                if (j != h.width - 1)
                    alive_cnt += (int) sh[i + 1][j + 1];
            }

            if (j != 0)
                alive_cnt += (int) sh[i][j - 1];

            if (i != h.width - 1)
                alive_cnt += (int) sh[i][j + 1];

            if (alive_cnt == 3)
                result[i][j] = (bool) 1;
            else if (alive_cnt == 2)
                result[i][j] = sh[i][j];
            else if (alive_cnt <= 1 || alive_cnt >= 4)
                result[i][j] = (bool) 0;
        }
    }
    //NEXT ITERATION
    for (int i = h.height - 1; i >= 0; i--)
    {
        for (int j = 0; j < h.widwidth; j++)
            sh[i][j] = result[i][j];
    }
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "Rus");

    FILE *datapic;
    char *outdir;
    int max_iter = 0;
    int freq = 1;

    for (int i = 1; i < argc; i ++)
    {
        //INPUT
        if (strstr(argv[i], "--input"))
        {
            datapic = fopen(argv[i + 1], "r");
            if (datapic != NULL)
                printf("OK\n");
            else
            {
                printf ("File has not been found\n");
                return -1;
            }
        }
        //OUTPUT
        if (strstr(argv[i], "--output"))
        {
            outdir = argv[i + 1];
            if (outdir != NULL)
                printf("OK\n");
            else
            {
                printf ("Directory has not been found\n");
                return -1;
            }
        }
        //NUMBER OF ITERATIONS
        if (strstr(argv[i], "--max_iter"))
            max_iter = atoi(argv[i + 1]);
        //NUMBER OF FREQUENCIES
        if (strstr(argv[i], "--dump_freq"))
            freq = atoi(argv[i + 1]);
    }

    //READING INFO
    Head head;
    for (int i = 0; i < 18; i++)
        head.info[i] = fgetc(datapic);

    //WIDTH
    head.width = 0;

    for (int i = 0; i < 4; i++)
    {
        char ch = fgetc(datapic);
        head.info[i + 18] = ch;
        head.width += (int) ((int) ch * pow(256, i));
    }

    if (head.width % 32 != 0)
        head.widwidth = head.width + 32 - head.width % 32;
    else
        head.widwidth = head.width;

    //HEIGHT
    head.height = 0;
    for (int i = 0; i < 4; i++)
    {
        char ch = fgetc(datapic);
        head.info[i + 22] = ch;
        head.height += (int) ((int) ch * pow(256, i));
    }

    for (int i = 0; i < 36; i++)
        head.info[i + 26] = fgetc(datapic);

    //
    bool **sheet;
    sheet = (bool **) malloc(head.height * sizeof(bool *));

    for (int i = 0; i < head.height; i++)
        sheet[i] = (bool *) malloc(head.widwidth * sizeof(bool *));

    for (int i = head.height - 1; i >= 0; i--)
    {
        for (int j = 0; j < head.widwidth; j += 8)
        {
            unsigned char nextbyte;
            nextbyte = fgetc(datapic);

            for (int  k = 7; k >= 0; k--)
            {
                sheet[i][j + k] = (bool) (nextbyte % 2);
                nextbyte /= 2;
            }
        }
    }

    //MAIN CYCLE
    for (int i = 1; i <= max_iter; i++)
    {
        Life(head, sheet);
        if (i % freq == 0)
        {
            char new_name[100];
            strcat(new_name, outdir);
            strcat(new_name, "\\");
            sprintf(new_name, "%d", i);
            strcat(new_name, ".bmp");

            FILE *outpic = fopen(new_name, "w");
            for (int j = 0; j < 62; j++)
                fputc(head.info[j], outpic);

            for (int j = head.height - 1; j >= 0; j--)
            {
                for (int k = 0; k < head.widwidth; k += 8)
                {
                    int nextobyte = 0;
                    for (int q = 0; q < 8; q++)
                        nextobyte += (int) sheet[j][k + q] * (int) pow(2, 7 - q);

                    unsigned char chbyte;
                    chbyte = nextobyte;
                    fputc(chbyte, outpic);
                }
            }
            fclose(outpic);
        }
    }
    fclose(datapic);
}
