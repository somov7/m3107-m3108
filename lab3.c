#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
int arr_time[10000000];
struct time_struct
{
    short Year, Month, Day, Hour, Minute, Second;
};
void siftDown( int root, int bottom)
{
  int maxChild;
  int done = 0;
  while ((root * 2 <= bottom) && (!done)) 
  {
    if (root * 2 == bottom)
      maxChild = root * 2;
    else if (arr_time[root * 2] > arr_time[root * 2 + 1])
      maxChild = root * 2;
    else
      maxChild = root * 2 + 1;
    if (arr_time[root] < arr_time[maxChild]) 
    {
      int temp = arr_time[root];
      arr_time[root] = arr_time[maxChild];
      arr_time[maxChild] = temp;
      root = maxChild;
    }
    else
      done = 1;
  }
}
void heapSort(int array_size) 
{
  for (int i = (array_size / 2); i >= 0; i--)
    siftDown(i, array_size - 1);
  for (int i = array_size - 1; i >= 1; i--)
  {
    int temp = arr_time[0];
    arr_time[0] = arr_time[i];
    arr_time[i] = temp;
    siftDown(0, i - 1);
  }
}
int Translete (struct time_struct *t, char arr[])
{
    int second=0;
    //sec
    second+=arr[t->Second+1]-48+(arr[t->Second]-48)*10;
    //min
    second+=(arr[t->Minute+1]-48+(arr[t->Minute]-48)*10)*60;
    //Hour
    second+=(arr[t->Hour+1]-48+(arr[t->Hour]-48)*10)*3600;
    //Day
    second+=(arr[t->Day+1]-48+(arr[t->Day]-48)*10)*86400;
    //Month
    if (arr[t->Month]=='F' && arr[t->Month+1]=='e' && arr[t->Month+2]=='b')
        second+=2678400;
    else if (arr[t->Month]=='M' && arr[t->Month+1]=='a' && arr[t->Month+2]=='r')
        second+=2*2678400;
    else if (arr[t->Month]=='A' && arr[t->Month+1]=='p' && arr[t->Month+2]=='r')
        second+=3*2678400;
    else if (arr[t->Month]=='M' && arr[t->Month+1]=='a' && arr[t->Month+2]=='y')
        second+=4*2678400;
    else if (arr[t->Month]=='J' && arr[t->Month+1]=='u' && arr[t->Month+2]=='n')
        second+=5*2678400;
    else if (arr[t->Month]=='J' && arr[t->Month+1]=='u' && arr[t->Month+2]=='l')
        second+=6*2678400;
    else if (arr[t->Month]=='A' && arr[t->Month+1]=='u' && arr[t->Month+2]=='g')
        second+=7*2678400;
    else if (arr[t->Month]=='S' && arr[t->Month+1]=='e' && arr[t->Month+2]=='p')
        second+=8*2678400;
    else if (arr[t->Month]=='O' && arr[t->Month+1]=='c' && arr[t->Month+2]=='t')
        second+=9*2678400;
    else if (arr[t->Month]=='D' && arr[t->Month+1]=='e' && arr[t->Month+2]=='c')
        second+=10*2678400;
    else if (arr[t->Month]!='J')
        second+=(arr[t->Month+1]-48+(arr[t->Month]-48)*10);
    //Year
    if (arr[0]!='[')
        second+=(arr[t->Year+3]-48+(arr[t->Year+2]-48)*10+(arr[t->Year+1]-48)*100+(arr[t->Year]-48)*1000-1995)*32140800;
    else
        second+=(arr[t->Year+3]-48+(arr[t->Year+2]-48)*10+(arr[t->Year+1]-48)*100+(arr[t->Year]-48)*1000)*32140800;
    return second;
}
int main()
{
    setlocale(LC_ALL,"ru");
    FILE * fin = fopen("access_log_Jul95.txt", "r");

    struct time_struct time_st, time_add;
    int i=0, quotes_number=0, error_number=0, inquiry_number=0;
    short error=-1;

    char arr[10000];
    char time_cin[22];

    printf("Введите временное окно вида:\n[--/--/----:--:--:--]\n");
    scanf("%s", time_cin);
    time_add.Day=1;
    time_add.Month=4;
    time_add.Year=7;
    time_add.Hour=12;
    time_add.Minute=15;
    time_add.Second=18;
    int time_period=Translete(&time_add, time_cin);
    while (!feof(fin))
    {
        fscanf(fin, "%c", arr+i);
        if (arr[i]!='\n')
        {
            if (arr[i]=='"')
            {
                quotes_number++;
                error=i+2;
            }
            if (arr[i]=='[')
            { 
                time_st.Day=i+1;
                time_st.Month=i+4;
                time_st.Year=i+8;
                time_st.Hour=i+13;
                time_st.Minute=i+16;
                time_st.Second=i+19;
            } 
        }
        else if (quotes_number!=0)
        {
            arr_time[inquiry_number] = Translete(&time_st, arr);
            if(quotes_number==2 && arr[error]=='5')
            {
                error_number++;
                if (error_number==1) 
                    printf("Error querios:");
                for (int j=0; j<i; j++)
                    printf("%c", arr[j]);
            }
            i=0;
            error=-1;
            quotes_number=0;
            inquiry_number++;

        }
        i++;
    }
    heapSort(inquiry_number);

    int max=0, time_end=arr_time[0]+time_period, k=0;
    i=0;

    while (i<inquiry_number)
    {
        printf("\n%i %i", arr_time[i], time_end);
        if(arr_time[i]<time_end)
            k++;
        else
        {
            if (max<k)
                max=k;
            time_end+=time_period;
            if(arr_time[i]<time_end)
                k++;
            k=0;
        }
            i++;
    }
    if (max<k)
        max=k;

    printf("\nError number: %i", error_number);
    printf("\nMax_window: %i", max);
    return 0;
}