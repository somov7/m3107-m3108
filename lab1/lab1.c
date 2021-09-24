#include <stdio.h>
#include <string.h>
//#include <io.h>

int main(int argc, char *argv[]) {

    int i = 0,  outValue = 0, isWord = 0; // i - value to run on the lines
    char option[15], path[100];

    
    strcpy(option, argv[1]);
    strcpy(path, argv[2]);

    FILE* ftext = fopen(path,"r");
    
    if (strcmp(option, "-l") == 0 | strcmp(option, "--lines") == 0){   
        printf("%d",linesCounter(ftext)+1);  // ---------------------------------> count of lines (+1 - cause need to count last line)
    }
    if (strcmp(option, "-c" ) == 0 | strcmp(option, "--bytes") == 0){
        printf("%d",bytesCounter(ftext, path)); // ---------------------------------> count of filesize
    }
    if (strcmp(option, "-w") == 0 | strcmp(option, "--words") == 0){
        printf("%d",wordsCounter(ftext));         // ---------------------------------> count of words
    }

    fclose(ftext);

}

int linesCounter(FILE* f){ // f - active file
    int outVal=0;
    char c;
        while ((c = getc(f)) != EOF) {
                if (c=='\n')
                    outVal++;

    }
    return outVal;

}

int wordsCounter(FILE* f){ // f - active file
    int outVal = 0,isWord = 0;
    char c = getc(f);
    while ((c == ' ') & (c != '\0'))
            c = getc(f);

    while ((c = getc(f)) != EOF) {

            //isWord = 0; // boolean value (0 - false/1 - true)
        if ((c != ' ') & (isWord == 0)){
            outVal++;
            isWord = 1;
        }
        else if (c == ' ' | c == '\n')
            isWord = 0;
        }
        return outVal;
    }

    int bytesCounter(FILE* f, char* p){ // f - active file, p - path
        int outVal;
        f = fopen(p,"rb");

        fseek(f, 0, SEEK_END);
        outVal = ftell(f);
        fseek(f, 0, SEEK_SET); 

        fclose(f);  

        return outVal;      

    }