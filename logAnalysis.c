#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>

int main() {
    FILE  *fin1, *fin2;
//    fin1 = fopen("testlog.txt", "r");
//    fin2 = fopen("testlog.txt", "r");
    fin1 = fopen("access_log_Jul95.txt", "r");
//    fin2 = fopen("access_log_Jul95.txt", "r");
    char str[500];
    int cnt = 0;
    int num = 0;
    int flag = 0;
    uint32_t amount_of_5xx = 0;
    char sep[2] = " ";
    int c = fgetc(fin1);

    while(1){
        if (c != '\n' && c != EOF){
            str[cnt] = c;
            cnt++;
            if (c != ' ' && flag == 0) {
                num++;
                flag = 1;
            }
            else if (c == ' ' && flag == 1){
                flag = 0;
            }
        }
        // в else нет работы с символом (только со строками)
        else{
            char s[cnt];
            char *arr[num][cnt];
            char *res;
            strcpy(s, str);
            res = strtok(str, sep);

            for (int i = 0; i < num; i++){
                arr[i][cnt] = res;
                res = strtok(NULL,sep);
            }

            int status_code = atoi(arr[num - 2][cnt]);

            if (status_code >= 500 && status_code < 600){
                amount_of_5xx++;
                printf("%s\n", s);
                // printf("code is %s\n", temp);
                // printf("num is %d\n", num);
            }

            flag = 0;
            num = 0;
            cnt = 0;
            memset(str,0,sizeof(str));
            // printf("next line %d\n");
        }
        if (c == EOF){
            break;
        }
        c = fgetc(fin1);
    }
    printf("completed successfully\n");
    printf("%u\n", amount_of_5xx);
    return 0;
}
