#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_REQUEST_LENGTH 4200

typedef struct {
  char remote_address[2048];
  char local_time[32];
  char request[2048];
  int status;
  int bytes_send;
} Log;

FILE *file;
void readParams(int argc, char *argv[]) {
  if (argv[1] != NULL) {
    file = fopen(argv[1], "r");
    if (file == NULL) {
      printf("%s\n", "File doesn't exist.");
      return;
    }
  } else {
    printf("%s\n", "Error in argument.");
    return;
  }
}

int countSeconds(FILE *file) {
  int countOfStr = 1;
  char c = fgetc(file);
  while (c != EOF) {
    if (c == '\n') countOfStr++;
    c = fgetc(file);
  }

  return countOfStr;
}

void program()  {
  int *secondsArr;
  int n = 0, months, timeWindow;
  char allMonths[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

  printf("Enter interval (in seconds)\n");
  scanf("%d", &timeWindow);
  
  int countOfStr = countSeconds(file);
  secondsArr = (int *) malloc(sizeof(int) * countOfStr);

  fseek(file, 0, SEEK_SET);
  char month[4];
  struct tm time;
  Log log;
  
  int number = 0, errorsCount = 0;
  while (!feof(file)) {
    fscanf(file, "%s - - [%[^]]] \"%[^\"]\" %d %d", log.remote_address, log.local_time, log.request, &log.status, &log.bytes_send);

    log.status /= 100;
    if (log.status == 5) {
      number++;
      printf("%d) %s %d\n", number, log.request, errorsCount);
      errorsCount = errorsCount + 1;
    }

    sscanf(log.local_time, "%d/%3s/%d:%d:%d:%d", &time.tm_mday, month,
    &time.tm_year, &time.tm_hour, &time.tm_min, &time.tm_sec);
    time.tm_year -= 1900;

    for (int i = 0; i < 12; i++) {
      if (allMonths[i] == month) {
        months = i;
        break;
      }
    }

    time.tm_mon = months;
    secondsArr[n] = mktime(&time);
    n++;
  }

  // max time window
  int temp = 0, count = 1, requests = 0, j = 1, begin, end;
  for (int i = 1; i < countOfStr; i++) {
    temp += secondsArr[i] - secondsArr[i - 1];
    count++;
    while (temp > timeWindow && j + 1 < countOfStr
) {
      temp -= (secondsArr[j] - secondsArr[j - 1]);
      j++;
      count--;
    }
    if (count > requests) {
      requests = count;
      begin = i;
      end = j;
    }
  }

  fseek(file, 0, SEEK_SET);

  char interval_start[32], interval_end[32];
  char *buf = malloc(MAX_REQUEST_LENGTH);
  for (int i = 0; i < countOfStr; i++) {
    fgets(buf, MAX_REQUEST_LENGTH, file);
    if (i == begin) {
      sscanf(buf, "%*s - - [%s]", interval_start);
    }
    if (i == end)  {  
      sscanf(buf, "%*s - - [%s]", interval_end);
    }
  }

  printf("\n5xx errors count: %d\n", errorsCount);
  printf("\nMax requests in %d seconds window: %d \n", timeWindow, requests);
  printf("%s to %s\n", interval_start, interval_end);
}

int main(int argc, char *argv[]) {
  readParams(argc, argv);
  program();
  return 0;
}