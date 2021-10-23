//
//  main.c
//  BigInteger
//
//  Created by Антон Чемодуров on 02.10.2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "uint1024_t.h"


/**
 time_t begin = clock();
 time_t end = clock();
 double time = (double)(end - begin) / CLOCKS_PER_SEC;
 printf("%f\n", time);
 */

typedef struct {
    uint1024_t num1;
    uint1024_t num2;
    char *ans;
}data;

int get_test_data(data *test, FILE *test_file);
double test(const char *file_name, uint1024_t (*func) (uint1024_t, uint1024_t));

int main(int argc, const char * argv[]) {
    double add_time = test("/Users/anton/CLionProjects/uint1024_t/test_add.data", add_op);
    if (add_time == -1) return 0;
    double sub_time = test("/Users/anton/CLionProjects/uint1024_t/test_sub.data", subtr_op);
    if (sub_time == -1) return 0;
    double mul_time = test("/Users/anton/CLionProjects/uint1024_t/test_mul.data", mult_op);
    if (mul_time == -1) return 0;
    printf("all test  passed\ntime %f %f %f", add_time, sub_time, mul_time);
}

double test(const char *file_name, uint1024_t (*func) (uint1024_t, uint1024_t)) {
    FILE *test_file = fopen(file_name, "r");
    if (test_file == NULL) {
        perror("error while reading file");
        return -1;
    }
    data test;
    double time = 0;
    int i = 1;
    while (get_test_data(&test, test_file) != -1) {
        time_t begin = clock();
        uint1024_t ans = func(test.num1, test.num2);

        time_t end = clock();
        time += (double)(end - begin) / CLOCKS_PER_SEC;

        char *ans_str = toString(ans);
        if (strcmp(ans_str, test.ans) != 0) {
            printf("test %d failed\n", i);
            puts("expected:");
            puts(test.ans);
            puts("actual:");
            puts(ans_str);
            return -1;
        } else {
            printf("test %d passed\n" , i);
        }
        free(ans_str);
        //free(test.ans);
        i++;
    }
    fclose(test_file);
    return time;
}


/**
 *
 * @param test
 * @param test_file указател на дата файл с тестами \n
 * формат теста: num1 ws num2 ws ans ws \n
 * @return
 * -1 в случае ошибки или если фаил закончился
 * 0 если функция отработала корректно
 */
int get_test_data(data *test, FILE *test_file) {

    if (test_file == NULL || test == NULL) {
        return -1;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read = getline(&line, &len, test_file);
    if (read == -1 || line == 0) {
        //perror("bad arguments");
        return -1;
    }
    //puts(line);
    test->num1 = from_str(strtok(line, "_"));
    test->num2 = from_str(strtok(NULL, "_"));
    test->ans = strtok(NULL, "_");
    //free(line);
    return 0;
}

