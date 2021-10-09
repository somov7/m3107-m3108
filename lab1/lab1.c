#include <stdio.h>
#include <string.h>
//#include <io.h>

int main(int argc, char *argv[]) {

    if (argc != 3){
        printf("ERROR, insufficient number of parameters");
        return -1;
    }
    
    //argv[1] is option
    //argv[2] is path

    if (DoesFileExist(argv[2]) == 0){
        printf("ERROR, cannot find a file");
        return -2;
    }
    
    if (strcmp(argv[1], "-l") == 0 || strcmp(argv[1], "--lines") == 0)
        printf("%d",linesCounter(argv[2]));  // ---------------------------------> count of lines
    else if (strcmp(argv[1], "-c" ) == 0 || strcmp(argv[1], "--bytes") == 0)
        printf("%d",bytesCounter(argv[2])); // ---------------------------------> count of filesize
    else if (strcmp(argv[1], "-w") == 0 || strcmp(argv[1], "--words") == 0)
        printf("%d",wordsCounter(argv[2])); // ---------------------------------> count of words
    else
        printf("ERROR, incorrect command");

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
    return outVal + 1; // +1 - cause need to count last line

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

int DoesFileExist(char* p){ // - path
    FILE* activeFile = fopen(p,"r");
    if (activeFile == NULL)
        return 0;
    else
        return 1;

}