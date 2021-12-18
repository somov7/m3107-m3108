#include <stdio.h>
#include <string.h>
#include <direct.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>

typedef struct{
    unsigned char info[62];
    int width;
    int widwidth;
    int height;
}Head;


void lifecycle(Head h, bool** sh){
    bool result[h.height][h.widwidth];

    for (int i = h.height - 1; i >= 0; i--) {
        for (int j = 0; j < h.widwidth; j++) {
            result[i][j] = sh[i][j];
        }
    }

    for (int i = h.height - 1; i >= 0; i--) {
        for (int j = 0; j < h.width; j++) {
            int neibrs = 0;
            if (i != 0){
                neibrs += (int) sh[i - 1][j];
                if (j != 0){
                    neibrs += (int) sh[i - 1][j - 1];
                }
                if (j != h.width - 1){
                    neibrs += (int) sh[i - 1][j + 1];
                }
            }
            if (i != h.height - 1){
                neibrs += (int) sh[i + 1][j];
                if (j != 0){
                    neibrs += (int) sh[i + 1][j - 1];
                }
                if (j != h.width - 1){
                    neibrs += (int) sh[i + 1][j + 1];
                }
            }
            if (j != 0){
                neibrs += (int) sh[i][j - 1];
            }
            if (i != h.width - 1){
                neibrs += (int) sh[i][j + 1];
            }

            if (neibrs == 3){
                result[i][j] = (bool) 1;
            }
            if (neibrs <= 1 || neibrs >= 4){
                result[i][j] = (bool) 0;
            }
            if (neibrs == 2){
                result[i][j] = sh[i][j];
            }
        }
    }
//    printf("Pic\n");
    for (int i = h.height - 1; i >= 0; i--) {

        for (int j = 0; j < h.widwidth; j++) {
//            printf("%d ", result[i][j]);
            sh[i][j] = result[i][j];
        }
//        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    FILE *datapic;
    char *outdir;
    int max_iter = 0;
    int freq = 1;
    for (int i = 1; i < argc; i ++) {

        if (strstr(argv[i], "--i")){
            datapic = fopen(argv[i + 1], "r");

            if (datapic == NULL) {
                printf ("%s", "error (there is no file!!)\n");
                return -1;
            }

        }
        if (strstr(argv[i], "--o")){

            outdir = argv[i + 1];
            if (_mkdir(outdir) != 0){
                printf ("%s", "error (smth wrong with directory)\n");
                return -1;
            }
        }

        if (strstr(argv[i], "--m")){
            max_iter = atoi(argv[i + 1]);
        }

        if (strstr(argv[i], "--d")){
            freq = atoi(argv[i + 1]);
        }
    }

    Head head;
    for (int i = 0; i < 18; i++){ // с 0 до 18 хранится информация которую мы никак не используем
        head.info[i] = fgetc(datapic);
    }

    head.width = 0;
    for (int i = 0; i < 4; i ++){ // вот тут по четыре бита на ширину и высоту
        char ch = fgetc(datapic);
        head.info[i + 18] = ch;
        head.width += (int) ((int) ch * pow(256, i)); // почему они опять записаны в обратном порядке кто знает
    }
    if (head.width % 32 != 0){
        head.widwidth = head.width + 32 - head.width % 32;
    } else{
        head.widwidth = head.width;
    }
     // файлы всегда округляются до целого количества байт


    head.height = 0;
    for (int i = 0; i < 4; i ++){
        char ch = fgetc(datapic);
        head.info[i + 22] = ch;
        head.height += (int) ((int) ch * pow(256, i));
    }


    for (int i = 0; i < 36; i++){
        head.info[i + 26] = fgetc(datapic); // опять неинтересно
    }

    bool **sheet;
    sheet = (bool **) malloc(head.height * sizeof(bool *));
    for (int i = 0; i < head.height; i++) {
        sheet[i] = (bool *) malloc(head.widwidth * sizeof(bool *));
    }
    for (int i = head.height - 1; i >= 0; i--) {
        for (int j = 0; j < head.widwidth; j += 8) {
            unsigned char nextbyte;
            nextbyte = fgetc(datapic);
            for (int  k = 7; k >= 0; k--){
                sheet[i][j + k] = (bool) (nextbyte % 2);
                nextbyte /= 2;
            }
        }
    }

    for (int i = 1; i <= max_iter; i++){
        lifecycle(head, sheet);
        if (i % freq == 0){
            char newname[100];
            strcat(newname, outdir);
            strcat(newname, "\\");
            sprintf(newname, "%d", i);
            strcat(newname, ".bmp");

            FILE *outpic;
            outpic = fopen(newname, "w");

            for (int j = 0; j < 62; j++){
                fputc(head.info[j], outpic);
            }

            for (int j = head.height - 1; j >= 0; j--) {
                for (int k = 0; k < head.widwidth; k += 8) {

                    int nextobyte = 0;
                    for (int q = 0; q < 8; q++) {
                        nextobyte += (int) sheet[j][k + q] * (int) pow(2, 7 - q);
                    }
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
