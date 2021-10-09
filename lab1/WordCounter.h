//
//  WordCounter.h
//  WordCount
//
//  Created by Антон Чемодуров on 11.09.2021.
//

#ifndef WordCounter_h
#define WordCounter_h

typedef struct {
    int lines_num;
    int words_num;
    int size;
} file_info;

int is_white_space(void);
void skipe_white_space(void);
int next_char(void);
_Bool test(int t);
_Bool _test(int t);

file_info get_file_info(FILE *);

#endif /* WordCounter_h */
