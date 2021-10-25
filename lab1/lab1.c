#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int flines(FILE *filemark){
    char c = fgetc(filemark);
    int counter = 1;
    while (c != EOF){
        if (c == '\n'){
            counter++;
        }
        c = fgetc(filemark);
    }
    return counter;

}
long fbytes(FILE *filemark){
    int c = fgetc(filemark);
    int counter = 0;
    while (c != EOF){
        counter++;
        c = fgetc(filemark);
    }
    return counter;
}
int fwords(FILE *filemark){
    int counter = 0;
    fscanf(filemark," ");
    while (!feof(filemark)){
        counter++;
        fscanf(filemark,"%*s ");
    }
    return counter;
}
int main(int argc, char *argv[]){
    FILE *file = fopen(argv[2], "r");
    if (argc != 3){
        printf("Error, invalid args");
        return 1;
    }
    if (file == NULL){
        printf("invalid file");
        return 2;
    }
    if (strcmp(argv[1], "-l") == 0 || strcmp(argv[1], "--lines") == 0){
         printf("Lines count: %d\n", flines(file));
    }
    else if(strcmp(argv[1], "-c") == 0 || strcmp(argv[1], "--bytes") == 0){
        printf("Bytes count: %d\n", fbytes(file));
    }
    else if(strcmp(argv[1], "-w") == 0 || strcmp(argv[1], "--words") == 0){
        printf("Words count: %d\n", fwords(file));
    }
    fclose(file);
}