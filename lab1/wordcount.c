#include <stdio.h>


void show_lines_count();
void show_bytes_size();
void show_words_count();

FILE* input_file;

int main(int argc, char** argv){
    input_file = fopen(argv[2], "r");

    if (input_file == NULL){
        printf("Could not open the file %s\n", argv[1]);
    return 1;
    } 
    
    if(strcmp(argv[1],"-l") == 0 || strcmp(argv[1],"--lines") == 0)
        show_lines_count();
    else if(strcmp(argv[1],"-c") == 0 || strcmp(argv[1],"--bytes") == 0)
        show_bytes_size();
    else if(strcmp(argv[1],"-w") == 0 || strcmp(argv[1],"--words") == 0)
        show_words_count();
    else{
        printf("Enter the right mode\n");
    }
    fclose(input_file);
    return 0;
}


void show_lines_count(){
    char ch;
    int line_count = 1;
    if(ch = fgetc(input_file) == EOF){
        printf("Lines: 0");
        return;
    }
    
    while((ch = fgetc(input_file)) != EOF){        
        if(ch == '\0' || ch == '\n')
                line_count++;
        
    }

    printf("Lines: %d.\n", line_count);
}



void show_bytes_size(){
    char ch;
    int char_count = 0;
    while((ch = fgetc(input_file)) != EOF){
        char_count++;
    }

    printf("Byte size: %d.\n", char_count);
    
}

void show_words_count(){
    char ch;
    int in_word = 0;
    int word_count = 0;
    
    while((ch = fgetc(input_file)) != EOF){
        if(ch == ' ' || ch == '\t' || ch == '\0' || ch == '\n'){
            if(in_word){
                in_word = 0;
                word_count++;
            }
        } else {
            in_word = 1;
        }
    }
    printf("Words: %d.\n", word_count);
}