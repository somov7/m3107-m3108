#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>


typedef struct{
    unsigned char info[10];
    unsigned char vers[2];
    unsigned char flags;
    unsigned int size;
}Head;

typedef struct {
    unsigned char info[10];
    unsigned char name[5];
    unsigned int size;
    unsigned char flags[2];
}Frame;


void show(FILE *data){

    fseek(data, 10, SEEK_SET);

    Frame nextframe;
    strcpy(nextframe.name, "     \0");
    nextframe.size = 0;
    int n = 1;
    while(n < 7){

        fread(nextframe.info, 1, 10, data);

        strncpy(nextframe.name, nextframe.info, 4);

        int k = 3;
        for (int i = 0; i < 4; i++, k--){
            nextframe.size += (int) nextframe.info[i + 4] * (int) pow(256, k);
        }

        printf("%d frame with name %s\n", n, nextframe.name);
        printf("its size %d\n", nextframe.size);
        for (int i = 0; i < nextframe.size; i++){
            printf("%c", fgetc(data));
        }

        printf("\n\n");
        n++;

        strcpy(nextframe.info, "");
        strcpy(nextframe.name, "");
        nextframe.size = 0;
    }
}

int get(FILE *data, Frame frame){

    frame.name[4] = '\0';

    fseek(data, 10, SEEK_SET);

    Frame nextframe;
    strcpy(nextframe.name, "     \0");
    nextframe.size = 0;

    int n = 1;
    while(n < 7){
        frame.size = 0;
        nextframe.size = 0;

        fread(nextframe.info, 1, 10, data);

        strncpy(nextframe.name, nextframe.info, 4);
        nextframe.name[4] = '\0';

        int k = 3;
        for (int i = 0; i < 4; i++, k--){
            nextframe.size += (int) nextframe.info[i + 4] * (int) pow(256, k);
        }


        if (strcmp(frame.name, nextframe.name) == 0){
            printf("you found %d frame with name %s\n", n, nextframe.name);
            printf("it has size %d\n", nextframe.size);

            for (int i = 0; i < nextframe.size; i++){
                printf("%c", fgetc(data));
            }
            return 0;

        } else {
            fseek(data, (long) nextframe.size, SEEK_CUR);
        }

        n++;
        strcpy(nextframe.info, "");
        strcpy(nextframe.name, "");
        nextframe.size = 0;
    }
    return -1;
}


void set(char *dataname, Frame frame, char *content){
    FILE* data = fopen(dataname, "rb+");
    frame.name[4] = '\0';

    fseek(data, 0, SEEK_SET);
    FILE *newfile;
    newfile = fopen("temp.mp3", "wb+");
    Frame nextframe;
    nextframe.size = 0;
    for (int i = 0; i < 10; i++){
        fputc(fgetc(data), newfile);
    }
    int n = 1;
    while(n < 7){
        fread(nextframe.info, 1, 10, data);

        for (int i = 0; i < 4; i++){
            nextframe.name[i] = nextframe.info[i];
        }
        nextframe.name[4] = '\0';

        int k = 3;
        for (int i = 0; i < 4; i++, k--){
            printf("%c", nextframe.info[i + 4]);
            nextframe.size += (int) nextframe.info[i + 4] * (int) pow(256, k);
        }
        printf("\n");
        printf("size %d\n", nextframe.size);

        printf("%s %s\n", frame.name, nextframe.name);

        if (strcmp(frame.name, nextframe.name) == 0){
            printf("found it!!!\n");

            for (int i = 0; i < 2; i ++){ //flags
                frame.info[i + 8] = nextframe.info[i + 8];
            }

            for (int i = 0; i < 10; i++){
                fputc(frame.info[i], newfile);
            }

            fputc(fgetc(data), newfile);

            for (int i = 0; i < frame.size - 1; i++){
                fputc(content[i], newfile);
            }
            fseek(data, (long) nextframe.size - 1, SEEK_CUR);

        } else {
            for (int i = 0; i < 10; i++){
                fputc(nextframe.info[i], newfile);
            }
            for (int i = 0; i < nextframe.size; i++){
                char nextchar = fgetc(data);
                fputc(nextchar, newfile);
            }
        }
        strcpy(nextframe.info, "");
        strcpy(nextframe.name, "");
        nextframe.size = 0;
        n++;
        printf("\n");
    }

    char nextchar;
    nextchar = getc(data);

    while(!feof(data)){
        putc(nextchar, newfile);
        nextchar = getc(data);
    }
    fclose(data);
    fclose(newfile);
    FILE *input = fopen("temp.mp3", "rb+");
    FILE *output = fopen(dataname, "wb+");

    nextchar = getc(input);
    while(!feof(input)){
        putc(nextchar, output);
        nextchar = getc(input);
    }
}


int main(int argc, char* argv[]){
    freopen("out.txt","w",stdout);

    if (argc < 1 || argc > 4) {
        printf("%s", "wdy want....\n");
        return -1;
    }

    FILE *data;
    char dataname[40];

    Head head;

    for (int i = 1; i < argc; i++) {

        if (strstr(argv[i], "--filepath=")) {

            strcpy(dataname, argv[i]);
            memmove(dataname, dataname+11, strlen(dataname));

            data = fopen(dataname, "rb+");
            printf("%s\n", dataname);

            if (data == NULL) {
                printf ("%s", "error (there is no file!!)\n");
                return -1;
            }
            fread(head.info, 1, 10, data);
            printf("head:\n");
            for (int j = 0; j < 10; j++){
                printf("%c", head.info[j]);
            }
            printf("\n\n");


        } else if (strstr(argv[i], "--show")){
            show(data);

        } else if (strstr(argv[i], "--get=")){
            Frame sekframe;
            char frname[4];
            for (int j = 6; j < strlen(argv[i]); j++){

                if (j > 9){
                    printf ("%s", "error (framename too long)\n");
                    return -1;
                }

                frname[j - 6] = argv[i][j];
            }
            strncpy(sekframe.name, frname, 4);
            sekframe.name[4] = '\0';

            printf("seeking for %s\n", sekframe.name);
            sekframe.size = 0;

            if(get(data, sekframe) != 0){
                printf("error (there is no frame with this ID :(( sorry)");
                return -1;
            }
        } else if (strstr(argv[i], "--set=")){

            Frame changframe;
            char frname[12];
            char content[strlen(argv[i])];

            strcpy(content, argv[i]);
            memmove(content, content+6, strlen(content));
            strcpy(frname, argv[i + 1]);
            memmove(frname, frname+8, strlen(frname));

            for (int j = 0; j < 4; j++){
                changframe.name[j] = frname[j];
                changframe.info[j] = frname[j];
            }

            changframe.size = strlen(content) + 1;

            int k = 3;
            for (int j = 0; j < 4; j++, k--){
                changframe.info[j + 4] = changframe.size / (int) pow (256, k);
            }

            printf("frame to change %s\n", changframe.name);
            printf("to content %s\n", content);
            printf("its size %d\n\n", changframe.size);
            set(dataname, changframe, content);

        }

    }
    fclose(data);
    return 0;
}
