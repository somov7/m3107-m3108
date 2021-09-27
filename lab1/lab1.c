#include <stdio.h>
#include <string.h>
//#include <io.h>

int main(int argc, char *argv[]) {

    int i = 0; // i - value to run on the lines
    char option[15], path[255];

    if (argc != 3){
        printf("ERROR, insufficient number of parameters");
        return -1;
    }
    
    strcpy(option, argv[1]);
    strcpy(path, argv[2]);

    if (fileIsExist(path) == 0){
        printf("ERROR, cannot find a file");
        return -2;
    }
    
    if (strcmp(option, "-l") == 0 || strcmp(option, "--lines") == 0){
        printf("%d",linesCounter(path)+1);  // ---------------------------------> count of lines (+1 - cause need to count last line)
    }
    if (strcmp(option, "-c" ) == 0 || strcmp(option, "--bytes") == 0){
        printf("%d",bytesCounter(path)); // ---------------------------------> count of filesize
    }
    if (strcmp(option, "-w") == 0 || strcmp(option, "--words") == 0){
        printf("%d",wordsCounter(path)); // ---------------------------------> count of words
    }

    return 0;
}

int linesCounter(char* p){ // p - path
    int outVal=0;
    char c;
    FILE* f = fopen(p,"r"); // f - active file
        while ((c = getc(f)) != EOF) {
                if (c=='\n')
                    outVal++;

    }
    fclose(f);
    return outVal;

}

int wordsCounter(char* p){ // p - path
    int outVal = 0,isWord = 0;
    FILE* f = fopen(p,"r"); // f - active file

    char c = getc(f); 
    while ((c == ' ') && (c != '\0'))
            c = getc(f);

    while ((c = getc(f)) != EOF) {

            //isWord = 0; // boolean value (0 - false/1 - true)
        if ((c != ' ') && (c != '\n') && (c != '\t') && (isWord == 0)){
            outVal++;
            isWord = 1;
        }
        else if (c == ' ' || c == '\n')
            isWord = 0;
        }

        fclose(f);
        return outVal;
    }

int bytesCounter(char* p){ // f - active file, p - path
    int outVal;
    FILE* f = fopen(p,"rb");

    fseek(f, 0, SEEK_END);
    outVal = ftell(f);

    fclose(f);  

    return outVal;      

    }

int fileIsExist(char* p){ // - path
    FILE* activeFile = fopen(p,"r");
    if (activeFile == NULL)
        return 0;
    else
        return 1;

}