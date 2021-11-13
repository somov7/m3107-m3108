#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STARTSIZE 1000

int task2(FILE* file, int window) {
  fseek(file, 0, SEEK_SET);

  int secondsSize = 0;
  int cursize = STARTSIZE;
  int* seconds = (int*)calloc(sizeof(int), cursize);

  char* allMonths[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  char monthString[3];
  int monthInt;

  struct tm time;
  for (int i = 0; !feof(file); ++i) {
    if (i == cursize) {
      cursize *= 2;
      seconds = realloc(seconds, sizeof(int)* cursize);
    }
    char* remoteAddr = calloc(sizeof(char), 2048);
    char* localTime = calloc(sizeof(char), 2048);
    char* request = calloc(sizeof(char), 2048);
    int status = 0;
    int bytesSend = 0;

    fscanf(file, "%s - - [%[^]]] \"%[^\"]\" %d %d",
      remoteAddr,
      localTime,
      request,
      &status,
      &bytesSend
    );

    sscanf(localTime,
      "%d/%3s/%d:%d:%d:%d",
      &time.tm_mday,
      monthString,
      &time.tm_year,
      &time.tm_hour,
      &time.tm_min,
      &time.tm_sec
    );

    for (int j = 0; j < 12; j++) {
      if (strcmp(allMonths[j], monthString) == 0) {
        monthInt = j + 1;
        break;
      }
    }

    time.tm_year -= 1900;
    time.tm_mon = monthInt;

    seconds[i] = mktime(&time);

    ++secondsSize;
  }

  int tmp = 0, counter = 1, maxRequest = 0, j = 1, first, last;
  for (int i = 1; i < secondsSize; i++) {
    tmp += seconds[i] - seconds[i - 1];
    counter++;

    while (tmp > window && j + 1 < secondsSize && j > 0) {
      tmp -= (seconds[j] - seconds[j - 1]);
      j++;
      counter--;
    }

    if (counter > maxRequest) {
      maxRequest = counter;
      first = i;
      last = j;
    }
  }

  printf("Window: %d, max requests: %d\n", window, maxRequest);
}
