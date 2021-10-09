//
//  main.c
//  WordCount
//
//  Created by Антон Чемодуров on 09.09.2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "WordCounter.h"


int main(int argc, const char * argv[]) {
    if (argc < 2) {
        perror("Enter file name");
        exit(0);
    }
    FILE *file_ = fopen(argv[argc-1], "r");
    if (file_ == NULL) {
        perror("file doesn't exist");
        exit(0);
    }
    file_info info = get_file_info(file_);

    const char* short_options = "lwc";
    const struct option long_options[] = {
		{"lines", no_argument, NULL, 'l'},
		{"words", optional_argument, NULL, 'w'},
		{"bytes", required_argument, NULL, 'c'},
		{NULL,0,NULL,0}
	};

    int res;
    while ((res = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
        switch (res)
        {
        case 'w':
            printf("words %d\n", info.words_num);
            break;
        case 'l':
             printf("lines %d\n", info.lines_num);
            break;
        case 'c':
             printf("bytes %d\n", info.size);
            break;
        default:
            break;
        }
    }

    //printf("\t%d %d %d %s\n", info.lines_num, info.words_num, info.size, argv[1]);
    
    
    
}


