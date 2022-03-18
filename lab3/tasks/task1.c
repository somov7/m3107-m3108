#include <stdio.h>
#include <stdlib.h>
#include "tasks.h"

void task1(FILE* file) {
  fseek(file, 0, SEEK_SET);

  int counter = 0;

  for (int i = 0; !feof(file); ++i) {
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

    int st = status / 100;
    if (st == 5) {
      printf("%d. %s\n", counter + 1, remoteAddr);
      counter++;
    }
  }
  printf("5xx code: %d\n", counter);
}
