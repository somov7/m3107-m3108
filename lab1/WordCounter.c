//
//  WordCounter.c
//  WordCount
//
//  Created by Антон Чемодуров on 11.09.2021.
//
#include <stdio.h>
#include <stdlib.h>

#include "WordCounter.h"

FILE *file;

//const int buffer_size = 1024;
//char *buffer;

int c;

int character_counter = 0;


int is_white_space(void) {
    return (test(' ') || test('\t') || test('\r'));
}


void init(FILE *file_) {
    file = file_;
}


_Bool test(int t) {
    if (c == t) {
        next_char();
        return 1;
    } else {
        return 0;
    }
//    return c == t ? (1, next_char()) : 0;
}

_Bool _test(int t) {
    if (c != t) {
        next_char();
        return 1;
    } else {
        return 0;
    }
}

int next_char(void) {
    return c != EOF ? (c = getc(file), character_counter++) : EOF;
}


//int next_char(void) {
//    static char is_eof = 0;
//    static int i = buffer_size;
//    static size_t size = buffer_size;
//    if (i == size) {
//        buffer = (char *)malloc(buffer_size * sizeof(char));
//        size = fread(buffer, sizeof(char), buffer_size, file);
//        if (size != buffer_size && ferror(file)) {
//            perror("error while reading file");
//            is_eof = 1;
//            return EOF;
//        }
//        i = 0;
//    }
//
//    if (size == i && feof(file)) is_eof = 1;
//    return is_eof ? EOF : (c = buffer[i++]);
//}


void skipe_white_space(void) {
    while (is_white_space());
}


file_info get_file_info(FILE *file_) {
    file = file_;
    
    file_info info = {0, 0, 0};
    while (c != EOF) {
        skipe_white_space();
        if (test('\n')) {
            info.lines_num++;
            while (test('\n')) info.lines_num++;
        } else {
            while (_test('\n') && !is_white_space()) {
                if (c == EOF) {
                    info.lines_num++;
                    break;
                }
            }
            info.words_num++;
        }
    }
    info.size = (character_counter - 1)* sizeof(char);
    return  info;
}
