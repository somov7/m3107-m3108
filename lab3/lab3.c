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
    unsigned int t1 = (atoi(temp)) * 86400;
    c+=3;
    temp[0] = input[c]; temp[1] = input[c+1]; temp[2] = input[c+2];
                if (temp == "Jan")
                {
                    t1 = t1;
                }
                if (temp == "Feb")
                {
                    t1 = t1 + 31;
                }
                char tmp[4];
                tmp[0] = input[c+4]; tmp[1] = input[c+5]; tmp[2] = input[c+6]; tmp[3] = input[c+7]; 
                if (atoi(tmp) % 4 == 0 && temp == "Mar")
                {
                    if (atoi(tmp) % 100 == 0)
                    {   
                        if (atoi(tmp) % 400 == 0)
                    {
                        t1 = t1 + 31 + 29;
                    }
                    t1 = t1 + 31 + 28;
                    }
                    t1 = t1 + 31 + 29;
                }
                if (temp == "Mar")
                {
                    t1 = t1 + 31 + 28;
                }
                if (temp == "Apr")
                {
                    t1 = t1 + 31 + 28 + 31;
                }
                if (temp == "May")
                {
                    t1 = t1 + 31 + 28 + 31 + 30;
                }
                if (temp == "Jun")
                {
                    t1 = t1 + 31 + 28 + 31 + 30 + 31;
                }
                if (temp == "Jul")
                {
                    t1 = t1 + 31 + 28 + 31 + 30 + 31 + 30;
                }
                if (temp == "Aug")
                {
                    t1 = t1 + 31 + 28 + 31 + 30 + 31 + 30 + 31;
                }
                if (temp == "Sep")
                {
                    t1 = t1 + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31;
                }
                if (temp == "Oct")
                {
                    t1 = t1 + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30;
                }
                if (temp == "Nov")
                {
                    t1 = t1 + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31;
                }
                if (temp == "Dec")
                {
                    t1 = t1 + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30;
                }
    temp[0] = input[c]; temp[1] = input[c+1]; temp[2] = input[c+2]; temp[3] = input[c+3]; // year
    for (int year = 0; year <= atoi(temp); year++)
    {
        if (year % 4)
        {
            if (year % 100)
            {
                if (year % 400)
                {
                    t1 = t1 + 366;
                }
                else
                {
                    t1 = t1 +365;
                }
            }
            else
            {
                t1 = t1 + 366;
            }
        }
        else
        {
            t1 = t1 + 365;
        }
    }
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
    fp = fopen("access.log","r");
    int h = 1;
    while(!feof(fp)){
    for(unsigned long z = 0; input1[z] >= 0; z++){
    fgets(input1, 1000, fp);
    str str1;
    scanf_str(&str1, input1);
    if (error_str(str1) == 1)
    {
    printf("%d ", h);
    printf_str(str1);
    h++;
    }
    zeroing_str(&str1);
    }
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
    fp1 = fopen("access.log","r");
    fp2 = fopen("access.log","r");
    fgets(input2, 1000, fp1);
    while(!feof(fp1) && !feof(fp2))
    {
    for(unsigned long z = 0; input2[z] >= 0; z++)
    {   
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
        fgets(input2, 1000, fp1);
        i++;

    }
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
