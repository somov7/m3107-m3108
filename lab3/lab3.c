#include <stdio.h>
#include <string.h>
#include <stdlib.h>
char ch;
typedef struct
{
    char remote_adr[400];
    char local_time[50];
    char request[400];
    int status;
    int bytes_send;
} str;
void scanf_str(str *x, char input[1000])
{
    int j = 0;
    while (input[j] != ' ')
    {
        x -> remote_adr[j] = input[j];
        j++;
    }
    j++;
    while (input[j] != '[')
    {
        j++;
    }
    j++;
     for (int i = 0; input[j] != ']'; i++)
        {
            x -> local_time[i] = input[j];
            j++;
        }
    j++;
    while (input[j] != '\"')
    {   
        j++;
    }
    j++;
    for (int i = 0; input[j] != '\"' || input[j+1] != ' '; i++)
        {

        x -> request[i] = input[j];
        j++;
        }
    j+=2;
    char tmp[12];
    char temp[3];
    for(int i = 0; i<3; i++, j++)
    {
        temp[i] = input[j];
    }
    x -> status = atoi(temp);
    j++;
    for (int i = 0; input[j] != '\n'; i++, j++)
    {
        tmp[i] = input[j];
    }
    x -> bytes_send = atoi(tmp);
}
void printf_str(str x)
{
    printf("%s",x.remote_adr);
    printf("%s"," - - [");
    printf("%s",x.local_time);
    printf("%s","] \"");
    printf("%s",x.request);
    printf("%s","\" ");
    printf("%d",x.status);
    printf("%c",' ');
    printf("%d,\n",x.bytes_send);
}
int error_str(str x)
{
    if (x.status/100 == 5)
    {
        return 1;
    }
}
void zeroing_str(str *x)
{
    memset(x->remote_adr, 0, 400);
    memset(x->local_time, 0, 50);
    memset(x->request, 0, 400);
    x->status = 0;
    x->bytes_send = 0;
}
int date(char *input)
{
    int c=0;
    char temp[4];
    while (input[c] != '[')
    {
        c++;
    }
    c++;
    temp[0] = input[c]; temp[1] = input[c+1]; // day
    unsigned int t1 = (atoi(temp) - 1) * 86400;
    while (input[c] != ':')
    {
        c++;
    }
    c++;
    temp[0] = input[c]; temp[1] = input[c+1];   // hour
    t1 = t1 + (atoi(temp) * 3600);
    while (input[c] != ':')
    {
        c++;
    }
    c++;
    temp[0] = input[c]; temp[1] = input[c+1];   // minute
    t1 = t1 + (atoi(temp) * 60);
    c+=3;
    temp[0]= input[c]; temp[1] = input[c+1];
    t1 = t1 + atoi(temp);
    return t1;
}
int interval(unsigned int t1, int gap)
{
    unsigned int t2 = t1 + gap;
    return t2;
}
int main()
{
    FILE *fp;
    char input1[1000];
    fp = fopen("access_log_Jul95.log","r");
    for(unsigned long z = 0; z < 1891713; z++){
    fgets(input1, 1000, fp);
    str str1;
    scanf_str(&str1, input1);
    if (error_str(str1) == 1)
    {
    printf_str(str1);
    }
    zeroing_str(&str1);
    }
    char input2[1000];
    char input3[1000];
    int gapp;
    scanf("%d", &gapp);
    printf("%d \n", gapp);
    int temp = 0;
    int record = 0;
    int record_time1 = 0;
    int record_time2 = 0;
    FILE *fp1;
    FILE *fp2;
    fp1 = fopen("access_log_Jul95.log","r");
    fp2 = fopen("access_log_Jul95.log","r");
    for(unsigned long z = 0; z < 1891713; z++)
    {
        fgets(input2, 1000, fp1);
        int i = 0;
                while(interval(date(input2), gapp) >= date(input3))
                {   
                    while (i >= 1)
                    {
                        fgets(input3, 1000, fp2);
                        i = i - 1;
                        continue;
                    }
                    fgets(input3, 1000, fp2);
                    temp++;
                }
                if (temp > record)
                {
                    record = temp;
                    record_time1 = date(input2);
                    record_time2 = date(input3);
                }
                temp = 0;
        i++;
    }
    printf("%d \n", record);
    int day = (record_time1 / 86400) + 1;
    int hours = (record_time1 % 86400) / 3600;
    int minutes = ((record_time1 % 86400) % 3600) / 60;
    int seconds = (((record_time1 % 86400) % 3600) % 60);
    printf("%d Jul %d:%d:%d \n", day, hours, minutes, seconds);    // (record_time1 / 86400) + 1     (record_time1 % 86400) / 3600    ((record_time1 % 86400) % 3600) / 60   (((record_time1 % 86400) % 3600) % 60)
    day = (record_time2 / 86400) + 1;
    hours = (record_time2 % 86400) / 3600;
    minutes = ((record_time2 % 86400) % 3600) / 60;
    seconds = (((record_time2 % 86400) % 3600) % 60 - 1);
    printf("%d Jul %d:%d:%d \n", day, hours, minutes, seconds);
}
